#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Actigo.hpp"
#include "Option.hpp"
#include "Data.hpp"
#include "Calibration.hpp"
#include <string>
#include "../quote/time_utils.hpp"
#include <iostream>

using namespace std;


int main(int argc, char **argv){
  time_t todayEpoch = currentEpoch();
  Actigo *actigo = new Actigo(1., 365, 5, 100., 100., 100.);
  PnlVect *sigma = pnl_vect_create_from_scalar(5, 0.5);
  PnlVect *spots = pnl_vect_create_from_scalar(5, 100.);
  PnlVect *trends = pnl_vect_create_from_scalar(5, 0.04);
  BlackScholesModel *bsm = new BlackScholesModel(5, 0.04, 0.2, sigma,spots);
  bsm->trend_ = trends;
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  PnlMat* dataMatrix = pnl_mat_create_from_scalar(365,5, 0.0);
  bsm->simul_market(dataMatrix, 365, 1., rng);
  Data *data = new Data(dataMatrix);
  double step = actigo->T_ / actigo->nbTimeSteps_;
  Calibration *calibration = new Calibration(data, step);
  PnlMat* correlations = pnl_mat_create(5,5);
  pnl_mat_clone(correlations, calibration->getCorrelationsMatrix());
  cout << "this is the correlations matrix" << endl;
  pnl_mat_print(correlations);
  cout << "this is the volatility vector" << endl;
  PnlVect *vols= calibration->getVolatilities();
  pnl_vect_print(vols);

  return 0;
}
