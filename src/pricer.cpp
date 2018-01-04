#include <iostream>
#include <string>
#include "parser.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "Basket.hpp"
#include "Performance.hpp"
#include "Asian.hpp"
#include "Option.hpp"
#include <ctime>

using namespace std;

int main(int argc,char **argv){
  const char *infile = argv[1];
  string s = "-c";
  string d = "-d";
  PnlMat *past;
  const char* dat = s.c_str();
  const char* datSecond = d.c_str();
  bool test = strcmp(infile,datSecond);
  bool test1 = strcmp(infile,dat);

  if ( test && !test1){// this is the case where we have an option -c
    infile = argv[3];//data input
    char *marketData = argv[2];
    past = pnl_mat_create_from_file(marketData);
  }
  if (test1 && !test){
    infile = argv[2];
  }

  Param *P = new Parser(infile);
  double T, r, strike,corr;
  PnlVect *spot, *sigma, *divid,*weights;
  string type;
  int size,timeStepNumber,hedgingDatesNumber;
  size_t n_samples;
  Option *myOption;
  P->extract("option type", type);
  P->extract("maturity", T);
  P->extract("option size", size);
  P->extract("spot", spot, size);
  P->extract("volatility", sigma, size);
  P->extract("interest rate", r);
  if (P->extract("dividend rate", divid, size) == false){
    divid = pnl_vect_create_from_zero(size);
  }
  P->extract("sample number", n_samples);
  P->extract("timestep number",timeStepNumber);
  P->extract("payoff coefficients",weights,size);
  P->extract("correlation",corr);
  if (type =="basket"){
    P->extract("strike", strike);
    myOption = new Basket(T,timeStepNumber,size,strike,weights);
  }else if ( type == "asian"){
    P->extract("strike", strike);
    myOption = new Asian(T,timeStepNumber,size,strike,weights);
  }else if ( type == "performance"){
    myOption = new Performance(T,timeStepNumber,size,weights);
  }
  BlackScholesModel *bsm = new BlackScholesModel(size,r,corr,sigma,spot);
  PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
  pnl_rng_sseed(rng, time(NULL));
  if ( test1 && !test ){
    PnlVect *trend;
    P->extract("hedging dates number",hedgingDatesNumber);
    P->extract("trend",trend,size);
    bsm->trend_ = trend;
    past = pnl_mat_create(hedgingDatesNumber,size);
    bsm->simul_market(past,hedgingDatesNumber,T,rng);
  }
  MonteCarlo *mc = new MonteCarlo(bsm,myOption,rng,0.1,n_samples);
  if (test && !test1){
    double prix;
    double ic;
    float temps;
    clock_t t1,t2;
    t1= clock();
    mc->price(prix,ic);
    PnlVect *delta = pnl_vect_create_from_scalar(size,0.0);
    past = pnl_mat_create_from_scalar(1,size,pnl_vect_get(spot,0));
    mc->delta(past,0.0,delta);
    t2 = clock();
    temps = (float) (t2 - t1) / CLOCKS_PER_SEC;
    cout << "This is the time spent : " <<temps << endl;
    cout << "These are the deltas at t = 0 " << endl;
    pnl_vect_print(delta);
    cout << "This is the price of the option at 0: "<< prix << endl;
    cout <<"This is the confidence interval corresponding: "<< ic << endl;
    pnl_vect_free(&delta);
  }else{
    float temps;
    clock_t t1,t2;
    t1= clock();
    double pl =mc->pAndL(past);
    t2 = clock();
    temps = (float) (t2 - t1) / CLOCKS_PER_SEC;
    cout << "this is the time spent: " <<temps << endl;
    cout << "this is the tracking error : " << pl << endl;
  }
  pnl_mat_free(&past);
  pnl_rng_free(&rng);
  pnl_vect_free(&spot);
  pnl_vect_free(&sigma);
  pnl_vect_free(&divid);
  pnl_vect_free(&weights);
  delete P;
  delete mc;
  delete bsm;
  delete myOption;
  return 0;

}
