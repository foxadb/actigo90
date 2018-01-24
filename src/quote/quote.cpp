#include <iostream>
#include <string>
#include <sstream>
#include <curl/curl.h>
#include <ctime>
#include <boost/algorithm/string.hpp>

#include "quote.hpp"
#include "time_utils.hpp"
#include "curl_utils.hpp"

Quote::Quote(std::string symbol) {
    this->symbol = symbol;
}

Quote::~Quote() {}

size_t Quote::nbSpots() {
    return this->spots.size();
}

Spot Quote::getSpot(size_t i) {
    if (i < this->spots.size()) {
        return this->spots[i];
    }

    std::string error = "ERROR: getSpot(index) - Index must be smaller than "
            + std::to_string(this->spots.size());
    throw std::invalid_argument(error);
}

Spot Quote::getSpot(std::string date) {
    for (std::vector<Spot>::iterator it = this->spots.begin();
         it != this->spots.end();
         ++it) {
        if (it->getDate() == date) {
            return *it;
        }
    }

    std::string error = "ERROR getSpot(date) - There is not spot at " + date;
    throw std::invalid_argument(error);
}


void Quote::printSpots() {
    for (std::vector<Spot>::iterator it = this->spots.begin();
         it != this->spots.end();
         ++it) {
        std::cout << std::endl << it->toString();
    }
    std::cout << std::endl;
}


void Quote::clearSpots() {
    this->spots.clear();
}

std::string Quote::getHistoricalCsv(std::time_t period1,
                                    std::time_t period2,
                                    const char *interval) {
    std::string url = "https://finance.yahoo.com/quote/"
            + this->symbol
            + "/?p="
            + this->symbol;

    std::string *crumb = new std::string;
    std::string *cookie = new std::string;

    // Get the Crumb and Cookie from Yahoo Finance
    getYahooCrumbCookie(url, crumb, cookie);

    // Download the historical prices Csv
    std::string csv = downloadYahooCsv(
                this->symbol, period1, period2, interval, crumb, cookie);

    // Free memory
    delete crumb;
    delete cookie;

    // Return the csv
    return csv;

}

void Quote::getHistoricalSpots(std::time_t period1,
                               std::time_t period2,
                               const char *interval) {
    // Download the historical prices Csv
    std::string csv = this->getHistoricalCsv(period1, period2, interval);
    std::istringstream csvStream(csv);
    std::string line;

    // Remove the header line
    std::getline(csvStream, line);

    while (std::getline(csvStream, line)) {
        std::vector<std::string> data;
        boost::split(data, line, boost::is_any_of(","));

        if (data[0] != "null" && data[1] != "null") {
            Spot spot = Spot(
                    data[0],                // date
                    std::stod(data[1]),     // open
                    std::stod(data[2]),     // high
                    std::stod(data[3]),     // low
                    std::stod(data[4])      // close
                    );

            this->spots.push_back(spot);
        }
    }
}

void Quote::getHistoricalSpots(const char *date1,
                               const char *date2,
                               const char *interval) {
    std::time_t period1 = dateToEpoch(date1);
    std::time_t period2 = dateToEpoch(date2);

    this->getHistoricalSpots(period1, period2, interval);
}
