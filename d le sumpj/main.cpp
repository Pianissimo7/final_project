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
        ElementVector->emplace_back(new element(SortedP[i], i));
    }
    
    return *ElementVector;
}

std::shared_ptr<permutation> getOptimalPermutation(vector<element *> OrderedElements, size_t index, double d, cache * MyCache) {
    // use a pereviously calculated sub optimal permutation to calculate the bigger sub optimal permutation
    double Optd = min(max(d, 0.0), MyCache->OptDDgeSumPjs[index]);
    if ((index > 0) && MyCache->PermMap[index - 1]->find(Optd) != MyCache->PermMap[index - 1]->end()) {
        return (*(MyCache->PermMap[index - 1]))[Optd];
    }
    
    rec_counter++;
    
    shared_ptr<permutation> OptimalPerm;
    element * e = OrderedElements[index];

    if (Optd == 0.0) {
        OptimalPerm = make_shared<permutation>();
        for (size_t i = 0 ; i <= index ; i++) {
            OptimalPerm->AddToEnd(OrderedElements[i]);
        }
        (*MyCache->PermMap[index])[0.0] = OptimalPerm;
        return OptimalPerm;
    }    
    if (index == 0) {
        //create empty perm
        OptimalPerm = make_shared<permutation>();
        OptimalPerm->AddToStart(e);
        return OptimalPerm;
    }
    
    shared_ptr<permutation> LeftOptPerm = getOptimalPermutation(OrderedElements, index - 1, d - e->getValue(), MyCache);
    LeftOptPerm->AddToStart(e);
    double CostLeft = LeftOptPerm->getCost(d);

    shared_ptr<permutation> RightOptPerm = getOptimalPermutation(OrderedElements, index - 1, d, MyCache);
    RightOptPerm->AddToEnd(e);
    double CostRight = RightOptPerm->getCost(d);

    OptimalPerm = (CostLeft >= CostRight) ? RightOptPerm : LeftOptPerm;
    // cout << "L perm: ";
    // LeftOptPerm->print();
    // cout << "R perm: ";
    // RightOptPerm->print();
    // cout << "e:" << to_string(e->getValue()) << " d: " << to_string(d) << " left: " << to_string(CostLeft) << " right: " << to_string(CostRight) << " total cost: " << to_string(min(CostLeft, CostRight)) << " curr perm: ";
    // OptimalPerm->print();
    (*MyCache->PermMap[index])[Optd] = OptimalPerm;
    
    return OptimalPerm;
}

permutation * getOptimalPermutation(double * p, size_t size, double d) {
    
    vector<element *> SortedElementVector = MakeSortedElementArray(p, size);

    cache * MyCache = new cache(SortedElementVector);
    
    double OptDDgeSumPj = MyCache->OptDDgeSumPjs[SortedElementVector.size() - 1];

    if (d >= OptDDgeSumPj) {
        permutation * DgeSumPj = new permutation(MyCache->DgeSumPj);
        delete(MyCache);
        return DgeSumPj;
    }    
    // now we can assume that d < SumPj
    permutation * OptimalPerm = new permutation(getOptimalPermutation(SortedElementVector, SortedElementVector.size() - 1, d, MyCache).get());
    delete(MyCache);
    return OptimalPerm;
}

int main() {
    // processing times array
    double p[] = {5, 7, 9, 13, 23, 11};
    // double p[] = {1, 2, 3, 7};
    // double p[] = {1, 2, 3, 7, 9};
    // double p[] = {2, 3, 6, 13};
    // double p[] = {1, 2, 3};
    // size of the array
    size_t size = (sizeof(p) / sizeof(double));
    // deadline
    size_t d = 20;
    
    permutation * OptimalPerm = getOptimalPermutation(p, size, d);
    cout << "dynamic programing optimal perm: ";
    OptimalPerm->print();
    cout << "dynamic programing min cost: " << to_string(OptimalPerm->getCost(d)) << endl;
    cout << "reccursion count: " << to_string(rec_counter) << endl;
    OptimalPerm->clear();
    
    bruteforcesolver bfs(p, size, d);
    vector<double> opt = bfs.getSolution();
    cout << "brute force optimal perm: ";
    for (size_t i = 0 ; i < size ; i++) {
        cout << to_string(opt[i]) << ", ";
    }
    cout << endl;
    cout << "brute force min cost: " << to_string(bfs.CalculateMinCost(opt)) << endl;
    delete(&bfs);
    return 0;
}
