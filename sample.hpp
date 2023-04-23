#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>

#pragma once

const int MAX_SAMPLES = 5;
const int MIN_SAMPLES = 3;
const int MIN_P = 1;
const int MAX_P = 50;
const int MAX_OVERHEAD_D = 50;
const int CREATE_DATA = 1;
const int READ_DATA = 2;

namespace project{

    class Sample {
        public:
            Sample(int mode);
            ~Sample();

            // friend std::fstream& operator>> (std::fstream& is, Sample& s);

            
            void print_data() const;
            size_t size;
            int* p;
            int d;
        private:
    };
}