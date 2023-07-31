#include "permutation.hpp"
#include <stdlib.h>
#include <unordered_map>
#include <memory>

class cache {
    public:
        cache(vector<element *> OrderedElements);
        ~cache();
        permutation * DgeSumPj;
        unordered_map<double, shared_ptr<permutation>> ** PermMap;
        double * OptDDgeSumPjs;
    private:
        double getOptDDgeSumPj();
        permutation * getDgeSumPj(vector<element *> OrderedElements);
        double * getAllDOptPerIndex();
        size_t size;
};