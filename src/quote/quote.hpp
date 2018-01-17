#ifndef QUOTE_HPP
#define QUOTE_HPP

#include <string>
#include <vector>

#include "spot.hpp"

class Quote {
    private:
    std::string symbol;
    int period1;
    int period2;
    std::vector<Spot> spots;

    public:
    Quote(std::string symbol);
    ~Quote();

    std::string getHistoricalCsv(int period1, int period2, const char *interval);
    void getHistoricalSpots(int period1, int period2, const char *interval);

    void printSpots();
};

#endif /* QUOTE_HPP */
