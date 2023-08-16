#include "../headers/permutation.hpp"
permutation::permutation() {
    this->pivot = nullptr;
}
permutation::permutation(permutation * other) {
    for (list<element *>::iterator it = other->getStart(); it != other->getEnd(); ++it) {
        this->perm.push_back(*it);
    }
    this->pivot = other->pivot;
}
permutation::~permutation() {

}
size_t permutation::getSize() {
    return this->perm.size();
}
void permutation::AddToStart(element * e) {
    if (this->getSize() == 0) {
        this->pivot = e;
    }
    this->perm.push_front(e);
}
void permutation::AddToEnd(element * e) {
    if (this->getSize() == 0) {
        this->pivot = e;
    }
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

// double permutation::getCost(double d, double RunningSum, size_t ElementLeftNo, size_t ElementRightNo) {
//     double offset = 0;
    
//     double LastElement = numeric_limits<double>::max();
//     size_t DuplicateCount = 0;
//     for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end() ; ++it) {
        
//         if (offset - (LastElement * (DuplicateCount / 2)) + (*it)->getValue() >= d) { 
//             offset -= LastElement * ((DuplicateCount / 2));
//             break;
//         }
//         else if (offset + (*it)->getValue() >= d && (LastElement != (*it)->getValue())) {
//             break;
//         }
//         if ((*it)->getValue() > LastElement) {
//             offset -= LastElement * (DuplicateCount / 2);
//             if (ElementLeftNo >= ElementRightNo) {
//                 return getNaiveCost(offset) + ElementLeftNo * offset + ElementRightNo * fabs(RunningSum - offset);
//             }
//             else {
//                 offset -= LastElement;
//                 return getNaiveCost(offset) + ElementLeftNo * offset + ElementRightNo * fabs(RunningSum - offset);
//             }
//         }
//         if ((*it)->getValue() < LastElement) {
//             DuplicateCount = 0;
//         }
//         else {
//             DuplicateCount++;
//         }
//         offset += (*it)->getValue();
//         LastElement = (*it)->getValue();
//     }

double permutation::getCost(double d, double RunningSum, size_t ElementLeftNo, size_t ElementRightNo) {
    if (this->getSize() == 0) {
        return 0;
    }
    double offset = 0;
    
    for (list<element *>::iterator it = this->perm.begin(); it != this->perm.end() ; ++it) {
        
        if (offset + (*it)->getValue() >= d) {
            break;
        }
        if (*it == this->pivot) {
            double CostPivotEarly = getNaiveCost(offset) + ElementLeftNo * offset + ElementRightNo * fabs(RunningSum - offset);
            offset += (*it)->getValue();    
            double CostPivotTardy = getNaiveCost(offset) + ElementLeftNo * offset + ElementRightNo * fabs(RunningSum - offset);
            
            double CostForD = getNaiveCost(d) + ElementLeftNo * d + ElementRightNo * fabs(RunningSum - d);
            return min(CostForD, min(CostPivotEarly, CostPivotTardy));
        }
        offset += (*it)->getValue();
    }
    
    double CostForOffset = getNaiveCost(offset) + ElementLeftNo * offset + ElementRightNo * fabs(RunningSum - offset);
    double CostForD = getNaiveCost(d) + ElementLeftNo * d + ElementRightNo * fabs(RunningSum - d);
    
    return min(CostForOffset, CostForD);
    
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
