#include "pt_permutation.hpp"
#include <iostream>

pt_permutation::pt_permutation(int p[], int size) {
    this->p = p;
    this->size = size;
    calculate_min_cost_and_p_opt(this->min_cost, this->p_opt);
}
pt_permutation::pt_permutation(pt_permutation &copy) {
    this->p = copy.p;
    this->size = copy.size;
    this->min_cost = copy.min_cost;
    this->p_opt = copy.p_opt;
    calculate_min_cost_and_p_opt(this->min_cost, this->p_opt);
}
void pt_permutation::calculate_min_cost_and_p_opt(int& min_cost, int& p_opt) {

}
void pt_permutation::update_opt() {
    calculate_min_cost_and_p_opt(this->min_cost, this->p_opt);
}
void pt_permutation::set_at(int val, int index) {
    try {
        if (index >= size) {
            throw;
        }
        this->p[index] = val;
    }
    catch (...) {
        std::cout << "index out of bounds at pt_permutation::set_at\n";
    }
}
bool operator<(const pt_permutation& perm1, const pt_permutation& perm2) {
    return perm1.min_cost < perm2.min_cost;
}