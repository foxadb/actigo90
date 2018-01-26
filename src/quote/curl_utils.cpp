#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <curl/curl.h>

#include "json.hpp"
#include "curl_utils.hpp"
#include "time_utils.hpp"

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void saveYahooCredentials(const char* filename, std::string *crumb, std::string *cookie) {
    // Open cookie file
    std::ofstream file(filename);

    // Save update time, crumb and cookie
    file << currentEpoch() << std::endl;
    file << *crumb << std::endl;
    file << *cookie << std::endl;

    // Close cookie file
    file.close();
}

std::time_t readCookieTime(const char* filename) {
    // Open cookie file
    std::ifstream file(filename);

    // Read the last cookie update time
    std::time_t date;
    file >> date;

    return date;
}

void readCrumbCredential(const char* filename, std::string *crumb) {
    // Open cookie file
    std::ifstream file(filename);

    // Read the crumb
    file >> *crumb;
    *crumb = "";
    file >> *crumb;
}

void readCookieCredential(const char* filename, std::string *cookie) {
    // Open cookie file
    std::ifstream file(filename);

    // Read the cookie
    file >> *cookie >> *cookie;
    *cookie = "";
    file >> *cookie;
}

bool needNewCookie(const char* filename, std::time_t time) {
    std::time_t currentDate = currentEpoch();
    std::time_t cookieDate = readCookieTime(filename);
    return currentDate - cookieDate > time;
}

void getYahooCrumbCookie(std::string url,
                         std::string *crumb,
                         std::string *cookie) {
    const char* credentialFile = "/tmp/yahoo-finance-credentials";
    const char* cookieFile = "/tmp/yahoo-finance-cookie";

    // Download new cookie every 5 minutes
    if (needNewCookie(credentialFile, 300)) {
        CURL* curl = curl_easy_init();
        std::string responseBuffer;

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

        // Save crumb and cookie values
        saveYahooCredentials(credentialFile, crumb, cookie);
    } else {
        // Read crumb and cookie values from tmp file
        readCrumbCredential(credentialFile, crumb);
        readCookieCredential(credentialFile, cookie);
    }
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
