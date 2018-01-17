#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <curl/curl.h>

#include "curl_utils.hpp"
#include "json.hpp"

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void getCrumbCookie(std::string url,
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

    *crumb = extractCrumb(responseBuffer);
    *cookie = extractCookie(cookieFile);
}

std::string extractCrumb(std::string code) {
    int i = 0;
    std::string line;
    std::istringstream codestream(code);
    while (std::getline(codestream, line) && i < 42) {
        ++i;
    }

    line = line.substr(16, line.size() - 17);

    auto json = nlohmann::json::parse(line);

    std::string crumb = json["context"]["dispatcher"]["stores"]["CrumbStore"]["crumb"];
    return crumb;
}

std::string extractCookie(const char *cookieFile) {
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

std::string downloadCsv(std::string symbol,
                     int period1,
                     int period2,
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
