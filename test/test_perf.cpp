#include <cstdlib>
#include "MonteCarlo.hpp"
#include "Performance.hpp"
#include "BlackScholesModel.hpp"
#include <ctime>
#include <iostream>

using namespace std;


int main(int argc, char** argv) {
  //float strike = 100.0;
  int size = 5;
  PnlVect* spotVector = pnl_vect_create_from_scalar(size,100);
  PnlVect* perfWeights = pnl_vect_create_from_scalar(size,0.2);
  double maturity = 2;
  int nbSteps = 12;
  Performance* myPerf = new Performance(maturity,nbSteps,size,perfWeights);
  //Basket *myBasket = new Basket(maturity,nbSteps,size,strike,basketWeights);
  double intresetRate = 0.03;
  double rho = 0.5;
  PnlVect* volatVector = pnl_vect_create_from_scalar(size,0.2);
  BlackScholesModel *bsm = new BlackScholesModel(size,intresetRate,rho,volatVector,spotVector);
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  double prix;
  double ic;
  int nbSamples = 50000;
  MonteCarlo *mc = new MonteCarlo(bsm,myPerf,rng,0.1,nbSamples);
  mc->price(prix,ic);
  cout << " hello" <<endl;
  cout << prix << endl;
  cout << ic << endl; 
  return 0;
}
