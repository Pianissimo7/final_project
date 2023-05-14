#include <bits/stdc++.h>
#include "pt_permutation.hpp"
#include "dynamic_permutation.hpp"
#include "sample.hpp"
#include <fstream>

using namespace project;
using namespace std;

void write_output_to_file(pt_permutation perm) {
    std::ofstream file("example.txt");

    if (file.is_open()) {
        
        file << "the data we are working on:\n";
        file << "    size:\n";
        file << "        " << perm.s->size << "\n";
        file << "    d:\n";
        file << "        " << perm.s->d << "\n";
        file << "    p[]:\n";
        for (size_t i = 0 ; i < perm.s->size ; i++) {
            file << "        p[" << i << "]:" << perm.s->p[i] << "\n";
        }
        file << "\n";
        file << "the output is:\n";
        file << "    the early jobs:\n";
        for (size_t i = 0 ; i <  perm.d_index ; i++) {
            file << "        p[" << perm.perm[i] << "]:" << perm.s->p[perm.perm[i]] << "\n";
        }   
        file << "    the tardy jobs:\n";
        for (size_t i = perm.d_index ; i < perm.s->size ; i++) {
            file << "        p[" << perm.perm[i] << "]:" << perm.s->p[perm.perm[i]] << "\n";
        }   
        file << "    cost:\n";
        file << "        " << perm.min_cost << "\n";
        file.close();
        cout << "Data has been written to the file." << endl;
    } else {
        cout << "Failed to open the file." << endl;
    }
}

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

pt_permutation naive_algo(Sample* s) { 
    int size = s->size;
    int naturals[size];
    for (int i = 0 ; i < size ; i++) {
        naturals[i] = i;
    }
    
    pt_permutation optimal_perm(s, naturals);
    naive_algo_rec(s, naturals, 0, optimal_perm);
    write_output_to_file(optimal_perm);
    return optimal_perm;
}
dynamic_permutation dynamic_programing_algo(Sample* s) { 
    int sorted_times[s->size];
    memcpy(sorted_times, s->p, s->size * sizeof(int));

    sort(sorted_times, sorted_times + s->size);
    dynamic_permutation dp = dynamic_permutation(s->size);
    //add the elements from the largest to the smallest into the dynamic permutation
    for (size_t i = s->size ; i > 0 ; i--) {
        dp.add_element(sorted_times[i - 1]);
    }
    
    return dp;
}

int main() {
    for (int i = 0 ; i < 1000 ; i++) {
        Sample s(CREATE_DATA);
        // Sample s(READ_DATA);
        pt_permutation np = naive_algo(&s);
        dynamic_permutation dp = dynamic_programing_algo(&s);
        // dp.print_perm();
        Sample sample_dp(3, dp.get_output(), s.size, s.d);

        pt_permutation dp_pt_perm(&sample_dp);
        dp_pt_perm.d_index = dp.get_element_amount_left();

        bool same_cost = (dp_pt_perm.get_cost(dp.get_element_amount_left()) == np.min_cost);
        cout << same_cost << endl;
        if (same_cost == false) {
            dp.print_perm();
            break;
        }
    }
    
    return 0;
}
