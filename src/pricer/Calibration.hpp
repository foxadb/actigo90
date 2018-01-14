#include <cstdio>
#include <vector>

using namespace std;
class Calibration{
public:
  vector<double> index_1;
  vector<double> index_2;
  vector<double> index_3;

  double corr(vector<double> x, vector<double> y);
  double volatility(vector<double> index);
};
