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

double Forex::getRate(std::string date) {
    double rate = this->rates[date];

    if (rate) {
        return rate;
    } else {
        std::string error = "ERROR getRate(date) - There is not rate at " + date;
        throw std::invalid_argument(error);
    }
}

void Forex::printRates() {
    for (std::map<std::string, double>::iterator it = this->rates.begin();
         it != this->rates.end();
         ++it) {
        std::cout << std::endl << it->first << ": " << it->second;
    }
    std::cout << std::endl;
}

void Forex::clearRates() {
    this->rates.clear();
}

void Forex::getHistoricalRates(const char *period1, const char *period2) {
    std::time_t begin = dateToEpoch(period1);
    std::time_t end = dateToEpoch(period2);
    
    if (begin <= end) {
        std::time_t current = begin;

        while (current <= end) {
            std::string currentDate = epochToDate(current);

            double rate = getForexRate(currentDate, this->base, this->symbol);
            this->rates[currentDate] = rate;

            // next day
            current += 86400;
        }
    }
}
