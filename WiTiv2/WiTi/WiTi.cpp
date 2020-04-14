#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <chrono>

using namespace std;

vector<int> P, W, D;

string line;
int quantityOfDataLines = 0;
int quantityofElements = 0;
ifstream myfile("data.txt");

vector<int> BruteForce(vector<int> P, int size);
void printVector(const vector<int> VEC);
bool isVectorContainingOnlyBigNumbers(const vector<int> VEC, const int BIG_NUMBER);
vector<int> getNewOrderWithSortD(vector<int> localD);
vector<int> getReorganisedVector(const vector<int> VEC, const vector<int> NEW_ORDER);
void printVectorWithIncrementedValues(vector<int> VEC);
int calculate_Penalty(vector<int> localP, vector<int> localW, vector<int> localD);

int main() {
	string line;
	string dataName;
	int tempP, tempW, tempD;
	vector<int> newOrder;

	vector<int> newP;
	vector<int> newW;
	vector<int> newD;

	vector<int> Penalty;

	if (myfile.is_open()) {

		int tab[11] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, };
		for (int i = 0; i < 11; i++) {
			dataName = "data" + to_string(tab[i]);

			// Skip useless data
			while (getline(myfile, line) && line != dataName) {}

			// read quantity of measures
			myfile >> quantityOfDataLines >> quantityofElements;

			// read data
			for (int i = 0; i < quantityOfDataLines; i++) {
				myfile >> tempP >> tempW >> tempD;
				P.push_back(tempP);
				W.push_back(tempW);
				D.push_back(tempD);
			}

			cout << "Penalty for initialorder = " << calculate_Penalty(P, W, D) << endl;

			auto start = std::chrono::steady_clock::now();
			//Brute Force (algorytm Dijkstry)
			//numbers of combinations
			long long silnia = 1;
			for (int j = quantityOfDataLines; j > 1; j--)
				silnia *= j;

			//for all combinations
			int lowestPenalty = calculate_Penalty(P, W, D);
			for (int j = 0; j < silnia - 1; j++) {
				newOrder = BruteForce(P, quantityOfDataLines);
				P = getReorganisedVector(P, newOrder);
				W = getReorganisedVector(W, newOrder);
				D = getReorganisedVector(D, newOrder);
				int newpenalty = calculate_Penalty(P, W, D);
				if (newpenalty < lowestPenalty)
				{
					lowestPenalty = newpenalty;
				}

			}
			auto end = std::chrono::steady_clock::now();
			chrono::duration<double> elapsed_seconds = end - start;
			cout << endl << "brute force time: " << elapsed_seconds.count() << " s"<< endl ;

			cout <<  "LOWEST PENALTY = " << lowestPenalty << endl << endl;

			//SortD
			auto start2 = std::chrono::steady_clock::now();
			vector<int> newOrder = getNewOrderWithSortD(D);

			vector<int> newP = getReorganisedVector(P, newOrder);
			vector<int> newW = getReorganisedVector(W, newOrder);
			vector<int> newD = getReorganisedVector(D, newOrder);

			Penalty.push_back(calculate_Penalty(newP, newW, newD));

			auto end2 = std::chrono::steady_clock::now();
			chrono::duration<double> elapsed_seconds2 = end2 - start2;
			cout << "sortD time: " << elapsed_seconds.count() << " s" << endl;

			cout << "New order for " + dataName + " with SortD "; printVector(newOrder);
			cout << "Calculated penalty for " + dataName + " = " << Penalty[i] << endl << endl << endl << endl;

			//clear vectors
			P.clear(); W.clear(); D.clear();
			newOrder.clear(); newP.clear(); newW.clear(); newD.clear();

		}
		myfile.close();



	}
	else {
		cerr << "Unable to open file. Exit program.\n";
	}
	return 0;
}




vector<int> getNewOrderWithSortD(vector<int> localD) {
	const int BIG_NUMBER = 1999999999;

	vector<int> newOrder;
	int smallestNumber;
	int smallestNumberIndex;

	while (isVectorContainingOnlyBigNumbers(localD, BIG_NUMBER) == false) {

		smallestNumber = localD[0];
		smallestNumberIndex = 0;

		// find index of the smallest number in vector
		for (int i = 0; i < localD.size(); i++) {
			if (smallestNumber > localD[i]) {

				smallestNumber = localD[i];
				smallestNumberIndex = i;
			}
		}
		newOrder.push_back(smallestNumberIndex);
		localD[smallestNumberIndex] = BIG_NUMBER;
	}
	return newOrder;
}
bool isVectorContainingOnlyBigNumbers(const vector<int> VEC, const int BIG_NUMBER) {
	for (int i = 0; i < VEC.size(); i++) {
		if (VEC[i] != BIG_NUMBER) return false;
	}
	return true;
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



int calculate_Penalty(vector<int> localP, vector<int> localW,
	vector<int> localD) {


	int s = 0, c = localP[0];
	for (int i = 0; i < localP.size(); i++) {
		int penalty = (c - localD[i]) * localW[i];
		if (penalty > 0) s = s + penalty;
		int j = i + 1;
		if (j == localP.size())
		{
		}
		else {
			c = c + localP[j];
		}
	}

	return s;
}

vector<int> BruteForce(vector<int> P, int size)
{
	vector<int> newOrder;
	int l;
	for (l = 0; l < size; l++)
	{
		newOrder.push_back(l);
	}
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
		if (j > 0) { //sprawdz warunek
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