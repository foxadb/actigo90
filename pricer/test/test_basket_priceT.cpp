#include "MonteCarlo.hpp"
#include "Basket.hpp"
#include "BlackScholesModel.hpp"
#include <ctime>
#include <iostream>

using namespace std;


int main(int argc, char** argv) {
  float strike = 100.0;
  PnlVect* spotVector = pnl_vect_create_from_scalar(40,100);
  PnlVect* basketWeights = pnl_vect_create_from_scalar(40,0.025);
  double maturity = 3;
  double pricingDate = 1;
  int nbSteps = 5;
  int size = 40;
  Basket *myBasket = new Basket(maturity-pricingDate,nbSteps,size,strike,basketWeights);
  Basket *myBasket2 = new Basket(maturity,nbSteps,size,strike,basketWeights);
  double intresetRate = 0.04879;
  double rho = 0.0;
  PnlVect* volatVector = pnl_vect_create_from_scalar(40,0.025);
  BlackScholesModel *bsm = new BlackScholesModel(size,intresetRate,rho,volatVector,spotVector);
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  double prix1;
  double ic1;
  double prix2;
  double ic2;
  int nbSamples = 50000;
  PnlMat* past = pnl_mat_create_from_scalar(1,40,100.0);
  MonteCarlo *mc = new MonteCarlo(bsm,myBasket,rng,0.1,nbSamples);
  MonteCarlo *mc2 = new MonteCarlo(bsm,myBasket2,rng,0.1,nbSamples);
  mc2->price(past,pricingDate,prix2,ic2);
  mc->price(prix1,ic1);
  cout << prix1 << endl;
  cout << prix2 << endl;
  cout << ic1 << endl; 
  cout << ic2 << endl; 
  return 0;
}
