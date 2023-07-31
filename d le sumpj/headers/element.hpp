#include <stdlib.h>

class element {
    public:
        element(double value, size_t index): value(value), index(index) {}
        double getValue() const;
        size_t getIndex() const;
        bool operator>(element other) const;
        bool operator<(element other) const;
    private:
        double value;
        size_t index;
};