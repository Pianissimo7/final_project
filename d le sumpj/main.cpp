#include "headers/permutation.hpp"
#include "headers/cache.hpp"
#include "headers/bruteforcesolver.hpp"
#include <iostream>
#include <cmath>
#include <string.h>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>
#include <chrono>

#define GENERATE_TESTS 0
#define RANDOM_TESTS 1
#define PREMADE_TESTS 2
#define SPECIFIC_TEST 3

using namespace std;

const size_t MAX_SAMPLES = 9;
const size_t MIN_SAMPLES = 5;
const size_t MIN_P = 1;
const size_t MAX_P = 5000;
const size_t MAX_OVERHEAD_D = 50;

const string SOLUTIONS_FILE_NAME = "bruteforcesolutions.csv";
const char DELIMITER = ',';
const string EMPTY_CHARACTER = "N/A";
const double EPSILON = 0.0001;

size_t rec_counter = 0;
size_t dynamic_usage_counter = 0;

vector<element *> MakeSortedElementArray(double * p, size_t size) {
    
    vector<element *> * ElementVector = new vector<element *>();
    ElementVector->reserve(size);
    
    double SortedP[size];
    for (size_t i = 0 ; i < size ; i++) {
        SortedP[i] = p[i];
    }
    sort(SortedP, SortedP + size);

    for (size_t i = 0 ; i < size ; i++) {
        ElementVector->emplace_back(new element(SortedP[i]));
    }
    
    return *ElementVector;
}

double RoundToEpsilon(double number) {
    size_t accuracy = (size_t)floor(1/EPSILON);
    return round(number * accuracy) / accuracy;
}

permutation * getOptimalPermutation(vector<element *> OrderedElements, size_t index, double d, cache * MyCache, size_t ElementsLeftNo, size_t ElementsRightNo) {
    
    // double Optd = RoundToEpsilon(min(max(d, 0.0), MyCache->RunningSums[index]));
    double Optd = RoundToEpsilon(max(d, 0.0));
    permutation * OptimalPerm;
    element * e = OrderedElements[index];

    if (index == 0) {
        OptimalPerm = new permutation();
        OptimalPerm->AddToStart(e);
        return OptimalPerm;
    }
    if (MyCache->PermMap[index]->find(Optd) != MyCache->PermMap[index]->end()) {
        dynamic_usage_counter++;
        rec_counter++;
        return new permutation((*(MyCache->PermMap[index]))[Optd]);
    }
    rec_counter++;
    permutation * LeftOptPerm = getOptimalPermutation(OrderedElements, index - 1, Optd - e->getValue(), MyCache, ElementsLeftNo + 1, ElementsRightNo);
    LeftOptPerm->AddToStart(e);
    double CostLeft = LeftOptPerm->getCost(Optd, MyCache->RunningSums[index], ElementsLeftNo, ElementsRightNo);

    permutation * RightOptPerm = getOptimalPermutation(OrderedElements, index - 1, Optd, MyCache, ElementsLeftNo, ElementsRightNo + 1);
    RightOptPerm->AddToEnd(e);
    double CostRight = RightOptPerm->getCost(Optd, MyCache->RunningSums[index], ElementsLeftNo, ElementsRightNo);

    if (CostLeft < CostRight) { 
        OptimalPerm = LeftOptPerm; 
        delete(RightOptPerm);
    }
    else {
        OptimalPerm = RightOptPerm; 
        delete(LeftOptPerm);
    }
    (*(MyCache->PermMap[index]))[Optd] = new permutation(OptimalPerm);
    return OptimalPerm;
}

permutation * getOptimalPermutation(double * p, size_t size, double d) {
    
    vector<element *> SortedElementVector = MakeSortedElementArray(p, size);

    cache * MyCache = new cache(SortedElementVector);

    permutation * OptimalPerm = new permutation(getOptimalPermutation(SortedElementVector, SortedElementVector.size() - 1, d, MyCache, 0, 0));
    delete(MyCache);
    return OptimalPerm;
}

void PrepareOutputFile(ofstream& file) {
    file << "Cycle" << DELIMITER;
    file << "Cost" << DELIMITER;
    file << "deadline" << DELIMITER;
    for (size_t i = 1 ; i <= MAX_SAMPLES ; i++) {
        file << "Element: " << i << DELIMITER;
    }
    file << "\n";
    file.flush();
}
void WriteOutputToFile(ofstream& file, int cycle_number, const vector<double> &perm, double d, double cost) {
    file << to_string(cycle_number) << DELIMITER;
    file << to_string(cost) << DELIMITER;
    file << to_string(d) << DELIMITER;
    for (size_t i = 0 ; i < MAX_SAMPLES ; i++) {
        if (perm.size() > i) {
            file << to_string(perm[i]) << DELIMITER;

        }
        else {
            file << EMPTY_CHARACTER << DELIMITER;
        }
    }
    file << "\n";
    file.flush();
}
void TestDynamicPrograming() {
    ifstream inputFile(SOLUTIONS_FILE_NAME);
    if (!inputFile.is_open()) {
        cerr << "Could not open the file." << endl;
        return;
    }
    string line;
    if (!getline(inputFile, line)) {
        cerr << "file is empty." << endl;
    }
    while (getline(inputFile, line)) {
        istringstream iss(line);
        
        int index;
        double cost, deadline;
        size_t size = 0;
        vector<double> ProcessingTimesVector;
        
        string temp;
        getline(iss, temp, ',');
        index = stoi(temp);

        getline(iss, temp, ',');
        cost = stod(temp);

        getline(iss, temp, ',');
        deadline = stod(temp);

        while (std::getline(iss, temp, ',')) {
            if (temp == EMPTY_CHARACTER || temp == "") {
                break;
            }
            ProcessingTimesVector.push_back(std::stod(temp));
            size++;
        }
        
        double * p = (double *)malloc(sizeof(double) * ProcessingTimesVector.size());
        
        for (size_t i = 0 ; i < ProcessingTimesVector.size() ; i++) {
            p[i] = ProcessingTimesVector[i];
        }

        rec_counter = 0;
        dynamic_usage_counter = 0;
        permutation * OptimalPerm = getOptimalPermutation(p, size, deadline);
        
        double DynamicCost = OptimalPerm->getCost(deadline);

        bool rec_fail = rec_counter >  2 * size * ceil(deadline);

        if (fabs(DynamicCost - cost) > EPSILON || rec_fail) {
            cerr << "failed on number: " << to_string(index) << endl;
            cerr << "DynamicCost: " << to_string(DynamicCost) << endl;
            cerr << "RealCost: " << to_string(cost) << endl;

            cerr << "the dynamic programing solution: ";
            OptimalPerm->print();
            cerr << "d: " << to_string(deadline) << endl;
            cerr << "dynamic usage: " << to_string(dynamic_usage_counter) << endl;
            if (rec_fail) {
                cerr << "rec_counter: " << to_string(rec_counter) << "/" << to_string(2 * size * ceil(deadline)) << endl;
            }
            return;
        }

        std::free(p);
    }
    std::cout << "SUCCESSFULLY PASSED ALL TESTS";
}
int main() {
    size_t mode = GENERATE_TESTS;
    if (mode == GENERATE_TESTS || mode == RANDOM_TESTS) {
        ofstream file(SOLUTIONS_FILE_NAME, ios::trunc);
        if (mode == GENERATE_TESTS) {
            if (!file.is_open()) {
                return 1;
            }
            PrepareOutputFile(file);
        }
        
        random_device rd;
        mt19937 re(rd());

        double AverageRunningTimeDP = 0;
        double AverageRunningTimeBF = 0;
        double WorstRunningTimeDP = 0;
        double WorstRunningTimeBF = 0;
        size_t TestAmount = 1000;

        for (size_t j = 0 ; j < TestAmount ; j++) {
            
            // generate the test
            size_t size = MIN_SAMPLES + rand() % ((MAX_SAMPLES + 1) - MIN_SAMPLES);
            
            uniform_real_distribution<double> pUnif(MIN_P,MAX_P + 1);
            
            double * p = (double *)malloc(sizeof(double) * size);
            double d = 0;
            double sum = 0;
            for (size_t i = 0 ; i < size ; i++) {
                p[i] = pUnif(re);
                sum += p[i];
            }
            // uniform_real_distribution<double> dUnif((size_t)ceil(sum) + MAX_OVERHEAD_D + 1);
            uniform_real_distribution<double> dUnif((size_t)(ceil(sum)/3), (size_t)(ceil(sum)/2));
            d = dUnif(re);
            
            // run the dynamic programing algorithm on the test
            
            auto DPStart = std::chrono::high_resolution_clock::now();
            
            permutation * OptimalPerm = getOptimalPermutation(p, size, d);

            auto DPEnd = std::chrono::high_resolution_clock::now();
            
            // print dynamic programing solution
            std::cout << "tasks: " << endl;
            for (size_t i = 0 ; i < size ; i++) {
                std::cout << to_string(p[i]) << ", ";
            }
            std::cout << endl;
            std::cout << "d: " << to_string(d) << endl;

            std::cout << "dynamic: " << endl;
            OptimalPerm->print();
            double DynamicCost = OptimalPerm->getCost(d);
            std::cout << "dynamic cost: " << DynamicCost << endl;

            // run the brute force algorithm on the test
            auto BFStart = std::chrono::high_resolution_clock::now();
            
            bruteforcesolver *bfs = new bruteforcesolver(p, size, d);
            vector<double> opt = bfs->getSolution();
            
            auto BFEnd = std::chrono::high_resolution_clock::now();
            
            // print brute force solution
            std::cout << "brute force: " << endl;
            for (size_t i = 0 ; i < size ; i++) {
                std::cout << to_string(opt[i]) + ", ";
            }
            std::cout << endl;
            double BruteForceCost = bfs->CalculateMinCost(opt);
            std::cout << "Brute cost: " << BruteForceCost << endl;

            if (mode == GENERATE_TESTS) {
                WriteOutputToFile(file, j, opt, d, BruteForceCost);
            }

            delete(bfs);
            std::free(p);
            
            // print the time it took each of the algorithms to finish computing
            auto DPDuration = std::chrono::duration_cast<std::chrono::milliseconds>(DPEnd - DPStart);
            auto BFDuration = std::chrono::duration_cast<std::chrono::milliseconds>(BFEnd - BFStart);
            std::cout << "Dynamic programing duration: " << DPDuration.count() << endl;
            std::cout << "brute force duration: " << BFDuration.count() << endl;
            AverageRunningTimeDP += (double)DPDuration.count() / TestAmount;
            AverageRunningTimeBF += (double)BFDuration.count() / TestAmount;
            if (DPDuration.count() > WorstRunningTimeDP) {
                WorstRunningTimeDP = DPDuration.count();
            }
            if (BFDuration.count() > WorstRunningTimeBF) {
                WorstRunningTimeBF = BFDuration.count();
            }
            // compare the results of the DP algorithm with the brute force algorithm
            double diff = fabs(BruteForceCost - DynamicCost);
            if (diff > EPSILON) {
                std::cout << "FAILED" << endl;
                return 0;
            } 
        }
        std::cout << "---------------STATS-------------" << endl;
        std::cout << "Average running time for DP: " << AverageRunningTimeDP << endl;
        std::cout << "Average running time for BF: " << AverageRunningTimeBF << endl;
        std::cout << "Worst running time for DP: " << WorstRunningTimeDP << endl;
        std::cout << "Worst running time for BF: " << WorstRunningTimeBF << endl;
        std::cout << "---------------------------------" << endl;
        if (mode == GENERATE_TESTS) {
            file.close();
        }
    }
    else if (mode == PREMADE_TESTS) {
        TestDynamicPrograming();
    }
    else if (mode == SPECIFIC_TEST) {        
        // example where the number of left elements is greated than the number of right elements
        // double p[] = { 39.702425, 13.911250, 10.728948, 46.716133, 4.668681, 13.453588, 47.603802};
        // double d = 87.220406;
        // double p[] = {35, 10, 3, 14, 20, 40};
        // double d = 19;
        
        // double p[] = {24.225068, 37.990771, 23.265589, 24.427745, 15.179924, 47.028345, 20.448316, 40.782033};
        // double d = 127.066072;
        // got to make sure this does not fail
        // double p[] = {24.998633, 41.827879, 44.255552, 3.507722, 29.047814, 33.343240, 23.415793};
        // double d = 79.095620;
        // double p[] = { 20, 4,  3, 21, 7,  17};
        // double d = 15;
        
        // double p[] = {5, 4, 3, 3, 3, 3, 3, 3, 3,1};
        // double d = 16;
        // double p[] = {1, 2, 3, 4, 5};
        // double d = 6;
        
        // 2 
        // double p[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
        // double d = 10922;
        // double p[] = {1, 4, 27, 256, 3125, 46656, 823543, 16777216};
        // double d = 0.5;
        // extra long permutation of prime numbers
        // double p[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541};
        // size_t size = sizeof(p)/sizeof(p[0]);
        // double sum = 0;
        // for (size_t i = 0 ; i < size ; i++) {
        //     sum += p[i];
        // }
        // double d = sum / 3;
        // example for where the Brute Force finishes before the dynamic programming
        // double p[] = {36.872481, 45.272827, 17.817891, 34.571412, 31.276184, 4.332805};
        // double d = 52.218603;
        
        double p[] = {1, 3, 4, 7};
        double d = 6;
        
        size_t size = sizeof(p)/sizeof(p[0]);

        permutation * OptimalPerm = getOptimalPermutation(p, size, d);
        std::cout << "tasks: " << endl;
        for (size_t i = 0 ; i < size ; i++) {
            std::cout << to_string(p[i]) << ", ";
        }
        std::cout << endl;
        std::cout << "d: " << to_string(d) << endl;

        std::cout << "dynamic: " << endl;
        OptimalPerm->print();
        double DynamicCost = OptimalPerm->getCost(d);
        std::cout << "dynamic cost: " << DynamicCost << endl;
        std::cout << "dynamic usage: " << to_string(dynamic_usage_counter) << endl;
        std::cout << "rec_counter: " << to_string(rec_counter) << "/" << to_string(2 * size * ceil(d)) << endl;
        bruteforcesolver *bfs = new bruteforcesolver(p, size, d);
        vector<double> opt = bfs->getSolution();
        
        std::cout << "brute force: " << endl;
        for (size_t i = 0 ; i < size ; i++) {
            std::cout << to_string(opt[i]) + ", ";
        }
        std::cout << endl;
        double BruteForceCost = bfs->CalculateMinCost(opt);
        std::cout << "Brute cost: " << BruteForceCost << endl;
        delete(bfs);
        double diff = fabs(BruteForceCost - DynamicCost);
        if (diff > EPSILON) {
            std::cout << "FAILED" << endl;
            return 1;
        } 
    }
    return 0;
}
