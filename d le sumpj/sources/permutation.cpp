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
std::list<element *>::reverse_iterator permutation::getReverseStart() {
     return this->perm.rbegin();
}
std::list<element *>::reverse_iterator permutation::getReverseEnd() {
    return this->perm.rend();
}
void permutation::print() {
    if (this->getSize() > 0 ) {
        for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end(); ++it) {
            cout << to_string((*it)->getValue()) << ", ";
        }     
    }
    else
        cout << "Empty Permutation";
    cout << endl;
}

double permutation::getCost(double d) {
    double MinCost = getNaiveCost(d);
    double offset = 0;
    for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end() ; ++it) {
        offset += (*it)->getValue();
        if (offset >= d) 
            break;
        MinCost = min(MinCost, getNaiveCost(offset));
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
