#include "cache.hpp"

using namespace std;

cache::cache(vector<element *> OrderedElements) {
    this->size = OrderedElements.size();
    this->PermMap = (unordered_map<double, permutation *> **) malloc(sizeof(unordered_map<double, permutation *> **) * this->size);
    for (size_t i = 0 ; i < this->size ; i++) { 
        this->PermMap[i] = new unordered_map<double, permutation *>();
    }
    this->RunningSums = this->getRunningSums(OrderedElements);
}
cache::~cache() {

    for (size_t i = 0 ; i < this->size ; i++) {
        for (auto it = this->PermMap[i]->begin(); it != this->PermMap[i]->end(); ++it) {
            delete(it->second); // Delete each object pointed to by the raw pointer
        }
        this->PermMap[i]->clear();
    }
    free(this->PermMap);
    free(this->RunningSums);
}
permutation * cache::getDgeSumPj(vector<element *> OrderedElements) {
    permutation * perm = new permutation();

    for (size_t i = 0 ; i < this->size ; i++) {
        if ((i + this->size) % 2 == 0) {
            perm->AddToEnd(OrderedElements[i]);
        }
        else {
            perm->AddToStart(OrderedElements[i]);
        }
    }
    return perm;
}
double * cache::getRunningSums(vector<element *> OrderedElements) {
    double * RunningSums = (double *) calloc(sizeof(double), this->size);
    double RunningSum = 0;
    for (size_t i = 0 ; i < OrderedElements.size() ; i++) {
        RunningSum += OrderedElements[i]->getValue();
        RunningSums[i] = RunningSum;
    }

    return RunningSums;
}
void cache::printMap() {
    cout << "Print Map " << endl;
    for (size_t i = 0 ; i < this->size ; i++) {
        for (auto it = this->PermMap[i]->begin(); it != this->PermMap[i]->end(); ++it) {
            cout << "index: " << i << " d: " << to_string(it->first) << " ";
            cout << "perm: ";
            it->second->print();// Delete each object pointed to by the raw pointer
        }
    }
}