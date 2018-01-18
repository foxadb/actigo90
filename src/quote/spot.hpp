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
    ~Spot();

    std::string getDate();
    double getOpen();
    double getHigh();
    double getLow();
    double getClose();

    std::string toString();

    void printSpot();
};
