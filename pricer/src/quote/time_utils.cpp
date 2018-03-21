#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <bsoncxx/types.hpp>
#include <chrono>



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
    osYear << ptm->tm_year + 1900;

    std::stringstream osMonth;
    if (ptm->tm_mon < 9) {
        osMonth << 0 << ptm->tm_mon + 1;
    } else {
        osMonth << ptm->tm_mon + 1;
    }

    std::stringstream osDay;
    if (ptm->tm_mday < 10) {
        osDay << 0 << ptm->tm_mday;
    } else {
        osDay << ptm->tm_mday;
    }

    std::string date = osYear.str()+ "-" + osMonth.str() + "-" + osDay.str();
    return date;
}

bool before(const char *date1, const char *date2) {
    std::time_t epoch1 = dateToEpoch(date1);
    std::time_t epoch2 = dateToEpoch(date2);

    return epoch1 < epoch2;
}

bsoncxx::types::b_date read_date(std::time_t date, std::int32_t offset_from_utc)
{
  std::chrono::system_clock::time_point time_point = std::chrono::system_clock::from_time_t(date);
  return bsoncxx::types::b_date(time_point + std::chrono::hours{offset_from_utc});
}

std::time_t bDateToEpoch(bsoncxx::types::b_date date){
  std::chrono::system_clock::time_point time_point = std::chrono::system_clock::time_point(date);
  std::time_t time = std::chrono::system_clock::to_time_t(time_point);
  return time;
}
