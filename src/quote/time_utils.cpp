#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

std::time_t currentEpoch() {
    return std::time(nullptr);
}

std::time_t dateToEpoch(const char *date) {
    char year[4];
    char month[3];
    char day[3];

    strncpy(year, date, 4);
    strncpy(month, date + 5, 2);
    strncpy(day, date + 8, 2);

    struct tm time = { 0 };
    time.tm_year = std::atoi(year) - 1900;
    time.tm_mon = std::atoi(month) - 1;
    time.tm_mday = std::atoi(day);

    std::time_t epochTime = mktime(&time);

    return epochTime;
}
