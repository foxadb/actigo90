#ifndef OPTION_H
#define	OPTION_H

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/**
* @brief Option class
*/
class Option {
public:
    double T_; /// maturity
    int nbTimeSteps_; /// nbre of steps for the finite difference discretization
    int size_; /// Model dimension

    virtual ~Option() {}

    /**
     * @brief computes the payoff on the given path
     *
     * @param[in] path is a matrix of size (N+1) x d
     * containing a path of the model
     * @return phi(path)
     */
    virtual double payoff(const PnlMat *path) = 0;
};

#endif /* OPTION_H */
