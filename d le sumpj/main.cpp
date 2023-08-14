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

using namespace std;

const size_t MAX_SAMPLES = 10;
const size_t MIN_SAMPLES = 6;
const size_t MIN_P = 1;
const size_t MAX_P = 50;
const size_t MAX_OVERHEAD_D = 50;
const size_t RANDOM_TESTS = 0;
const size_t PREMADE_TESTS = 1;
const size_t SPECIFIC_TEST = 2;

const string SOLUTIONS_FILE_NAME = "bruteforcesolutions.csv";
const char DELIMITER = ',';
const string EMPTY_CHARACTER = "N/A";
const double EPSILON = 0.0001;



size_t rec_counter = 0;

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

permutation * getOptimalPermutation(vector<element *> OrderedElements, size_t index, double d, cache * MyCache, size_t ElementsLeftNo, size_t ElementsRightNo) {
    double Optd = min(max(d, 0.0), MyCache->RunningSums[index]);
    if ((index > 0) && MyCache->PermMap[index]->find(Optd) != MyCache->PermMap[index]->end()) {
        // cout << "e: " << to_string(OrderedElements[index]->getValue()) << " d: " << to_string(d) << "found permutation in map: ";
        // (*(MyCache->PermMap[index]))[Optd]->print();
        return new permutation((*(MyCache->PermMap[index]))[Optd]);
        
    }
    
    rec_counter++;
    
    permutation * OptimalPerm;
    element * e = OrderedElements[index];

    if (Optd == 0.0) {
        OptimalPerm = new permutation();
        for (size_t i = 0 ; i <= index ; i++) {
            OptimalPerm->AddToEnd(OrderedElements[i]);
            ElementsRightNo++;
        }
        return OptimalPerm;
    }    
    if (index == 0) {
        OptimalPerm = new permutation();
        OptimalPerm->AddToStart(e);
        return OptimalPerm;
    }
    
    permutation * LeftOptPerm = getOptimalPermutation(OrderedElements, index - 1, d - e->getValue(), MyCache, ElementsLeftNo + 1, ElementsRightNo);
    LeftOptPerm->AddToStart(e);
    double CostLeft = LeftOptPerm->getCost(d, MyCache->RunningSums[index], ElementsLeftNo, ElementsRightNo);

    permutation * RightOptPerm = getOptimalPermutation(OrderedElements, index - 1, d, MyCache, ElementsLeftNo, ElementsRightNo + 1);
    RightOptPerm->AddToEnd(e);
    double CostRight = RightOptPerm->getCost(d, MyCache->RunningSums[index], ElementsLeftNo, ElementsRightNo);

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
    
    double AlternatingLeftSideSum = 0.0;
    for (size_t i = size - 1 ; (i + 1 > 0 && i + 2 > 0) ; i -= 2) {
        AlternatingLeftSideSum += SortedElementVector[i]->getValue();
    }
    if (d >= AlternatingLeftSideSum) {
        permutation * DgeSumPj = new permutation(MyCache->DgeSumPj);
        delete(MyCache);
        return DgeSumPj;
    }
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
}
void TestDynamicPrograming() {
    ifstream inputFile(SOLUTIONS_FILE_NAME); // Replace "data.csv" with your CSV file name
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

        permutation * OptimalPerm = getOptimalPermutation(p, size, deadline);
        
        double DynamicCost = OptimalPerm->getCost(deadline);
        
        // cout << "size: " << to_string(size) << endl;
        // std::cout << "Int value: " << index << std::endl;
        // std::cout << "Double value 1: " << cost << std::endl;
        // std::cout << "Double value 2: " << deadline << std::endl;
        // std::cout << "Double array: ";

        // for (double val : ProcessingTimesVector) {
        //     std::cout << val << " ";
        // }
        // std::cout << std::endl;


        if (fabs(DynamicCost - cost) > EPSILON) {
            cerr << "failed on number: " << to_string(index) << endl;
            cerr << "DynamicCost: " << to_string(DynamicCost) << endl;
            cerr << "RealCost: " << to_string(cost) << endl;

            cerr << "the dynamic programing solution: ";
            OptimalPerm->print();
            return;
        }

        std::free(p);
    }
    cout << "SUCCESSFULLY PASSED ALL TESTS";
}
int main() {
    size_t mode = SPECIFIC_TEST;
    if (mode == RANDOM_TESTS) {
        ofstream file(SOLUTIONS_FILE_NAME, ios::trunc);
        if (!file.is_open()) {
            return 1;
        }
        PrepareOutputFile(file);
        random_device rd;
        mt19937 re(rd());
        for (size_t j = 0 ; j < 1000 ; j++) {
            size_t size = MIN_SAMPLES + rand() % ((MAX_SAMPLES + 1) - MIN_SAMPLES);
            
            uniform_real_distribution<double> pUnif(MIN_P,MAX_P + 1);
            
            double * p = (double *)malloc(sizeof(double) * size);
            double d = 0;
            double sum = 0;
            for (size_t i = 0 ; i < size ; i++) {
                p[i] = pUnif(re);
                sum += p[i];
            }
            uniform_real_distribution<double> dUnif((size_t)ceil(sum) + MAX_OVERHEAD_D + 1);
            d = dUnif(re);
            
            // permutation * OptimalPerm = getOptimalPermutation(p, size, d);
            // std::cout << "tasks: " << endl;
            // for (size_t i = 0 ; i < size ; i++) {
            //     std::cout << to_string(p[i]) << ", ";
            // }
            // std::cout << endl;
            // std::cout << "d: " << to_string(d) << endl;

            // std::cout << "dynamic: " << endl;
            // OptimalPerm->print();
            // double DynamicCost = OptimalPerm->getCost(d);
            // std::cout << "dynamic cost: " << DynamicCost << endl;
            
            bruteforcesolver *bfs = new bruteforcesolver(p, size, d);
            vector<double> opt = bfs->getSolution();
            
            std::cout << "brute force: " << endl;
            for (size_t i = 0 ; i < size ; i++) {
                std::cout << to_string(opt[i]) + ", ";
            }
            std::cout << endl;
            double BruteForceCost = bfs->CalculateMinCost(opt);
            std::cout << "Brute cost: " << BruteForceCost << endl;

            WriteOutputToFile(file, j, opt, d, BruteForceCost);

            delete(bfs);
            std::free(p);
            // double diff = fabs(BruteForceCost - DynamicCost);
            // double epsilon = 0.00001;
            // if (diff > epsilon) {
            //     std::cout << "FAILED" << endl;
            //     return 0;
            // } 
        }
        file.close();
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
        
        double p[] = {5, 4, 3, 3, 3, 3, 3, 3, 3};
        double d = 10;

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
        double epsilon = 0.00001;
        if (diff > epsilon) {
            std::cout << "FAILED" << endl;
            return 0;
        } 
    }
    return 0;
}
