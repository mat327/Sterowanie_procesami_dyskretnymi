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

vector<int> BruteForce(vector<int> P, int size, vector<int> newOrder);
void printVector(const vector<int> VEC);
vector<int> getReorganisedVector(const vector<int> VEC, const vector<int> NEW_ORDER);
int calculate_Cmax(vector<vector<int>> Myvector, int quantityofMachines, int quantityofDataLines);

int main() {
	string line;
	string dataName;
	int temp, MachineIndex;
	vector<int> newOrder;

	if (myfile.is_open()) {

		int tab[6] = { 1, 2, 3, 4, 5, 6};
		for (int i = 0; i < 6; i++) {
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
			//Brute Force (algorytm Dijkstry)
			//numbers of combinations
			long long silnia = 1;
			for (int j = quantityofDataLines; j > 1; j--)
				silnia *= j;

			//vector of indexs
			vector<int> indexVec;
			int l;
			for (l = 0; l < quantityofDataLines; l++)
			{
				indexVec.push_back(l);
			}

			//for all combinations
			int lowestCmax = calculate_Cmax(MyVector, quantityofMachines, quantityofDataLines);
			for (int j = 0; j < silnia - 1; j++) {
				newOrder = BruteForce(MyVector[1], quantityofDataLines, indexVec);
				for (int j = 0; j < quantityofMachines; j++)
				{
					MyVector[j] = getReorganisedVector(MyVector[j], newOrder);
				}
				int newCmax = calculate_Cmax(MyVector, quantityofMachines, quantityofDataLines);
				if (newCmax < lowestCmax)
				{
					lowestCmax = newCmax;
				}

			}
			auto end = std::chrono::steady_clock::now();
			chrono::duration<double> elapsed_seconds = end - start;
			cout << endl << "brute force time: " << elapsed_seconds.count() << " s" << endl;

			cout << "LOWEST Cmax = " << lowestCmax << endl << endl << endl;
			
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
	//wektor czasu zak zad dla maszyn
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
				if (j == 0) {//dla pierwszego zad napewno maszyna jest gotowa
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

vector<int> BruteForce(vector<int> P, int size, vector<int> newOrder)
{
	int i, j, k;

	if (size < 2)
		return newOrder;

	/* wyznaczanie pierwszego od prawej elementu
	 * mniejszego niz jego sasiad z prawej strony
	 */
	int previ = size - 1;
	i = previ - 1;
	while ((previ > 0) && (P[i] >= P[previ]))
	{
		i--;
		previ--;
	}

	/* wyznaczanie elementu wiekszego od znalezionego */
	if (i > -1) {
		j = size - 1;
		while ((j > 0) && (P[j] <= P[i]))
			j--;

		/* zamiana miejscami dwoch znalezionych wyzej elementow */
		if (j > 0) { //warunek sprawdzajacy czy nie podmieniamy tej samej liczby
			k = newOrder[i];
			newOrder[i] = newOrder[j];
			newOrder[j] = k;
		}
	}



	/* odbicie lustrzane szeregu elementow od indeksu i+1 do konca tablicy */
	for (i++, j = size - 1; i < j; i++, j--) {
		k = newOrder[i];
		newOrder[i] = newOrder[j];
		newOrder[j] = k;
	}
	return newOrder;
}