#ifndef FOREX_HPP
#define FOREX_HPP

#include <string>
#include <vector>

#include "spot.hpp"

class Forex {
public:
    Forex(std::string base, std::string symbol);
    ~Forex();

    Spot getSpot(size_t i);
    Spot getSpot(std::string date);

    double getRate(size_t i);
    double getRate(std::string date);

    void printSpots();
    void clearSpots();

    void getHistoricalSpots(const char *period1, const char *period2);

private:
    std::string base;
    std::string symbol;
    std::vector<Spot> spots;
};

#endif /* FOREX_HPP */
