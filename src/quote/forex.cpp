#include "forex.hpp"
#include "curl_utils.hpp"
#include "time_utils.hpp"

#include <iostream>
#include <ctime>

Forex::Forex(std::string base, std::string symbol) {
    this->base = base;
    this->symbol = symbol;
}

Forex::~Forex() {}

Spot Forex::getSpot(size_t i) {
    if (i < this->spots.size()) {
        return this->spots[i];
    }

    std::string error = "ERROR: getSpot(index) - Index must be smaller than "
            + std::to_string(this->spots.size());
    throw std::invalid_argument(error);
}

Spot Forex::getSpot(std::string date) {
    for (std::vector<Spot>::iterator it = this->spots.begin();
         it != this->spots.end();
         ++it) {
        if (it->getDate() == date) {
            return *it;
        }
    }

    std::string error = "ERROR getSpot(date) - There is not spot at " + date;
    throw std::invalid_argument(error);
}

double Forex::getRate(size_t i) {
    if (i < this->spots.size()) {
        Spot spot = this->getSpot(i);
        return spot.getClose();
    }

    std::string error = "ERROR: getSpot(index) - Index must be smaller than "
            + std::to_string(this->spots.size());
    throw std::invalid_argument(error);
}

double Forex::getRate(std::string date) {
    Spot spot = this->getSpot(date);
    return spot.getClose();
}

void Forex::printSpots() {
    for (std::vector<Spot>::iterator it = this->spots.begin();
         it != this->spots.end();
         ++it) {
        std::cout << std::endl << it->toString();
    }
    std::cout << std::endl;
}

void Forex::clearSpots() {
    this->spots.clear();
}

void Forex::getHistoricalSpots(const char *period1, const char *period2) {
    std::time_t begin = dateToEpoch(period1);
    std::time_t end = dateToEpoch(period2);
    
    if (begin <= end) {
        std::time_t current = begin;

        while (current <= end) {
            std::string currentDate = epochToDate(current);

            double rate = getForexRate(currentDate, this->base, this->symbol);

            // Create the spot and push it to the rate vector
            Spot spot = Spot(currentDate, rate, rate, rate, rate);
            this->spots.push_back(spot);

            // next day
            current += 86400;
        }
    }
}
