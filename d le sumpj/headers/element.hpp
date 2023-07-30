#include <stdlib.h>

class element {
    public:
        element(int value, size_t index): value(value), index(index) {}
        double getValue();
        size_t getIndex();
        bool operator>(element other);
        bool operator<(element other);
    private:
        double value;
        size_t index;
};