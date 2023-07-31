#include "cache.hpp"

using namespace std;

cache::cache(vector<element *> OrderedElements) {
    this->size = OrderedElements.size();
    this->DgeSumPj = this->getDgeSumPj(OrderedElements);
    this->PermMap = (unordered_map<double, shared_ptr<permutation>> **) malloc(sizeof(unordered_map<double, shared_ptr<permutation>> **) * this->size);
    this->OptDDgeSumPjs = this->getAllDOptPerIndex();
    for (size_t i = 0 ; i < this->size ; i++) { 
        this->PermMap[i] = new unordered_map<double, shared_ptr<permutation>>();
    }
}
cache::~cache() {

    for (size_t i = 0 ; i < this->size ; i++) {
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
    
    size_t index = this->size - 1;
    
    size_t i = 0;
    for (list<element *>::iterator it = this->DgeSumPj->getStart() ; it != this->DgeSumPj->getEnd(); ++it, i++) {
        if (i < this->size / 2 + (this->size % 2)) {
            AllDOptPerIndex[index] += (*it)->getValue();
        }
        else {
            AllDOptPerIndex[index - 1] += (*it)->getValue();
        }
    }
    
    index -= 2;
    
    list<element *>::iterator start = this->DgeSumPj->getStart();
    std::list<element *>::reverse_iterator end = this->DgeSumPj->getReverseStart();
    
    for (; index + 1 > 0 ; index--) {
        if ((this->size + index) % 2 == 1) {
            AllDOptPerIndex[index] = AllDOptPerIndex[index + 2] - (*start)->getValue();
            ++start;
        }
        else {
            AllDOptPerIndex[index] = AllDOptPerIndex[index + 2] - (*end)->getValue();
            ++end;
        }
    }
    
    return AllDOptPerIndex;
}