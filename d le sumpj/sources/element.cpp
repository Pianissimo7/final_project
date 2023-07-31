#include "../headers/element.hpp"

double element::getValue() const {
    return this->value;
}
size_t element::getIndex() const {
    return this->index;
}
bool element::operator>(const element other) const {
    return this->getIndex() > other.getIndex();
}
bool element::operator<(const element other) const {
    return this->getIndex() < other.getIndex();
}