#include <cstdlib>
#include "MonteCarlo.hpp"
#include "Asian.hpp"
#include "BlackScholesModel.hpp"
#include <ctime>
#include <iostream>

using namespace std;


int main(int argc, char** argv) {
  float strike = 100.0;
  int size = 2;
  PnlVect* spotVector = pnl_vect_create_from_scalar(size,100);
  PnlVect* weights = pnl_vect_create_from_scalar(size,0.5);
  double maturity = 1.5;
  int nbSteps = 150;
  Asian* myAsian = new Asian(maturity,nbSteps,size,strike,weights);
  double intresetRate = 0.02;
  double rho = 0.0;
  PnlVect *sigma = pnl_vect_create_from_scalar(size,0.2);
  BlackScholesModel *bsm = new BlackScholesModel(size,intresetRate,rho,sigma,spotVector);
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  double prix;
  double ic;
  int nbSamples = 50000;
  MonteCarlo *mc = new MonteCarlo(bsm,myAsian,rng,0.1,nbSamples);
  mc->price(prix,ic);
  cout << " hello" <<endl;
  cout << prix << endl;
  cout << ic << endl; 
  pnl_vect_free(&spotVector);
  pnl_vect_free(&weights);
  delete myAsian;
  pnl_vect_free(&sigma);
  delete bsm;
  pnl_rng_free(&rng);
  delete mc;
  
  return 0;
}
