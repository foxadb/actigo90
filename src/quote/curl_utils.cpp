#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <curl/curl.h>

#include "curl_utils.hpp"
#include "json.hpp"

using namespace std;
using namespace nlohmann;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void getCrumbCookie(string url,
                    string *crumb,
                    string *cookie) {
    CURL* curl = curl_easy_init();
    string readBuffer;

    const char* cookieFile = "/tmp/yahoo-finance-cookie";

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR,  cookieFile);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Write result into the string buffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Cleanup
        curl_easy_cleanup(curl);
    }

    *crumb = extractCrumb(readBuffer);
    *cookie = extractCookie(cookieFile);
}

string extractCrumb(string code) {
    int i = 0;
    string line;
    istringstream codestream(code);
    while (getline(codestream, line) && i < 42) {
        ++i;
    }

    line = line.substr(16, line.size() - 17);

    auto json = json::parse(line);

    string crumb = json["context"]["dispatcher"]["stores"]["CrumbStore"]["crumb"];
    return crumb;
}

string extractCookie(const char *cookieFile) {
    int i = 0;
    string line;
    ifstream file(cookieFile);

    if (file.is_open()) {
        while (getline(file, line) && i < 4) {
            ++i;
        }
        file.close();
    }

    string cookie = "B=" + line.substr(37);
    return cookie;
}

void sendDataRequest(string symbol,
                     int period1,
                     int period2,
                     string interval,
                     string *crumb,
                     string *cookie) {
    string url = "https://query1.finance.yahoo.com/v7/finance/download/"
            + symbol
            + "?period1=" + to_string(period1)
            + "&period2=" + to_string(period2)
            + "&interval=" + interval
            + "&events=history"
            + "&crumb=" + *crumb;

    CURL* curl = curl_easy_init();
    CURLcode res;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookie->c_str());

        // Perform the request
        res = curl_easy_perform(curl);

        // Cleanup
        curl_easy_cleanup(curl);
    }
}

CURLcode getUrlData(string url) {
    CURL *curl = curl_easy_init();
    CURLcode result;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return result;
}
