#include <iostream>
#include "sample.hpp"

using namespace project;
class pt_permutation {
    public:
        pt_permutation(const Sample* s);
        pt_permutation(const Sample* s, int indexes[]);
        ~pt_permutation();
        
        pt_permutation& operator=(const pt_permutation& other);
        friend bool operator<(const pt_permutation& perm1, const pt_permutation& perm2);        
        void calculate_opt();
        void print_perm();
        int get_cost(size_t d_index);
        
        const Sample* s;
        int *perm;
        int min_cost;
        size_t d_index;
        
    private:
        
};
