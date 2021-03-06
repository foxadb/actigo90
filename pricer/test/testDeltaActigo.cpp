#include <cstdlib>
#include "MonteCarlo.hpp"
#include "BlackScholesModel.hpp"
#include <ctime>
#include <iostream>
#include "Actigo.hpp"


using namespace std;


int main(int argc, char** argv) {

  float temps;
  clock_t t1, t2;
  t1 = clock();
  double rEur = 0.0075;
  double rUsd = 0.028;
  double rAud = 0.026;
  PnlMat *data = pnl_mat_create(450, 5);
  PnlVect *sigma = pnl_vect_create_from_scalar(5, 0.2);
  PnlVect *spot = pnl_vect_create_from_scalar(5, 1.0);
  LET(spot,3) = 0.04;
  LET(spot, 4) = 0.04;
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));;
  BlackScholesModel *bsm = new BlackScholesModel(5, rEur, 0.0, sigma, spot);
  bsm->trend_ = pnl_vect_create_from_scalar(5, 0.04);
  bsm->simul_market(data, 450, 3.0, rng);
  Actigo *actigo = new Actigo(1.5, 450,5, 1., 1., 1., rEur, rUsd, rAud);
  MonteCarlo *monteCarlo = new MonteCarlo(bsm, actigo, rng, 0.1,200);
  PnlVect *delta = pnl_vect_create_from_scalar(5, 0.0);
  double price, ic;
  double pL = monteCarlo->pAndL(data);
  cout << "PL with simulated Data: " << pL << endl;
  pnl_vect_free(&sigma);
  pnl_vect_free(&spot);
  pnl_rng_free(&rng);
  pnl_mat_free(&data);
  delete actigo;
  delete bsm;
  delete monteCarlo;



  return 0;
}
