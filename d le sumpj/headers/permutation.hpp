#pragma once

#include "element.hpp"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <math.h>
#include <list>
#include <iostream>
#include <limits>

using namespace std;

class permutation {
    public:
        permutation();
        permutation(permutation * other);
        ~permutation();
        size_t getSize();
        void AddToStart(element * e);
        void AddToEnd(element * e);
        list<element *>::iterator getStart();
        list<element *>::iterator getEnd();
        double getCost(double d, double RunningSum=0, size_t ElementLeftNo=0, size_t ElementRightNo=0);
        double getNaiveCost(double d);
        void print();
        void clear();
    private:
        list<element *> perm;
        double sum;
};