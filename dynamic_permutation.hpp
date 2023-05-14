#include <stdlib.h>
#include <iostream>
class dynamic_permutation {
    public:
        dynamic_permutation(size_t pt_size) : element_amount_left(0), element_amount_right(0), size(pt_size), left_cost(0), right_cost(0) {
            this->output = (int *) calloc(sizeof(int), pt_size);
        };  
        ~dynamic_permutation();
        void add_element(int element);
        void print_perm();
        int* get_output();
        size_t get_element_amount_left();
    private:
        void add_left(int element, int cost);
        void add_right(int element, int cost);
        
        size_t element_amount_left;
        size_t element_amount_right;
        size_t size;
        int left_cost;
        int right_cost;
        int *output;
        
};
