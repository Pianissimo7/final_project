#include <bits/stdc++.h>
#include "pt_permutation.hpp"
#include "sample.hpp"
#include <fstream>

using namespace project;
using namespace std;

void naive_algo_rec(Sample* s, int* naturals, size_t curr_index, pt_permutation& opt_perm) {
    if (curr_index == opt_perm.s->size) {
        
        pt_permutation curr_permutation(s, naturals);
        
        if (curr_permutation < opt_perm) {
            opt_perm = curr_permutation;
        }
        return;
    }
    int size = opt_perm.s->size;
    
    int temp;

    for (int i = curr_index ; i < size ; i++) {
        int copy[size];
        memcpy(copy, naturals, sizeof(int) * size);
        temp = copy[curr_index];
        copy[curr_index] = copy[i];
        copy[i] = temp;
        naive_algo_rec(s, copy, curr_index + 1, opt_perm);
    }
}

void naive_algo(Sample* s) { 
    int size = s->size;
    int naturals[size];
    for (int i = 0 ; i < size ; i++) {
        naturals[i] = i;
    }
    
    pt_permutation optimal_perm(s, naturals);
    naive_algo_rec(s, naturals, 0, optimal_perm);
    cout << endl;
    optimal_perm.print_perm();
}

int get_min_cost(int p[], int size) {
    return 0;
}

int main() {
    
    Sample s(CREATE_DATA);
    // Sample s(READ_DATA);
    s.print_data();
    naive_algo(&s);

    return 0;
}