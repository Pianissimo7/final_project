#include "headers/permutation.hpp"
#include "headers/cache.hpp"
#include <iostream>
#include <cmath>
#include <string.h>
#include <map>
#include <memory>
#include <algorithm>

using namespace std;
size_t rec_counter = 0;
// takes an array of doubles and converts it to an array of elements
element * MakeElementArray(double *p, size_t size) {
    element * elements = (element *) malloc(sizeof(element) * size);
    for (size_t i = 0 ; i < size ; i++) {
        element e(p[i], i);
        memcpy(&elements[i], &e, sizeof(element));
    }
    return elements;
}

std::shared_ptr<permutation> getOptimalPermutation(vector<element *> OrderedElements, size_t index, double d, cache * MyCache) {
    // use a pereviously calculated sub optimal permutation to calculate the bigger sub optimal permutation
    if (MyCache->PermMap[index]->find(max(d, 0.0)) != MyCache->PermMap[index]->end()) {
        return (*(MyCache->PermMap[index]))[max(d, 0.0)];
    }

    rec_counter++;
    
    shared_ptr<permutation> OptimalPerm;
    element * e = OrderedElements[index];

    if (max(d, 0.0) == 0.0) {
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
    
    double CostLeft = 0;
    CostLeft += fabs(e->getValue() - d);
    shared_ptr<permutation> LeftOptPerm = getOptimalPermutation(OrderedElements, index - 1, d - e->getValue(), MyCache);
    CostLeft += LeftOptPerm->getCost(d - e->getValue());

    double CostRight = 0;
    double RunningSum = 0;
    for (size_t i = 0 ; i < index ; i++) {
        RunningSum =+ OrderedElements[i]->getValue();
        CostRight += fabs(RunningSum - d);
    }
    shared_ptr<permutation> RightOptPerm = getOptimalPermutation(OrderedElements, index - 1, d, MyCache);
    CostRight += RightOptPerm->getCost(d);
    
    if (CostLeft >= CostRight) {
        OptimalPerm = RightOptPerm;    
        OptimalPerm->AddToEnd(e);
    }
    else {
        OptimalPerm = LeftOptPerm;
        OptimalPerm->AddToStart(e);
    }
    // add the optimal permutation for this scenario to the cache
    (*MyCache->PermMap[index])[d] = OptimalPerm;
    
    return OptimalPerm;
}

permutation * getOptimalPermutation(vector<element *> OrderedElements, double d) {
    
    cache * MyCache = new cache(OrderedElements);
    
    double OptDDgeSumPj = MyCache->OptDDgeSumPjs[OrderedElements.size() - 1];

    if (d >= OptDDgeSumPj) {
        permutation * DgeSumPj = new permutation(MyCache->DgeSumPj);
        delete(MyCache);
        return DgeSumPj;
    }    
    // now we can assume that d < SumPj
    permutation * OptimalPerm = getOptimalPermutation(OrderedElements, OrderedElements.size() - 1, d, MyCache).get();
    delete(MyCache);
    return OptimalPerm;
}

int main() {
    // pj
    double p[] = {1, 2, 3, 7, 9, 13, 16, 23, 30, 34};
    // double p[] = {1, 2, 3, 7, 9, 13};
    // size of the array
    size_t size = (size_t)(sizeof(p) / sizeof(double));
    // deadline
    size_t d = 50;
    
    element * arr = MakeElementArray(p, size);
    vector<element *> OrderedElements;
    for (size_t i = 0 ; i < size ; i++) {
        OrderedElements.push_back(&(arr[i]));
    }
    for (auto element : OrderedElements) {
        cout << element->getValue() << " ";    
    }
    cout << endl;
    cout << endl;
    permutation * OptimalPerm = getOptimalPermutation(OrderedElements, d);
    cout << "optimal perm: ";
    OptimalPerm->print();
    cout << "optimal perm cost: " << to_string(OptimalPerm->getCost(d)) << endl;
    cout << "reccursion count: " << to_string(rec_counter) << endl;

    free(arr);
    cout << "SUCCESS" << endl;
    return 0;
}
