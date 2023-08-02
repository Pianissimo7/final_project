#include "../headers/permutation.hpp"
permutation::permutation() {
}
permutation::permutation(permutation * other) {
    for (list<element *>::iterator it = other->getStart(); it != other->getEnd(); ++it) {
        this->perm.push_back(*it);
    }
}
permutation::~permutation() {

}
size_t permutation::getSize() {
    return this->perm.size();
}
void permutation::AddToStart(element * e) {
    this->perm.push_front(e);
}
void permutation::AddToEnd(element * e) {
    this->perm.push_back(e);
}
list<element *>::iterator permutation::getStart() {
    return this->perm.begin();
}
list<element *>::iterator permutation::getEnd() {
    return this->perm.end();
}
void permutation::print() {
    if (this->getSize() > 0 ) {
        for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end(); ++it) {
            cout << to_string((*it)->getValue()) << ", ";
        }     
    }
    else {
        cout << "Empty Permutation";
    }
    cout << endl;
}

double permutation::getCost(double d, double * OutOffset) {
    double MinCost = getNaiveCost(d);
    double offset = 0;
    double MinCostOffset = 0;
    for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end() ; ++it) {
        offset += (*it)->getValue();
        if (offset >= d) { 
            break;
        }
        double cost = getNaiveCost(offset);
        if (MinCost > cost) {
            MinCost = cost;
            MinCostOffset = offset;
        }
    }
    if (OutOffset != nullptr) {
        *OutOffset = MinCostOffset;
    }
    return MinCost;
}
double permutation::getNaiveCost(double d) {
    double Cost = 0;
    double RunningSum = 0;
    for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end(); ++it) {
        RunningSum += (*it)->getValue();
        Cost += fabs(RunningSum - d);
    }
    return Cost;
}
void permutation::clear() {
    for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end(); ++it) {
        delete(*it);
    }
}
