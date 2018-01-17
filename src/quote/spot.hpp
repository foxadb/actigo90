#include <string>

class Spot {
    private:
    std::string date;
    double open;
    double high;
    double low;
    double close; 

    public:
    Spot(std::string date, double open, double high, double low, double close);

};