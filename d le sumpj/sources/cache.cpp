#include "cache.hpp"

using namespace std;

cache::cache(vector<element *> OrderedElements) {
    this->size = OrderedElements.size();
    this->DgeSumPj = this->getDgeSumPj(OrderedElements);
    this->PermMap = (unordered_map<double, permutation *> **) malloc(sizeof(unordered_map<double, permutation *> **) * this->size);
    for (size_t i = 0 ; i < this->size ; i++) { 
        this->PermMap[i] = new unordered_map<double, permutation *>();
    }
    this->OptDDgeSumPjs = this->getAllDOptPerIndex();
    
}
cache::~cache() {

    for (size_t i = 0 ; i < this->size ; i++) {
        for (auto it = this->PermMap[i]->begin(); it != this->PermMap[i]->end(); ++it) {
            delete it->second; // Delete each object pointed to by the raw pointer
        }
        this->PermMap[i]->clear();
    }
    free(this->PermMap);
    delete(this->DgeSumPj);
    free(this->OptDDgeSumPjs);
}
double cache::getOptDDgeSumPj() {
    return this->OptDDgeSumPjs[this->size - 1];
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
double * cache::getAllDOptPerIndex() {
    double * AllDOptPerIndex = (double *) calloc(sizeof(double), this->size);
  
    size_t index = 0;
    double sum = 0;
    for (list<element *>::iterator it = this->DgeSumPj->getStart() ; it != this->DgeSumPj->getEnd(); ++it, index++) {
        sum += (*it)->getValue();
        AllDOptPerIndex[index] = sum;
    }

    return AllDOptPerIndex;
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