#include "../headers/permutation.hpp"
permutation::permutation() {

}
permutation::permutation(permutation * other) {
    for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end(); ++it) {
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
    for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end(); ++it) {
        cout << (*it)->getValue() << ", ";
    }
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
