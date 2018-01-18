#include <iostream>
#include <string>

#include "quote.hpp"
#include "curl_utils.hpp"

int main(int argc, char* argv[]) {
    // Get the symbol and create the quote
    std::string symbol(argv[1]);
    Quote* quote = new Quote(symbol);

    // Get the historical spots from Yahoo Finance
    quote->getHistoricalSpots(argv[2], argv[3], argv[4]);

    // Print the spots
    quote->printSpots();

    // Get the EUR/AUD and EUR/USD rates
    std::string symbols[] = { "AUD", "USD" };
    double *rates = getForexRates("2018-01-10", "EUR", symbols, 2);
    std::cout << "EUR/AUD 2017-01-10: " << rates[0] << std::endl;
    std::cout << "EUR/USD 2017-01-10: " << rates[1] << std::endl;

    // Free memory
    delete quote;
    delete rates;
}
