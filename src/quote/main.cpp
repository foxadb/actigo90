#include <iostream>
#include <string>

#include "quote.hpp"
#include "curl_utils.hpp"

int main() {
    // Euro Stoxx 50
    Quote* quote = new Quote("^STOXX50E");

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
    double *rates = getForexRates("2018-01-10", "EUR", symbols, 2);
    std::cout << "EUR/AUD 2018-01-10: " << rates[0] << std::endl;
    std::cout << "EUR/USD 2018-01-10: " << rates[1] << std::endl;

    // Free memory
    delete quote;
    delete rates;
}
