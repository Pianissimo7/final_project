#include "../headers/element.hpp"

double element::getValue() {
    return this->value;
}
size_t element::getIndex() {
    return this->index;
}
bool element::operator>(element other) {
    return this->getIndex() > other.getIndex();
}
bool element::operator<(element other) {
    return this->getIndex() < other.getIndex();
}