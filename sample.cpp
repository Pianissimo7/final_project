#include "sample.hpp"
using namespace std;
using namespace project;

Sample::Sample(int mode) {
    if (mode == CREATE_DATA) {
        std::srand(std::time(nullptr));
        // number of processing times
        this->size = MIN_SAMPLES + std::rand() % (MAX_SAMPLES - MIN_SAMPLES);
        // and array to store the processing times
        this->p = (int *)malloc(sizeof(int) * this->size);
        this->d = 0;
        for (size_t i = 0 ; i < this->size ; i++) {
            // randomize a value for each of the n processing times
            this->p[i] = MIN_P + std::rand() % (MAX_P - MIN_P);
            // sum up the processing times to get d
            this->d += this->p[i];
        }
        // just because d >= sum(p_j) and not ==
        this->d += (std::rand() % MAX_OVERHEAD_D);
    }
    else {
        // for testing a specific example.
        // mostly for debug currently.
        this->size = 3;
        this->p = (int *)malloc(sizeof(int) * this->size);
        this->p[0] = 10;
        this->p[1] = 1;
        this->p[2] = 1;
        
        this->d = 121;
    }
}

Sample::~Sample() {
    free(this->p);
}

void Sample::print_data() const {
    cout<<"n = " << this->size << endl;
    cout<<"d = " << this->d << endl;
    for (size_t i = 0 ; i < this->size ; i++) {
        cout<<"    p_" << i << " = " << this->p[i] << endl;
    }
}

