#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <chrono>

using namespace std;

vector < vector < int > > MyVector;

string line;
int quantityofDataLines = 0;
int quantityofMachines = 0;
ifstream myfile("data.txt");

vector<int> JhonsonsAlgorithm(vector<vector<int>> Myvector, int quantityofDataLines);
void printVector(const vector<int> VEC);
vector<int> getReorganisedVector(const vector<int> VEC, const vector<int> NEW_ORDER);
int calculate_Cmax(vector<vector<int>> Myvector, int quantityofMachines, int quantityofDataLines);

int main() {
	string line;
	string dataName;
	int temp, MachineIndex;
	vector<int> newOrder;

	if (myfile.is_open()) {

		int tab[3] = { 1, 2, 3};
		for (int i = 0; i < 3; i++) {
			dataName = "data00" + to_string(tab[i]);

			// Skip useless data
			while (getline(myfile, line) && line != dataName) {}

			// read quantity of measures
			myfile >> quantityofDataLines >> quantityofMachines;
			for (int i = 0; i < quantityofMachines; i++)
			{
				MyVector.push_back(vector < int >());
			}
			// read data
			for (int i = 0; i < quantityofDataLines; i++) {
				for (int j = 0; j < quantityofMachines; j++)
				{
					myfile >> MachineIndex;
					myfile >> temp;
					MyVector[j].push_back(temp);
				}
			}

			cout << "###########" << dataName << "###########" << endl;
			cout << "Cmax for initialorder = " << calculate_Cmax(MyVector, quantityofMachines, quantityofDataLines) << endl;

			auto start = std::chrono::steady_clock::now();

			newOrder = JhonsonsAlgorithm(MyVector, quantityofDataLines);
			MyVector[0] = getReorganisedVector(MyVector[0], newOrder);
			MyVector[1] = getReorganisedVector(MyVector[1], newOrder);

			auto end = std::chrono::steady_clock::now();
			chrono::duration<double> elapsed_seconds = end - start;
			cout << endl << "Johnson’s Algorithm time: " << elapsed_seconds.count() << " s" << endl;

			cout << "Cmax for Johnson’s Algorithm = " << calculate_Cmax(MyVector, quantityofMachines, quantityofDataLines) << endl << endl << endl;

			//clear vectors
			MyVector.clear();
			newOrder.clear();

		}
		myfile.close();



	}
	else {
		cerr << "Unable to open file. Exit program.\n";
	}
	return 0;
}




vector<int> getReorganisedVector(const vector<int> VEC, const vector<int> NEW_ORDER) {
	if (VEC.size() != NEW_ORDER.size()) {
		throw invalid_argument("Invalid arguments! Vectors sizes are diffrent. can not get reorder vector.\n");
	}

	vector<int> reOrderedVector;
	int indexOfNextSmallestNumber;

	for (int i = 0; i < NEW_ORDER.size(); i++) {
		indexOfNextSmallestNumber = NEW_ORDER[i];
		reOrderedVector.push_back(VEC[indexOfNextSmallestNumber]);
	}
	return reOrderedVector;
}

void printVector(const vector<int> VEC) {
	cout << " [";
	for (int i = 0; i < VEC.size(); i++) {
		cout << " " << VEC[i];
	}
	cout << " ]\n";
}


int calculate_Cmax(vector<vector<int>> Myvector, int quantityofMachines, int quantityofDataLines) {
	//wektor pomocniczy
	vector < vector < int > > Vec;

	for (int i = 0; i < quantityofMachines; i++)
	{
		Vec.push_back(vector < int >());
	}
	//wektor czasu zakończenia zad dla maszyn
	for (int i = 0; i < quantityofMachines; i++)
	{
		for (int j = 0; j < quantityofDataLines; j++)
		{
			if (i == 0) { // dla pierwszej maszyny zadania jedno po drugim
				if (j == 0) {
					Vec[i].push_back(MyVector[i][j]);
				}
				else
				{
					Vec[i].push_back(MyVector[i][j] + Vec[i][j-1]);
				}
			}
			else { //dla nast maszyn po zakonczeniu na poprzedniej
				if (j == 0) {//dla pierwszego zad napewno
					Vec[i].push_back(MyVector[i][j] + Vec[i - 1][j]);
				}
				else//dla nastepnych jesli maszyna skonczyla poprzednie
				{
					if (Vec[i][j-1] <= Vec[i-1][j]) {
						Vec[i].push_back(MyVector[i][j] + Vec[i - 1][j]);
					}
					else
					{
						Vec[i].push_back(MyVector[i][j] + Vec[i][j-1]);
					}
				}
			}
		}
	}
	return Vec[quantityofMachines-1][quantityofDataLines-1];
}

vector<int> JhonsonsAlgorithm(vector<vector<int>> Myvector, int quantityofDataLines) {

	//newOrder vector
	vector<int> newOrder;
	int l;
	for (l = 0; l < quantityofDataLines; l++)
	{
		newOrder.push_back(1);
	}
	int BIGNUMBER=9999;
	int left=0, right=quantityofDataLines-1;
	int min1, min2, smallestindex1 = BIGNUMBER, smallestindex2 = BIGNUMBER;
	for (int j = 0; j < quantityofDataLines; j++) {
		min1 = BIGNUMBER;
		min2 = BIGNUMBER;
		for (int i = 0; i < quantityofDataLines; i++) //szukanie najmniejszego czasu na pierwszej i drugiej maszynie
		{
			if (min1 > Myvector[0][i])
			{
				min1 = Myvector[0][i];
				smallestindex1 = i;
			}
			if (min2 > Myvector[1][i])
			{
				min2 = Myvector[1][i];
				smallestindex2 = i;
			}
		}
		if (min1 < min2) { //sprawdzenie min czasów na maszynach
			newOrder[left] = smallestindex1;
			Myvector[0][smallestindex1]=BIGNUMBER;
			Myvector[1][smallestindex1] = BIGNUMBER;
			left++;
		}
		else {
			newOrder[right] = smallestindex2;
			Myvector[0][smallestindex2] = BIGNUMBER;
			Myvector[1][smallestindex2] = BIGNUMBER;
			right--;
		}
	}
	return newOrder;
}