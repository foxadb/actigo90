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
  PnlMat *data = pnl_mat_create(450, 3);

  PnlVect *sigma = pnl_vect_create_from_scalar(3, 0.2);
  PnlVect *spot = pnl_vect_create_from_scalar(3, 100.0);
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  PnlVect* basketWeights = pnl_vect_create_from_scalar(3,0.33);
  BlackScholesModel *bsm = new BlackScholesModel(3, 0.04879, 0.0, sigma, spot);
  bsm->trend_ = pnl_vect_create_from_scalar(5, 0.04);
  bsm->simul_market(data, 450, 3.0, rng);
  Actigo *actigo = new Actigo(1.5, 150,3, 100., 100., 100.);
  //Basket *actigo = new Basket(1.0,365,3,100,basketWeights);
  //Asian* actigo = new Asian(1.5,50,3,100.0,basketWeights);
  MonteCarlo *monteCarlo = new MonteCarlo(bsm, actigo, rng, 0.1,200);
  PnlMat *past = pnl_mat_create_from_scalar(1, 5, 100.0);
  PnlVect *delta = pnl_vect_create_from_scalar(5, 0.0);
  double price, ic;
  monteCarlo->delta(past,0.0,delta);
  //monteCarlo->delta(past,0.0,delta);
  //double pL = monteCarlo->pAndL(data);
  pnl_vect_print(delta);
  //cout << "PL" << pL;
  pnl_vect_free(&sigma);
  pnl_vect_free(&spot);
  pnl_rng_free(&rng);
  pnl_mat_free(&data);
  delete actigo;
  delete bsm;
  delete monteCarlo;
    


  return 0;
}
