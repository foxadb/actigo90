#pragma once

#include "Option.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"

/**
* @brief MonteCarlo class
*/
class MonteCarlo {

public:
    BlackScholesModel *mod_; /*! pointer to the model */
    Option *opt_; /*! pointer to the option */
    PnlRng *rng_; /*! pointer to the generator */
    double fdStep_; /*! step of the finite difference discretization */
    int nbSamples_; /*! number of draws Monte Carlo */
    PnlMat *path;   /*! matrix conatining the path */
    PnlMat *shiftPath; /*! matrix containing the shifted path */

    /**
     * @brief Computes the option's price at 0
     *
     * @param[out] price given by the monte carlo estimation
     * @param[out] ic width of the confidence interval
     */
    void price(double &prix, double &ic);

    /**
     * @brief Computes the option's price at t
     *
     * @param[in]  past contains the underlying asset path until t
     * @param[in] t date at which we want to compute the price
     * @param[out] prix contains the price
     * @param[out] ic contains the width of the confidence interval
     * for the price estimation
     */
    void price(const PnlMat *past, double t, double &prix, double &ic);

    /**
     * @brief Computes the option's delta at t
     *
     * @param[in] past contains the path of the underlying asset
     * until t
     * @param[in] t date at which the comutation is done
     * @param[out] delta contains the delta vector
     */
    void delta(const PnlMat *past, double t, PnlVect *delta);

    /**
     * @brief Computes the Actigo's delta at t
     *
     * @param[in] past contains the path of the underlying asset
     * until t
     * @param[in] t date at which the comutation is done
     * @param[out] delta contains the delta vector
     */
    void delta(const PnlMat *past, double t, PnlVect *delta, double rDoll, double rAusDoll);

    /**
     * @brief Computes the P&L
     *
     * @param[in] data contains the underlying asset path
     * on the rebalance subdivision
     */
    double pAndL(PnlMat *data);


    /*!
     *  @brief MonteCarlo Constructor
     *
     *
     *  @param mod       : A BlackScholesModel to generate the underlying assets path
     *  @param opt       : Option to price/hedge
     *  \param rng       : Random numbers generator
     *  \param fdStep    : step of finite difference discretization
     *  \param nbSamples : number of simulations Monte Carlo
     */
    MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, int nbSamples);

    /*!
     *
     * @brief MonteCarlo destructor
     */
    virtual ~MonteCarlo();

    void updatePast(PnlMat *past, PnlMat *data, int i);
};
