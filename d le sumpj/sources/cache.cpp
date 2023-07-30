#include "cache.hpp"

using namespace std;

cache::cache(vector<element *> OrderedElements) {
    this->size = OrderedElements.size();
    
    this->DgeSumPj = this->getDgeSumPj(OrderedElements);
    this->PermMap = (map<double, shared_ptr<permutation>> **) malloc(sizeof(map<double, shared_ptr<permutation>> **) * this->size);
    this->OptDDgeSumPjs = (double *) malloc(sizeof(double) * this->size);
    for (size_t i = 0 ; i < this->size ; i++) {
        this->OptDDgeSumPjs[i] = DOptPerIndex(i);
        this->PermMap[i] = new map<double, shared_ptr<permutation>>();
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
    return this->DOptPerIndex(this->size);
}
permutation * cache::getDgeSumPj(vector<element *> OrderedElements) {
    permutation * perm = new permutation();

    for (size_t i = 0 ; i < this->size ; i++) {
        if (i + this->size % 2 == 0) {
            perm->AddToEnd(OrderedElements[i]);
        }
        else {
            perm->AddToStart(OrderedElements[i]);
        }
    }
    return perm;
}
double cache::DOptPerIndex(size_t index) {
    double DOpt = 0.0;
    
    size_t begin = ceil((this->size - index) / 2);
    size_t end = begin + index;
    size_t i = 0;
    for (list<element *>::iterator it = this->DgeSumPj->getStart() ; it != this->DgeSumPj->getEnd(), i <= end; ++it, i++) {
        if (begin <= i) {
            DOpt += (*it)->getValue();
        }
    }
    return DOpt;
}