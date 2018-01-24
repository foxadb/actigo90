#include <iostream>
#include <string>

#include "quote.hpp"
#include "forex.hpp"
#include "curl_utils.hpp"
#include "time_utils.hpp"

int main() {
    // S&P 500
    Quote *quote = new Quote("^GSPC");

    // Get the historical spots from Yahoo Finance
    quote->getHistoricalSpots("2017-12-01", "2017-12-31", "1d");

    // Print the spots
    quote->printSpots();

    // Print a spot
    try {
        Spot spot = quote->getSpot("2017-12-01");
        std::cout << "Spot at 2017-12-01: " << std::endl;
        spot.printSpot();
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    // Get the EUR/AUD and EUR/USD rates
    std::string symbols[] = { "AUD", "USD" };
    double *rates = getForexRates("2018-01-22", "EUR", symbols, 2);
    std::cout << "EUR/AUD 2018-01-10: " << rates[0] << std::endl;
    std::cout << "EUR/USD 2018-01-10: " << rates[1] << std::endl;

    // Get the historical EUR/USD rates
    Forex* forex = new Forex("EUR", "USD");
    forex->getHistoricalSpots("2018-01-01", "2018-01-19");
    forex->printSpots();

    // Free memory
    delete quote;
    delete rates;
}
