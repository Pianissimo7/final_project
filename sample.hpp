#include <vector>
#include <iostream>
#include <fstream>

namespace project{

    class Sample{
        private:
            size_t n;
            int* p;
            int d;
        public:
            Sample():p(NULL),d(0),n(0){}

            friend std::fstream& operator>> (std::fstream& is, Sample& s);

            size_t getN() const{
                return n;
            }
            int* getP() const{
                
                return p;
            }
            int getD() const{
                return d;
            }
            ~Sample();



    };
}