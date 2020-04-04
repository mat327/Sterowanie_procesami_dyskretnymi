#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <chrono>

using namespace std;

vector<int> R, P, Q;
string line;
int quantityOfDataLines = 0;
int quantityofElements = 0;
ifstream myfile("data.txt");


void printVector(const vector<int> VEC);
bool isVectorContainingOnlyBigNumbers(const vector<int> VEC, const int BIG_NUMBER);
vector<int> getNewOrderWithSortR(vector<int> localR);
vector<int> getReorganisedVector(const vector<int> VEC, const vector<int> NEW_ORDER);
void printVectorWithIncrementedValues(vector<int> VEC);
int calculateC_MAX(vector<int> localR, vector<int> localP, vector<int> localQ);

int main() {
    string line;
    string dataName;
    int tempR, tempP, tempQ;
    vector<int> newOrder;

    vector<int> newR;
    vector<int> newP;
    vector<int> newQ;

    vector<int> Cmax;

    if (myfile.is_open()) {

        int tab[6] = { 10, 20, 50, 100, 200, 500};
        for (int i = 0; i < 6; i++) {
            dataName = "data" + to_string(tab[i]);

            // Skip useless data
            while (getline(myfile, line) && line != dataName) {}

            // read quantity of measures
            myfile >> quantityOfDataLines >> quantityofElements;

            // read data

            for (int i = 0; i < quantityOfDataLines; i++) {
                myfile >> tempR >> tempP >> tempQ;
                R.push_back(tempR);
                P.push_back(tempP);
                Q.push_back(tempQ);
            }


            cout << "CMAX for initialorder = " << calculateC_MAX(R, P, Q) << endl;


            auto start = chrono::steady_clock::now();

            vector<int> newOrder = getNewOrderWithSortR(R);

            vector<int> newR = getReorganisedVector(R, newOrder);
            vector<int> newP = getReorganisedVector(P, newOrder);
            vector<int> newQ = getReorganisedVector(Q, newOrder);

           
            Cmax.push_back(calculateC_MAX(newR, newP, newQ));


           // cout << "New order for " + dataName + " with SortR "; printVector(newOrder);
            cout << "Calculated Cmax for " + dataName + " = " << Cmax[i] << "\n\n"; // has to be i-1 cuz we got date from 1 to 4 while Cmax start indexing from zero

            //clear vectors
            R.clear(); P.clear(); Q.clear();
            newOrder.clear(); newR.clear(); newP.clear(); newQ.clear();

        }
        myfile.close();

        cout << "Sum for every Cmax = " << Cmax[0] + Cmax[1] + Cmax[2] + Cmax[3] << "\n";

    }
    else {
        cerr << "Unable to open file. Exit program.\n";
    }
    return 0;
}




vector<int> getNewOrderWithSortR(vector<int> localR) {
    const int BIG_NUMBER = 1999999999;

    vector<int> newOrder;
    int smallestNumber;
    int smallestNumberIndex;

    while (isVectorContainingOnlyBigNumbers(localR, BIG_NUMBER) == false) {

        smallestNumber = localR[0];
        smallestNumberIndex = 0;

        // find index of the smallest number in vector
        for (int i = 0; i < localR.size(); i++) {
            if (smallestNumber > localR[i]) {

                smallestNumber = localR[i];
                smallestNumberIndex = i;
            }
        }
        newOrder.push_back(smallestNumberIndex);
        localR[smallestNumberIndex] = BIG_NUMBER; // use assignment instead of erase!! cuz with erase() you modifying initial order!!
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



int calculateC_MAX(vector<int> localR, vector<int> localP,
    vector<int> localQ) {


    int m = 0, c = 0;
    for (int i = 0; i < localR.size(); i++) {
        m = max(localR[i], m) + localP[i];
        c = max(c, m + localQ[i]);
    }

    return c;
}
