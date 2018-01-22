#ifndef FOREX_HPP
#define FOREX_HPP

#include <string>
#include <map>

class Forex {
    private:
    std::string base;
    std::string symbol;
    std::map<std::string, double> rates;

    public:
    Forex(std::string base, std::string symbol);
    ~Forex();

    double getRate(std::string date);

    void printRates();
    void clearRates();

    void getHistoricalRates(const char *period1, const char *period2);
};

#endif /* FOREX_HPP */
