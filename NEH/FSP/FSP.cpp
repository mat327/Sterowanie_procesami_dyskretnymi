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

void NEHAlgorithm(vector<vector<int>> Myvector, int quantityofDataLines, int quantityofMachines);
vector<int> getReorganisedVector(const vector<int> VEC, const vector<int> NEW_ORDER);
bool isVectorContainingOnlySmallNumbers(const vector<int> VEC, const int SMALL_NUMBER);
vector<int> getNewOrderWithSortWj(vector<int> localWj);
int calculate_Cmax(vector<vector<int>> VEC, int quantityofMachines, int quantityofDataLines);

int main() {
	string line;
	string dataName;
	int temp, MachineIndex;

	for (int i = 1; i <= 120; i++) {
		string filename;
		if (i < 10) {
			filename = "ta00" + to_string(i) + ".txt";
		}
		if (i >= 10 && i < 100)
		{
			filename = "ta0" + to_string(i) + ".txt";
		}
		if (i>=100)
		{
			filename = "ta" + to_string(i) + ".txt";
		}
		ifstream myfile(filename);
		if (myfile.is_open()) {
			// read quantity of measures
			myfile >> dataName >> quantityofDataLines >> quantityofMachines;
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

			NEHAlgorithm(MyVector, quantityofDataLines, quantityofMachines);

			auto end = std::chrono::steady_clock::now();
			chrono::duration<double> elapsed_seconds = end - start;
			cout << endl << "NEH Algorithm time: " << elapsed_seconds.count() << " s" << endl << endl << endl;

			//clear vectors
			MyVector.clear();
			myfile.close();
		}
		else {
			cerr << "Unable to open file.\n";
		}

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

vector<int> getNewOrderWithSortWj(vector<int> localWj) {
	const int SMALL_NUMBER = -1;

	vector<int> newOrder;
	int biggestNumber;
	int biggestNumberIndex;

	while (isVectorContainingOnlySmallNumbers(localWj, SMALL_NUMBER) == false) {

		biggestNumber = localWj[0];
		biggestNumberIndex = 0;

		// find index of the biggest number in vector
		for (int i = 0; i < localWj.size(); i++) {
			if (biggestNumber < localWj[i]) {

				biggestNumber = localWj[i];
				biggestNumberIndex = i;
			}
		}
		newOrder.push_back(biggestNumberIndex);
		localWj[biggestNumberIndex] = SMALL_NUMBER;
	}
	return newOrder;
}
bool isVectorContainingOnlySmallNumbers(const vector<int> VEC, const int SMALL_NUMBER) {
	for (int i = 0; i < VEC.size(); i++) {
		if (VEC[i] != SMALL_NUMBER) return false;
	}
	return true;
}

int calculate_Cmax(vector<vector<int>> VEC, int quantityofMachines, int quantityofDataLines) {
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
					Vec[i].push_back(VEC[i][j]);
				}
				else
				{
					Vec[i].push_back(VEC[i][j] + Vec[i][j-1]);
				}
			}
			else { //dla nast maszyn po zakonczeniu na poprzedniej
				if (j == 0) {//dla pierwszego zad napewno
					Vec[i].push_back(VEC[i][j] + Vec[i - 1][j]);
				}
				else//dla nastepnych jesli maszyna skonczyla poprzednie
				{
					if (Vec[i][j-1] <= Vec[i-1][j]) {
						Vec[i].push_back(VEC[i][j] + Vec[i - 1][j]);
					}
					else
					{
						Vec[i].push_back(VEC[i][j] + Vec[i][j-1]);
					}
				}
			}
		}
	}
	return Vec[quantityofMachines-1][quantityofDataLines-1];
}

void NEHAlgorithm(vector<vector<int>> Myvector, int quantityofDataLines, int quantityofMachines) {

	//Wj vector
	vector<int> Wj, OrderWj;
	vector < vector < int > > VEC, VECCopy;
	vector<int> newOrder, newOrderCopy, bestnewOrder;
	
	for (int l = 0; l < quantityofDataLines; l++) //calculate W for tasks
	{
		int w = 0;
		for (int i = 0; i < quantityofMachines; i++) w = w + Myvector[i][l];
		Wj.push_back(w);
	}
	OrderWj = getNewOrderWithSortWj(Wj); //sort tasks by W 

	for (int j = 0; j < quantityofMachines; j++) VEC.push_back(vector < int >());

	for (int i = 0; i < quantityofDataLines; i++)
	{
		for (int j = 0; j < quantityofMachines; j++) VEC[j].push_back(MyVector[j][OrderWj[i]]); //add next Wj value
		newOrder.push_back(i); //initial order
		bestnewOrder = newOrder;
		int Cmax = calculate_Cmax(VEC, quantityofMachines, i+1);// Cmax for first combination
		
		/*cout << endl << endl;
		for (int x = 0; x <= i; x++) cout << newOrder[x] << " ";
		cout << Cmax << endl;*/

		int Cmax2 = 0;
		for (int k = 0; k < i; k++) //check all combination
		{
			VECCopy = VEC;
			newOrderCopy = newOrder;
			for (int l = i-k; l > 0; l--) { //generate new comb order (opearte on copy)
				int zmienna = newOrderCopy[k+l-1];
				newOrderCopy[k+l-1] = newOrderCopy[k+l];
				newOrderCopy[k+l] = zmienna;
			}
			for (int j = 0; j < quantityofMachines; j++) {
				VECCopy[j]=getReorganisedVector(VEC[j], newOrderCopy); // reorgenised VEC to new comb and save to copy
			}
			Cmax2 = calculate_Cmax(VECCopy, quantityofMachines, i+1); //calculate Cmax for new comb

			//for (int x = 0; x <= i; x++) cout << newOrderCopy[x] << " ";//
			//cout << Cmax2 << endl;//

			if (Cmax2 <= Cmax) // if Cmax value is smaller than best Cmax, change
			{
				Cmax = Cmax2;
				bestnewOrder = newOrderCopy;
			}
		}

		if (i != 0) { // do only for 2 or more tasks
			//for (int x = 0; x <= i; x++) cout << bestnewOrder[x] << " ";//
			for (int j = 0; j < quantityofMachines; j++) VEC[j]=getReorganisedVector(VEC[j], bestnewOrder);// reorgenised original VEC to best comb
		}
	}

	cout << "Cmax for NEH Algorithm = " << calculate_Cmax(VEC, quantityofMachines, quantityofDataLines) << endl;

	//clear vectors
	Wj.clear();
	OrderWj.clear();
	VEC.clear();
	VECCopy.clear();
	newOrder.clear();
	bestnewOrder.clear();
	newOrderCopy.clear();
}