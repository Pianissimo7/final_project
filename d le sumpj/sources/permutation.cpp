#include "../headers/permutation.hpp"
permutation::permutation() {
    this->offset = 0;
}
permutation::permutation(permutation * other) {
    for (list<element *>::iterator it = other->getStart(); it != other->getEnd(); ++it) {
        this->perm.push_back(*it);
    }
    this->offset = other->offset;
}
permutation::~permutation() {

}
size_t permutation::getSize() {
    return this->perm.size();
}
void permutation::AddToStart(element * e) {
    this->perm.push_front(e);
    this->offset += e->getValue();
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
    for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end(); ++it) {
        cout << to_string((*it)->getValue()) << ", ";
    }
    cout << endl;
}
double permutation::getCost(double d) {
    double Cost = 0;
    double RunningSum = 0;
    for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end(); ++it) {
        RunningSum += (*it)->getValue();
        Cost += fabs(RunningSum - d);
    }
    return Cost;
}
double permutation::getCostWithOffset(double d) {
    return this->getCost(min(this->offset, d));
}
void permutation::clear() {
    for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end(); ++it) {
        delete(*it);
    }
}
