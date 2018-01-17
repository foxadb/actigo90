#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <curl/curl.h>

void getCrumbCookie(std::string url,
                    std::string *crumb,
                    std::string *cookie);

std::string extractCrumb(std::string code);

std::string extractCookie(const char *cookieFile);

void sendDataRequest(std::string symbol,
                     int period1,
                     int period2,
                     std::string interval,
                     std::string *crumb,
                     std::string *cookie);

CURLcode getUrlData(std::string url);

#endif /* UTILS_HPP */
