#include <iostream>
#include <string>

#include "quote.hpp"

int main(int argc, char* argv[]) {
    // Get the symbol and create the quote
    std::string symbol(argv[1]);
    Quote* quote = new Quote(symbol);

    // Get the historical spots from Yahoo Finance
    quote->getHistoricalSpots(argv[2], argv[3], argv[4]);

    // Print the spots
    quote->printSpots();

    // Free memory
    delete quote;
}
