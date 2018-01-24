#include <ctime>

std::time_t currentEpoch();

std::time_t dateToEpoch(const char *date);

std::string epochToDate(const std::time_t epoch);

bool before(const char *date1, const char *date2);
