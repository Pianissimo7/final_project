#pragma once

#include "element.hpp"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <math.h>
#include <list>
#include <iostream>

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
        std::list<element *>::reverse_iterator getReverseStart();
        std::list<element *>::reverse_iterator getReverseEnd();
        double getCost(double d);
        double getNaiveCost(double d);
        void print();
        void clear();
    private:
        list<element *> perm;
        double sum;
};