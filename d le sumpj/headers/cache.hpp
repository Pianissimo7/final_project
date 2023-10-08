#include "permutation.hpp"
#include <stdlib.h>
#include <unordered_map>
#include <memory>

class cache {
    public:
        cache(vector<element *> OrderedElements);
        ~cache();
        unordered_map<double, permutation *> ** PermMap;
        double * RunningSums;
        void printMap();
    private:
        permutation * getDgeSumPj(vector<element *> OrderedElements);
        double * getRunningSums(vector<element *> OrderedElements);
        size_t size;
};