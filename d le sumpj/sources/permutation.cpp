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
    cout<<*this<<endl;
}
ostream& operator<<(ostream& os,  permutation& p){
    if (p.getSize() > 0 ) {
        for (list<element *>::iterator it = p.perm.begin(); it != p.perm.end(); ++it) {
            os << to_string((*it)->getValue()) << ", ";
        }     
    }
    else {
        os << "Empty Permutation";
    }
    return os;
    
}

// double permutation::getCost(double d, double RunningSum, size_t ElementLeftNo, size_t ElementRightNo){
//     double MinCost = numeric_limits<double>::max();;
//     double offset = 0;
    
//     for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end() ; ++it) {
//         double cost = getNaiveCost(offset) + ElementLeftNo * offset + ElementRightNo * fabs(RunningSum - offset);
//         if (MinCost > cost) {
//             MinCost = cost;
//         }
//         if (offset + (*it)->getValue() >= d) { 
//             break;
//         }
//         offset += (*it)->getValue();
        
//     }
//     double DMinCost = getNaiveCost(d) + ElementLeftNo * d + ElementRightNo * fabs(RunningSum - d);
    
//     if (DMinCost <= MinCost) {
//         MinCost = DMinCost;
//     }
//     return MinCost;
// }

double permutation::getCost(double d, double RunningSum, size_t ElementLeftNo, size_t ElementRightNo) {
    double MinCost = numeric_limits<double>::max();
    double offset = 0;
    
    double LastElement = numeric_limits<double>::max();
    for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end() ; ++it) {
        
        if (offset + (*it)->getValue() >= d) { 
            break;
        }
        if ((*it)->getValue() > LastElement) {
            if (ElementLeftNo >= ElementRightNo) {
                return min(MinCost, getNaiveCost(offset) + ElementLeftNo * offset + ElementRightNo * fabs(RunningSum - offset));
            }
            else {
                offset -= LastElement;
                return min(MinCost, getNaiveCost(offset) + ElementLeftNo * offset + ElementRightNo * fabs(RunningSum - offset));
            }
            break;
        }
        offset += (*it)->getValue();
        LastElement = (*it)->getValue();
    }
    MinCost = min(MinCost, getNaiveCost(offset) + ElementLeftNo * offset + ElementRightNo * fabs(RunningSum - offset));
    MinCost = min(MinCost, getNaiveCost(d) + ElementLeftNo * d + ElementRightNo * fabs(RunningSum - d));
    return MinCost;
}
double permutation::getNaiveCost(double d){
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
