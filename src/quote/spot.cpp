#include <iostream>
#include <string>

#include "spot.hpp"

Spot::Spot(std::string date, double open, double high, double low, double close) {
    this->date = date;
    this->open = open;
    this->high = high;
    this->low = low;
    this->close = close;
}

Spot::~Spot() {}

std::string Spot::getDate() {
    return this->date;
}

double Spot::getOpen() {
    return this->open;
}

double Spot::getHigh() {
    return this->high;
}

double Spot::getLow() {
    return this->low;
}

double Spot::getClose() {
    return this->close;
}

std::string Spot::toString() {
    return "{ date: " + this->date
            + " open: " + std::to_string(this->open)
            + " high: " + std::to_string(this->high)
            + " low: " + std::to_string(this->low)
            + " close: " + std::to_string(this->close)
            + " }";
}

void Spot::printSpot() {
    std::cout << this->toString() << std::endl;
}
