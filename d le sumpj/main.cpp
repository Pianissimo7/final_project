#include "headers/permutation.hpp"
#include "headers/cache.hpp"
#include "headers/bruteforcesolver.hpp"
#include <iostream>
#include <cmath>
#include <string.h>
#include <unordered_map>
#include <memory>
#include <algorithm>

using namespace std;
size_t rec_counter = 0;
// takes an array of doubles and converts it to an array of elements
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

permutation * getOptimalPermutation(vector<element *> OrderedElements, size_t index, double d, cache * MyCache) {
    // use a pereviously calculated sub optimal permutation to calculate the bigger sub optimal permutation
    double Optd = min(max(d, 0.0), MyCache->OptDDgeSumPjs[index]);
    cout << "searching for permutation, index: " << to_string(index) << " d: " << to_string(d) << "Optd " << to_string(Optd) << endl;
    if ((index > 0) && MyCache->PermMap[index]->find(Optd) != MyCache->PermMap[index]->end()) {
        cout << "permutation found in cache!!" << endl ;
        return new permutation((*(MyCache->PermMap[index]))[Optd]);
    }
    
    rec_counter++;
    
    // create empty perm
    permutation * OptimalPerm;
    // get the element that we are working on
    element * e = OrderedElements[index];

    if (Optd == 0.0) {
        OptimalPerm = new permutation();
        for (size_t i = 0 ; i <= index ; i++) {
            OptimalPerm->AddToEnd(OrderedElements[i]);
        }
        OptimalPerm->print();
        cout << "adding permutation, index: " << to_string(index) << " d: " << to_string(d) << "Optd " << to_string(Optd) << endl;
       
        (*MyCache->PermMap[index])[Optd] = new permutation(OptimalPerm);
        
        MyCache->printMap();
        return OptimalPerm;
    }    
    if (index == 0) {
        OptimalPerm = new permutation();
        OptimalPerm->AddToStart(e);
        return OptimalPerm;
    }
    
    permutation * LeftOptPerm = getOptimalPermutation(OrderedElements, index - 1, d - e->getValue(), MyCache);
    LeftOptPerm->AddToStart(e);
    double LeftOffset;
    double CostLeft = LeftOptPerm->getCost(d, &LeftOffset);

    permutation * RightOptPerm = getOptimalPermutation(OrderedElements, index - 1, d, MyCache);
    RightOptPerm->AddToEnd(e);
    double RightOffset;
    double CostRight = RightOptPerm->getCost(d, &RightOffset);
    // cout << "L: ";
    // LeftOptPerm->print();
    // cout << "R: ";
    // RightOptPerm->print();
    if (CostLeft < CostRight || (LeftOffset > RightOffset && CostLeft == CostRight)) {
        OptimalPerm = LeftOptPerm;   
        delete(RightOptPerm);
    }
    else if (CostLeft > CostRight || (LeftOffset <= RightOffset && CostLeft == CostRight)) {
        OptimalPerm = RightOptPerm; 
        delete(LeftOptPerm);
    }
    cout << "e:" << to_string(e->getValue()) << " d: " << to_string(d) << " left: " << to_string(CostLeft) << " right: " << to_string(CostRight) << " total cost: " << to_string(min(CostLeft, CostRight)) << " curr perm: ";
    OptimalPerm->print();
    (*(MyCache->PermMap[index]))[Optd] = new permutation(OptimalPerm);
    cout << "adding permutation, index: " << to_string(index) << " d: " << to_string(d) << "Optd " << to_string(Optd) << endl;
    MyCache->printMap();
    return OptimalPerm;
}

permutation * getOptimalPermutation(double * p, size_t size, double d) {
    
    vector<element *> SortedElementVector = MakeSortedElementArray(p, size);

    cache * MyCache = new cache(SortedElementVector);
    
    if (d >= MyCache->OptDDgeSumPjs[SortedElementVector.size() - 1]) {
        permutation * DgeSumPj = new permutation(MyCache->DgeSumPj);
        delete(MyCache);
        return DgeSumPj;
    }    
    // now we can assume that d < SumPj
    MyCache->printMap();
    permutation * OptimalPerm = new permutation(getOptimalPermutation(SortedElementVector, SortedElementVector.size() - 1, d, MyCache));
    delete(MyCache);
    return OptimalPerm;
}

int main() {
    //processing times array
    
    double p[] = { 5,7, 9, 13, 23, 11, 4, 34, 0,0.1};
    double d = 50;

    // double p[] = { 5,7, 9, 13, 23, 11, 4};
    // double d = 28;

    // double p[] = { 2, 5,7,4, 11};
    // double p[] = {7, 9, 23, 4};
    //double p[] = { 2, 3,2.1};
    // double p[] = {1, 2, 3, 7};
    // double d = 3;
    // double p[] = {1, 2, 3, 7, 9};
    // double p[] = {2, 3, 6, 13};
    // double p[] = {1, 2, 3};
    // size of the array
    size_t size = (sizeof(p) / sizeof(double));
    // deadline
    //double d = 50;

    
    permutation * OptimalPerm = getOptimalPermutation(p, size, d);
    cout << "dynamic programing optimal perm: ";
    OptimalPerm->print();
    cout << "dynamic programing min cost: " << to_string(OptimalPerm->getCost(d)) << endl;
    cout << "reccursion count: " << to_string(rec_counter) << endl;

    // element * e = new element(32.0);
    // OptimalPerm->AddToEnd(e);
    // cout << "add Right Cost" << to_string(OptimalPerm->getCost(50.0)) << endl;
    // OptimalPerm->clear();
    
    bruteforcesolver *bfs = new bruteforcesolver(p, size, d);
    vector<double> opt = bfs->getSolution();
    cout << "brute force optimal perm: ";
    for (size_t i = 0 ; i < size ; i++) {
        cout << to_string(opt[i]) << ", ";
    }
    cout << endl;
    cout << "brute force min cost: " << to_string(bfs->CalculateMinCost(opt)) << endl;
    delete(bfs);
    return 0;
}
