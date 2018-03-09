#include <cstdlib>
#include "MonteCarlo.hpp"
#include "Basket.hpp"
#include "BlackScholesModel.hpp"
#include <ctime>
#include <iostream>

#include "pnl/pnl_vector.h"

using namespace std;


int main(int argc, char** argv) {
  float strike = 100.0;
  PnlVect* spotVector = pnl_vect_create_from_scalar(40,100);
  PnlVect* basketWeights = pnl_vect_create_from_scalar(40,0.025);
  double maturity = 3;
  int nbSteps = 5;
  int size = 40;
  Basket *myBasket = new Basket(maturity,nbSteps,size,strike,basketWeights);
  double intresetRate = 0.04879;
  double rho = 0.0;
  PnlVect* volatVector = pnl_vect_create_from_scalar(40,0.025);
  BlackScholesModel *bsm = new BlackScholesModel(size,intresetRate,rho,volatVector,spotVector);
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  double prix;
  double ic;
  int nbSamples = 50000;
  MonteCarlo *mc = new MonteCarlo(bsm,myBasket,rng,0.1,nbSamples);
  mc->price(prix,ic);
  cout << prix << endl;
  cout << ic << endl;
  pnl_vect_free(&spotVector);
  pnl_vect_free(&basketWeights);
  delete myBasket;
  pnl_vect_free(&volatVector);
  delete bsm;
  pnl_rng_free(&rng);
  delete mc;
  return 0;
}
