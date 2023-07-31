#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iostream>

using namespace std;

class bruteforcesolver {
    public:
        bruteforcesolver(double * p, size_t size, double d);
        ~bruteforcesolver();
        vector<double> getSolution();
        double CalculateMinCost(vector<double> PVector);
    private:
        double CalculateMinCostPerD(vector<double> PVector, double d);
        double * p;
        size_t size;
        double d;
};