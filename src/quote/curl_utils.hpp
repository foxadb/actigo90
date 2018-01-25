#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <ctime>
#include <curl/curl.h>

void saveYahooCredentials(const char* filename, std::string *crumb, std::string *cookie);

std::time_t readCookieTime(const char* filename);

void readCrumbCredential(const char* filename, std::string *crumb);

void readCookieCredential(const char* filename, std::string *cookie);

void getYahooCrumbCookie(std::string url, std::string *crumb, std::string *cookie);

std::string extractYahooCrumb(std::string code);

std::string extractYahooCookie(const char* cookieFile);

std::string downloadYahooCsv(std::string symbol,
                             std::time_t period1,
                             std::time_t period2,
                             std::string interval,
                             std::string *crumb,
                             std::string *cookie);

#endif /* UTILS_HPP */
