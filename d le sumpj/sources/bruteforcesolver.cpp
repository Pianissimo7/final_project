#include "bruteforcesolver.hpp"

using namespace std;

bruteforcesolver::bruteforcesolver(double * p, size_t size, double d) {
    this->p = (double *)calloc(sizeof(double), size);
    for (size_t i = 0 ; i < size ; i++) {
        this->p[i] = p[i];
    }
    this->d = d;
    this->size = size;
}
bruteforcesolver::~bruteforcesolver() {
    free(this->p);
}
vector<double> bruteforcesolver::getSolution() {
    vector<double> * PVector = new vector<double>(this->p, this->p + this->size);
    sort(PVector->begin(), PVector->end());
    double MinCost = numeric_limits<double>::max();
    vector<double> bestPermutation;
    
    do {
        double CurrentCost = this->CalculateMinCost(*PVector);
        if (CurrentCost < MinCost) {
            MinCost = CurrentCost;
            bestPermutation = *PVector;
        }
    } while (next_permutation(PVector->begin(), PVector->end()));
    
    delete(PVector);

    return bestPermutation;
}
double bruteforcesolver::CalculateMinCost(vector<double> PVector) {
    double sum = 0.0;
    double MinCost = CalculateMinCostPerD(PVector, this->d);
    size_t i = 0;
    while(sum < this->d) {
        MinCost = min(CalculateMinCostPerD(PVector, sum), MinCost);
        if (i == this->size) {
            break;
        }
        sum += PVector[i];
        i++;
    }
    return MinCost;
}
double bruteforcesolver::CalculateMinCostPerD(vector<double> PVector, double d) {
    double Cost = 0;
    double RunningSum = 0;
    for (size_t i = 0 ; i < this->size ; i++) {
        RunningSum += PVector[i];
        Cost += fabs(RunningSum - d);
    }
    return Cost;
}