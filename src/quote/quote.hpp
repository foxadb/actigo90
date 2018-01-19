#ifndef QUOTE_HPP
#define QUOTE_HPP

#include <string>
#include <ctime>
#include <vector>

#include "spot.hpp"

class Quote {
    private:
    std::string symbol;
    std::vector<Spot> spots;

    public:
    Quote(std::string symbol);
    ~Quote();

    Spot getSpot(size_t i);
    Spot getSpot(std::string date);

    void printSpots();
    void clearSpots();

    std::string getHistoricalCsv(std::time_t period1,
                                 std::time_t period2,
                                 const char *interval);

    void getHistoricalSpots(std::time_t period1,
                            std::time_t period2,
                            const char *interval);

    void getHistoricalSpots(const char *period1,
                            const char *period2,
                            const char *interval);
};

#endif /* QUOTE_HPP */
