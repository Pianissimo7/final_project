#include "dynamic_permutation.hpp"
using namespace std;

dynamic_permutation::~dynamic_permutation() {
    free(this->output);
}

void dynamic_permutation::add_element(int element) {
    int cost_if_left = this->left_cost + this->element_amount_left * element;
    int cost_if_right = this->right_cost + (this->element_amount_right + 1) * element;
    if (cost_if_right - this->right_cost < cost_if_left - this->left_cost) {
        //put the element in the right side
        this->add_right(element, cost_if_right);
    }
    else if (cost_if_right > cost_if_left) {
        // put the element in the left side
        this->add_left(element, cost_if_left);
    }
    else {
        if (this->element_amount_left > this->element_amount_right) {
            add_right(element, cost_if_right);
        }
        else {
            add_left(element, cost_if_left);
        }
    }
}

void dynamic_permutation::add_left(int element, int cost) {
    this->output[this->element_amount_left] = element;
    this->element_amount_left++;
    this->left_cost = cost;
}
void dynamic_permutation::add_right(int element, int cost) {
    this->output[(this->size - 1) - this->element_amount_right] = element;
    this->element_amount_right++;
    this->right_cost = cost;
}

void dynamic_permutation::print_perm() {
    for (size_t i = 0 ; i < this->size ; i++) {
        if (i == this->element_amount_left) {
            cout << "------------------------------" << endl;    
        }
        cout << this->output[i] << endl;
    }
    cout << "cost: " << (this->left_cost + this->right_cost) << endl;
}
int* dynamic_permutation::get_output() {
    return this->output;
}
size_t dynamic_permutation::get_element_amount_left() {
    return this->element_amount_left;
}