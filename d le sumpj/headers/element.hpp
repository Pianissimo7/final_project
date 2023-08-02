#include <stdlib.h>

class element {
    public:
        element(double value): value(value) {}
        double getValue() const;
        
    private:
        double value;
};