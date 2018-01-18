#include "Basket.hpp"
#include <iostream>

using namespace std;

Basket::Basket() {
}

Basket::Basket(const Basket& orig) {
}

Basket::~Basket() {
    pnl_vect_free(&coefficients);
    pnl_vect_free(&spotFinal);
}

Basket::Basket(double t, int nbSteps, int size, float str, PnlVect *coeff) {
    T_ = t;
    nbTimeSteps_ = nbSteps;
    size_ = size;
    strike = str;
    coefficients = pnl_vect_new();
    pnl_vect_clone(coefficients, coeff);
    spotFinal = pnl_vect_create(size_);
}

double Basket::payoff(const PnlMat* path) {
    pnl_mat_get_row(spotFinal, path, path->m - 1);
    double payoff = pnl_vect_scalar_prod(coefficients, spotFinal);
    return (payoff - strike > 0) ? (payoff - strike) : 0;
}


