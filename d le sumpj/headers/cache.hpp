#include "permutation.hpp"
#include <stdlib.h>
#include <map>
#include <memory>

class cache {
    public:
        cache(vector<element *> OrderedElements);
        ~cache();
        permutation * DgeSumPj;
        map<double, shared_ptr<permutation>> ** PermMap;
        double * OptDDgeSumPjs;
    private:
        double getOptDDgeSumPj();
        permutation * getDgeSumPj(vector<element *> OrderedElements);
        double DOptPerIndex(size_t index);
        size_t size;
};