/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Performance.cpp
 * Author: elfilalz
 * 
 * Created on September 19, 2017, 11:09 AM
 */

#include "Performance.hpp"

Performance::Performance() {
}

Performance::Performance(const Performance& orig) {
}

Performance::~Performance() {
    pnl_vect_free(&coefficients_);
    pnl_vect_free(&spot);
    pnl_vect_free(&spotAvant);
}

Performance::Performance(double T, int nbTimeSteps, int size, PnlVect *coefficients){
    T_ = T;
    nbTimeSteps_ = nbTimeSteps;
    size_ = size;
    coefficients_ = pnl_vect_new();
    spot = pnl_vect_create(size_);
    spotAvant = pnl_vect_create(size_);
    pnl_vect_clone(coefficients_,coefficients);
}

double Performance::payoff(const PnlMat *path){
    double payoff = 1.0;
    for (int i=1; i<nbTimeSteps_+1; i++) {
      pnl_mat_get_row(spot,path,i);
      pnl_mat_get_row(spotAvant,path,i-1);
      payoff += (pnl_vect_scalar_prod(coefficients_,spot) / pnl_vect_scalar_prod(coefficients_,spotAvant) > 1)?(pnl_vect_scalar_prod(coefficients_,spot) / pnl_vect_scalar_prod(coefficients_,spotAvant) - 1):0;
}
    return payoff;
}
