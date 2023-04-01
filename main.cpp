#include <bits/stdc++.h>
#include "pt_permutation.hpp"
#include "sample.hpp"
#include <fstream>

using namespace project;
using namespace std;
pt_permutation rec(int p[], int size, int i, pt_permutation perm) {
    if (i == perm.size) {
        return perm;
    }
    // will be returned at the end, and is the optimal permutation found.
    pt_permutation opt_perm(NULL, 0);
    // set the min_cost of the initial optimized permutation to max.
    opt_perm.min_cost = INT_MAX;

    for (int j = 0 ; j < size; j++) {
        // temporary permutation, a local optimum
        pt_permutation temp_perm(perm);
        
        perm.set_at(p[j], i);
        int rec_p[size - i];
        
        // copy p into rec_p except for the i'th element (for the reccursion)
        for (int k = 0 ; k < size; k++) {
            if (k < i) {
                rec_p[k] = p[k];
            }
            else if (k > i){
                rec_p[k] = p[k - 1];
            }
        }
        // get the local optimum
        temp_perm = rec(rec_p, (size - i), (i + 1), temp_perm);
        
        // calculate the minimum cost of the localy optimized permutation
        temp_perm.update_opt();

        //compare to the overall optimal and replace if it is better
        if (temp_perm.min_cost < opt_perm.min_cost) {
            opt_perm(temp_perm);
        }
    }
    return opt_perm;
}

void naive_algo(int p[], int size, int d) {
    pt_permutation perm = perm(p, size);
    rec(p, size, 0, perm);
}

int get_min_cost(int p[], int size) {
    return 0;
}

int main() {
    
    fstream file;
    file.open("f.txt",ios::in);
    
    Sample s;
    while(file.peek() != EOF){
        file>>s;
        //TODO
        // create a sample object from each line
    
        // processing times
        
        naive_algo(s.getP(), s.getN(), s.getD());
    }
    return 0;
}

// int get_cost(int group1[], int group2[], int d) {
//     //sort the groups
//     int group1_size = sizeof(group1) / sizeof(int);
//     std::sort(group1, group1 + group1_size);
//     int group2_size = sizeof(group2) / sizeof(int);
//     std::sort(group2, group2 + group2_size);

//     int group1_cost = 0;
//     int group2_cost = 0;

//     for (int i = 0 ; i < group1_size ; i++) {
//         group1_cost += group1_cost + group1[i];
//     }
//     for (int i = 0 ; i < group2_size ; i++) {
//         group2_cost += group2_cost + group2[i];
//     }
//     return group1_cost + group2_cost;
// }