#include "pt_permutation.hpp"
#include <bits/stdc++.h>

using namespace std;

pt_permutation::pt_permutation(const Sample* s): s(s) {
    size_t size = s->size;
    this->perm = (int *) malloc(sizeof(int) * size);
    for (size_t i = 0 ; i < size ; i++) {
        this->perm[i] = i;
    }
    calculate_opt();
}
pt_permutation::pt_permutation(const Sample* s, int indexes[]): s(s) {
    size_t size = s->size;
    this->perm = (int*) malloc(sizeof(int) * size);
    memcpy(this->perm, indexes, size * sizeof(int));
    calculate_opt();
}
pt_permutation& pt_permutation::operator=(const pt_permutation& other) {
    int size = other.s->size;
    this->s = other.s;
    memcpy(this->perm, other.perm, sizeof(int) * size);
    this->d_index = other.d_index;
    this->min_cost = other.min_cost;
    return *this;
}
pt_permutation::~pt_permutation() {
    free(this->perm);
}
void pt_permutation::calculate_opt() {
    this->d_index = 0;
    this->min_cost = this->get_cost(this->d_index);
    
    for (size_t i = 1 ; i < s->size + 1 ; i++) {
        int cost = get_cost(i);
        if (cost < this->min_cost) {
            this->d_index = i;
            this->min_cost = cost;
        }
    }
}
int pt_permutation::get_cost(size_t d_index) {
    return get_cost_left(d_index) + get_cost_right(d_index);
}
int pt_permutation::get_cost_left(size_t d_index) {
    int sum_bottom = 0;
    if (d_index > 1) {
        for (size_t i = 1 ; i < d_index ; i++) {
            sum_bottom += this->s->p[this->perm[i]] * i;
        }
    }
    return sum_bottom;
}
int pt_permutation::get_cost_right(size_t d_index) {
    int sum_top = 0;
    for (size_t i = d_index ; i < s->size ; i++) {
        sum_top += this->s->p[this->perm[i]] * (s->size - i);
    }
    return sum_top;
}
void pt_permutation::print_perm() {
    size_t size = s->size;
    cout<<"n = " << size << endl;
    cout<<"d_index = " << this->d_index << endl;
    cout<<"min_cost = " << this->min_cost << endl;
    for (size_t i = 0 ; i < size ; i++) {
        cout << "    (" << this->perm[i] << ")p_" << i << " = " << this->s->p[this->perm[i]] << endl;
    }
}
bool operator<(const pt_permutation& perm1, const pt_permutation& perm2) {
    return perm1.min_cost < perm2.min_cost;
}