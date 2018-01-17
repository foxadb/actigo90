#ifndef QUOTE_HPP
#define QUOTE_HPP

#include <string>

#include "spot.hpp"

class Quote {
    private:
    std::string symbol;
    int period1;
    int period2;

    Spot* spots;

    public:
    Quote(std::string symbol);
    void getHistoricalData(int period1, int period2, const char *interval);

};

#endif /* QUOTE_HPP */
