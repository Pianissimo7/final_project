#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>

#pragma once

const int MAX_SAMPLES = 10;
const int MIN_SAMPLES = 9;
const int MIN_P = 1;
const int MAX_P = 50;
const int MAX_OVERHEAD_D = 50;

const int CREATE_DATA = 1;
const int READ_DATA = 2;
const int INPUT_DATA = 3;

namespace project{

    class Sample {
        public:
            Sample(int mode, int * data = NULL, int size = 0, int deadline = 0);
            ~Sample();
            
            void print_data() const;
            size_t size; // size of processing times array
            int* p; // processing times array
            int d; // deadline time
        private:
    };
}