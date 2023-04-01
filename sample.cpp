#include "sample.hpp"
using namespace std;
using namespace project;


fstream& project::operator>> (std::fstream& is, Sample& s){
    
    is >> s.n;
    s.p = (int*)realloc(s.p,s.n*(sizeof(int)));
    for(size_t i = 0; i<s.n; i++){
        is >> s.p[i];
    }
    is >> s.d;
    return is;
}

