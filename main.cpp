#include <bits/stdc++.h>
#include "pt_permutation.hpp"
#include "dynamic_permutation.hpp"
#include "sample.hpp"
#include <fstream>

using namespace project;
using namespace std;

string FILE_PATH = "example.csv";
char DELIMITER = ',';
string NAIVE_ALGO = "NAIVE";
string DYNAMIC_ALGO = "DYNAMIC";

void naive_algo_rec(const Sample* s, int* naturals, size_t curr_index, pt_permutation& opt_perm) {
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

pt_permutation naive_algo(const Sample* s) { 
    int size = s->size;
    int naturals[size];
    for (int i = 0 ; i < size ; i++) {
        naturals[i] = i;
    }
    
    pt_permutation optimal_perm(s, naturals);
    naive_algo_rec(s, naturals, 0, optimal_perm);
    return optimal_perm;
}

dynamic_permutation dynamic_programing_algo(Sample* s) { 
    int sorted_times[s->size];
    int max_p_time = *max_element(s->p, s->p + s->size);
    int count[max_p_time + 1] = {0};
    for (size_t i = 0 ; i < s->size ; i++) {
        count[s->p[i]]++;
    }
    size_t j = 0;
    for (size_t i = 0 ; i <= (size_t)max_p_time ; i++) {        
        if (count[i] > 0) {
            count[i]--;
            sorted_times[j] = i;
            j++;
            i--;
        }
    }
    dynamic_permutation dp = dynamic_permutation(s->size);
    //add the elements from the largest to the smallest into the dynamic permutation
    for (size_t i = s->size ; i > 0 ; i--) {
        dp.add_element(sorted_times[i - 1]);
    }
    return dp;
}

void prepare_output_file(ofstream& file) {
    file << "Cycle" << DELIMITER;
    file << "Algo_Type" << DELIMITER;
    for (size_t i = 1 ; i <= MAX_SAMPLES ; i++) {
        file << "Element: " << i << DELIMITER;
    }
    file << "Cost" << DELIMITER;
    file << "D_Index" << DELIMITER << "\n";
}

void write_output_to_file(ofstream& file, int cycle_number, string algo_name, const pt_permutation &perm) {
    file << to_string(cycle_number) << DELIMITER;
    file << algo_name << DELIMITER;
    for (size_t i = 0 ; i < MAX_SAMPLES ; i++) {
        if (perm.s->size > i) {
            file << to_string(perm.s->p[perm.perm[i]]) << DELIMITER;

        }
        else {
            file << "N/A" << DELIMITER;
        }
    }
    file << to_string(perm.min_cost) << DELIMITER;
    file << to_string(perm.d_index) << DELIMITER << "\n";
}

int main() {
    ofstream file(FILE_PATH, ios::trunc);
    prepare_output_file(file);
    if (file.is_open()) {
        for (int i = 0 ; i < 1000 ; i++) {
            
            Sample s(CREATE_DATA, i);
            // Sample s(READ_DATA);
            pt_permutation np = naive_algo(&s);
            dynamic_permutation dp = dynamic_programing_algo(&s);
            Sample sample_dp(3, i, dp.get_output(), s.size, s.d);

            pt_permutation dp_pt_perm(&sample_dp);
            dp_pt_perm.d_index = dp.get_element_amount_left();
            dp_pt_perm.min_cost = dp_pt_perm.get_cost(dp.get_element_amount_left());

            bool same_cost = (dp_pt_perm.min_cost == np.min_cost);
            cout << same_cost << endl;
            
            write_output_to_file(file, i, NAIVE_ALGO, np);
            write_output_to_file(file, i, DYNAMIC_ALGO, dp_pt_perm);

            if (same_cost == false) {
                np.print_perm();
                dp.print_perm();
                break;
            }
        }
        file.close();
    }
    else {
        cout << "Failed to open the file." << endl; 
    }
    return 0;
}

