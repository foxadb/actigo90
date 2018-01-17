#include <string>

#include "spot.hpp"

Spot::Spot(std::string date, double open, double high, double low, double close) {
    this->date = date;
    this->open = open;
    this->high = high;
    this->low = low;
    this->close = close;
}