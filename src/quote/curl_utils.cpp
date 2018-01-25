#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <curl/curl.h>

#include "json.hpp"
#include "curl_utils.hpp"

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

double getForexRate(std::string date, std::string base, std::string symbol) {
    std::cerr << "getForexRate: DEPRECATED" << std::endl;

    std::string url = "https://api.fixer.io/" + date
            + "?base=" + base
            +  "&symbols=" + symbol;

    CURL* curl = curl_easy_init();
    std::string responseBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Write result into the buffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Cleanup
        curl_easy_cleanup(curl);
    }

    std::cout << responseBuffer << std::endl;
    auto json = nlohmann::json::parse(responseBuffer);

    double rate = json["rates"][symbol];
    return rate;
}

double* getForexRates(std::string date,
                      std::string base,
                      std::string *symbol,
                      int nbSymbols) {
    std::string url = "https://api.fixer.io/" + date
            + "?base=" + base
            +  "&symbols=";

    for (int i = 0; i < nbSymbols; ++i) {
        url += symbol[i] + ",";
    }

    CURL* curl = curl_easy_init();
    std::string responseBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Write result into the buffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Cleanup
        curl_easy_cleanup(curl);
    }

    auto json = nlohmann::json::parse(responseBuffer);

    double* rates = new double[nbSymbols];
    for (int i = 0; i < nbSymbols; ++i) {
        rates[i] = json["rates"][symbol[i]];
    }
    return rates;
}

void getYahooCrumbCookie(std::string url,
                         std::string *crumb,
                         std::string *cookie) {
    CURL* curl = curl_easy_init();
    std::string responseBuffer;

    const char* cookieFile = "/tmp/yahoo-finance-cookie";

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR,  cookieFile);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Write result into the buffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Cleanup
        curl_easy_cleanup(curl);
    }

    *crumb = extractYahooCrumb(responseBuffer);
    *cookie = extractYahooCookie(cookieFile);
}

std::string extractYahooCrumb(std::string code) {
    // Find the root app main json
    size_t root = code.find("root.App.main");

    // Create the start of JSON String
    std::string jsonString = code.substr(root + 16);

    // Find the last brace (at the end of line)
    size_t lastBrace = jsonString.find("\n") - 1;

    // Cut the JSON end and parse it correctly
    jsonString = jsonString.substr(0, lastBrace);
    auto json = nlohmann::json::parse(jsonString);

    // Extract the crumb
    std::string crumb = json["context"]["dispatcher"]["stores"]["CrumbStore"]["crumb"];
    return crumb;
}

std::string extractYahooCookie(const char* cookieFile) {
    int i = 0;
    std::string line;
    std::ifstream file(cookieFile);

    if (file.is_open()) {
        while (std::getline(file, line) && i < 4) {
            ++i;
        }
        file.close();
    }

    std::string cookie = "B=" + line.substr(37);
    return cookie;
}

std::string downloadYahooCsv(std::string symbol,
                             std::time_t period1,
                             std::time_t period2,
                             std::string interval,
                             std::string *crumb,
                             std::string *cookie) {
    std::string url = "https://query1.finance.yahoo.com/v7/finance/download/"
            + symbol
            + "?period1=" + std::to_string(period1)
            + "&period2=" + std::to_string(period2)
            + "&interval=" + interval
            + "&events=history"
            + "&crumb=" + *crumb;

    CURL* curl = curl_easy_init();
    std::string responseBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookie->c_str());

        // Write result into the buffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Cleanup
        curl_easy_cleanup(curl);
    }

    return responseBuffer;
}
