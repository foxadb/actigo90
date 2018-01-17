#include <iostream>
#include <string>

#include "quote.hpp"

int main(int argc, char* argv[]) {
    std::string symbol(argv[1]);

    Quote quote = Quote(symbol);

    quote.getHistoricalData(1504710156, 1516184887, "1wk");
}
