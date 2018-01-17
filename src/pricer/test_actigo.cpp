#include <cstdlib>
#include "MonteCarlo.hpp"
#include "Asian.hpp"
#include "BlackScholesModel.hpp"
#include <ctime>
#include <iostream>
#include "Actigo.hpp"
#include "Basket.hpp"

using namespace std;


int main(int argc, char** argv) {

  float temps;
  clock_t t1, t2;
  t1 = clock();
  PnlMat *data = pnl_mat_create(2000, 3);

  PnlVect *sigma = pnl_vect_create_from_scalar(3, 0.2);
  PnlVect *spot = pnl_vect_create_from_scalar(3, 100.0);
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  PnlVect* basketWeights = pnl_vect_create_from_scalar(3,0.33);
  BlackScholesModel *bsm = new BlackScholesModel(3, 0.04879, 0.0, sigma, spot);
  bsm->trend_ = pnl_vect_create_from_scalar(3, 0.04);
  bsm->simul_market(data, 2000, 3.0, rng);
  Actigo *actigo = new Actigo(7.0, 400, 3, 100., 100., 100.);
  //Basket *actigo = new Basket(7.0,1,3,100,basketWeights);
  MonteCarlo *monteCarlo = new MonteCarlo(bsm, actigo, rng, 0.1,100);
  PnlMat *past = pnl_mat_create_from_scalar(1, 3, 100.0);
  PnlVect *delta = pnl_vect_create_from_scalar(3, 0.0);
  monteCarlo->delta(past,0.0,delta);
  double pL = monteCarlo->pAndL(data);
  //pnl_vect_print(delta);
  cout << "PL" << pL;
  pnl_vect_free(&sigma);
  pnl_vect_free(&spot);
  pnl_rng_free(&rng);
  pnl_mat_free(&data);
  delete actigo;
  delete bsm;
  delete monteCarlo;
    


  return 0;
}
