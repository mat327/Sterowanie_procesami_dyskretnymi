#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <chrono>

using namespace std;

class Job
{
public:
    int R;
    int P;
    int Q;
    int initialP;

    int initialIndex;

    Job job() {};

    void showJob() {
        std::cout << "R: " << R << " P: " << P << " Q: " << Q << " init index: " << initialIndex + 1 << "\n";
    }
};

int calculateC_MAX(std::vector<Job>& jobs) {
    int m = 0, c = 0;
    for (int i = 0; i < jobs.size(); i++) {
        m = std::max(jobs[i].R, m) + jobs[i].P;
        c = std::max(c, m + jobs[i].Q);
    }
    return c;
}


/*
 Funkcja znajduje indexy dostępnych zadań w danym przedziale czasu

 do tej fuunkcji trzeba przekazać vector prac z usunietymi pracami które zostały wykonane
*/

std::vector<int> getIndiciesAvailableJobs(std::vector<Job>& jobs, int elapsedTime) {
    std::vector<int> result;
    for (int i = 0; i < jobs.size(); i++) {
        if (jobs[i].R <= elapsedTime) {
            result.push_back(i);
        }
    }

    return result;
}

// Funkcja dodaje zadanie o największym Q do vectora result
// zwraca kolejność uszeregowania
std::vector<Job>& addNewJobWithBiggestQ(std::vector<Job>& jobs, std::vector<int>& availbleTasksIndices,
    std::vector<Job>& resultJobs, int& elapsedTime) {

    int biggestQIndex = availbleTasksIndices[0];
    int biggestQ = jobs[biggestQIndex].Q;

    for (int index : availbleTasksIndices) {
        if (biggestQ < jobs[index].Q) {
            biggestQ = jobs[index].Q;
            biggestQIndex = index;

        }
        else if (biggestQ == jobs[index].Q) { // jeżeli takie same Q to bierzemy o mniejszym indexie początkowym

            if (jobs[biggestQIndex].initialIndex > jobs[index].initialIndex) {
                biggestQ = jobs[index].Q;
                biggestQIndex = index;
            }
        }
    }



    resultJobs.push_back(jobs[biggestQIndex]);
    elapsedTime += jobs[biggestQIndex].P;


    jobs[biggestQIndex].R = std::numeric_limits<int>::max(); // podmieniamy wartość R zadania które własnie dodaliśmy do kolekcji result.

    return resultJobs;
}

int main(int argc, char** argv) {

    std::vector<Job> jobs, result;
    Job temporaryJob;



    std::ifstream myfile("data.txt");
    int quantityOfData = 0;
    int quantityOfElements = 0;

    std::string line;


    if (!myfile.is_open()) throw std::runtime_error("Could not open file");

    std::string currentDataSet;

    int tab[6] = { 10, 20, 50, 100, 200, 500 };

    for (int dataSetIndex = 0; dataSetIndex < 6; dataSetIndex++) {

        currentDataSet = "data" + std::to_string(tab[dataSetIndex]);

        while (getline(myfile, line) && (line != currentDataSet)); // skip useless data
        myfile >> quantityOfData >> quantityOfElements;
        try
        {
            if (quantityOfData == 0) throw std::runtime_error(std::string("Could not read data from file. (probably inappropriate end of line character in variable currentDataSet)\n") +
                "Try \\n, \\n\\r, \\r, or none");
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << "\n";
            exit(-1);
        }

        // read data

        for (int i = 0; i < quantityOfData; i++) {
            myfile >> temporaryJob.R >> temporaryJob.P >> temporaryJob.Q;
            temporaryJob.initialIndex = i;

            jobs.push_back(temporaryJob);
        }

        // Posortujmye vector prac po R rosnąco
        auto start = chrono::steady_clock::now();
        std::sort(jobs.begin(), jobs.end(), [](const Job& first, const Job& second) { return first.R < second.R; });


        // Pierwsze dostępne zadanie będzie zadaniem o najmniejszym R
        // Zadanie to będzie dostępne w momencie elapsedTime równym jego R
        Job jobWithSmallestR = jobs[0];
        int elapsedTime = jobWithSmallestR.R;

        int initialQuantitiOfJobs = jobs.size();
        std::vector<int> indicesAvailableJobs;

        // Shrange zwykłe

        // wykonujemy dopóki w kolekcji result jest mniej prac niż prac wczytanych

        while (result.size() != initialQuantitiOfJobs) {

            // Z puli zadań wyszukujemy te indexy zadań których R jest mniejsze bądź równe elapsedTime
            // oraz nie bierzemy pod uwage zadań uprzednio dodanych do kolekcji result

            indicesAvailableJobs = getIndiciesAvailableJobs(jobs, elapsedTime);

            // W przypadku gdy obecnie nie ma żdanych dostępnych zadań w czasie elapsedTime
            // To w kolekcji z zadaniami wyszukujemy pierwsze zadanie którego R jest większe od elapsedTime oraz różne od MAX_INT (ponieważ po dodaniu zadania do result, jego wartość R podmieniamy na MAX_INT)

            if (indicesAvailableJobs.size() == 0) {
                // W tym przypadku znaleźć pierwsze większe R niz elapsedTime z vectora jobs i przypisac je do elapsed time
                for (int i = 0; i < jobs.size(); i++) {
                    if (jobs[i].R > elapsedTime && (jobs[i].R != std::numeric_limits<int>::max())) {
                        elapsedTime = jobs[i].R;
                        // kiedy znaleźliśmy następne elapsedTime dla którego będą jakieś zadania, to ponownie wyszukujemy dostępne zadania
                        indicesAvailableJobs = getIndiciesAvailableJobs(jobs, elapsedTime);
                        break;
                    }
                }

            }

            // z puli dostępnych zadań, dodaj zadanie do kolekcji result na podstawie parametru Q
            // w tej funkcji dzieje sie wiele rzeczy.
            // dodajemy zadanie o najwiekszym Q do kolekcji result
            // zwiększamy elapsed time o wartość P zadania które dodaliśmy do kolekcji result
            // podmieniamy wartość R zadania na MAX_INT aby pomijać to zadanie w kolejnych iteracjach.
            result = addNewJobWithBiggestQ(jobs, indicesAvailableJobs, result, elapsedTime);

        }


        int C_max = calculateC_MAX(result);

        auto end = chrono::steady_clock::now();

        cout << "Elapsed time in microseconds : "
            << chrono::duration_cast<chrono::microseconds>(end - start).count()
            << " µs" << endl;

        std::cout << "\n Current data set:" << dataSetIndex << " C_max = " << C_max << "\n";
        std::cout << "\n";

        result.clear();
        jobs.clear();
    }


    return 0;
}
