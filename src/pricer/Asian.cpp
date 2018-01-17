/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Asian.cpp
 * Author: elfilalz
 * 
 * Created on September 19, 2017, 10:39 AM
 */

#include "Asian.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;

Asian::Asian() {
}

Asian::Asian(const Asian& orig) {
}

Asian::~Asian() {
    pnl_vect_free(&coefficients_);
    pnl_vect_free(&spot);
}

Asian::Asian(double T, int nbTimeSteps, int size, float strike, PnlVect *coefficients) {
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
    strike_ = strike;
    coefficients_ = pnl_vect_new();
    pnl_vect_clone(coefficients_,coefficients);
    spot = pnl_vect_create(size_);
}

double Asian::payoff(const PnlMat *path){
  double payoff = 0.0;
  for ( int i = 0; i < nbTimeSteps_+1; i++ ) {
  pnl_mat_get_row(spot,path,i);
   payoff += pnl_vect_scalar_prod(coefficients_,spot);
  }
    payoff = payoff / ( nbTimeSteps_ + 1 );
  return (payoff - strike_ > 0)?(payoff - strike_):0;
}

