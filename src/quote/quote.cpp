#include <iostream>
#include <curl/curl.h>

#include "quote.hpp"
#include "curl_utils.hpp"

Quote::Quote(std::string symbol) {
    this->symbol = symbol;
}

void Quote::getHistoricalData(int period1, int period2, const char *interval) {
    std::string url = "https://finance.yahoo.com/quote/"
        + this->symbol
        + "/?p="
        + this->symbol;

    std::string *crumb = new std::string;
    std::string *cookie = new std::string;

    getCrumbCookie(url, crumb, cookie);

    sendDataRequest(this->symbol,
                    period1,
                    period2,
                    interval,
                    crumb,
                    cookie);
}
