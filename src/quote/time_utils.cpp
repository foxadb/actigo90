#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstring>

std::time_t currentEpoch() {
    return std::time(NULL);
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

std::string epochToDate(const std::time_t epoch) {
    struct std::tm * ptm = std::localtime(&epoch);

    std::stringstream osYear; 
    osYear << ptm->tm_year;
    std::stringstream osMonth; 
    osMonth << ptm->tm_mon;
    std::stringstream osDay; 
    osDay << ptm->tm_mday;
    std::string date = osYear.str()+ "-" + osMonth.str() + "-" + osDay.str();
    return date;
}

bool before(const char *date1, const char *date2) {
    std::time_t epoch1 = dateToEpoch(date1);
    std::time_t epoch2 = dateToEpoch(date2);

    return epoch1 < epoch2;
}
