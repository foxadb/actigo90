#ifndef BLACKSCHOLESMODEL_H
#define	BLACKSCHOLESMODEL_H

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/**
* @brief BlackScholesModel class
*/

class BlackScholesModel {
public:
    int size_; /// number of assets in the model
    double r_; /// interest rate
    double rho_; /// correlation parameter
    PnlVect *sigma_; /// volatilities vector
    PnlVect *spot_; /// initial values of the underlying assets
    PnlVect *trend_; /// model trend
    PnlMat *gamma; /// covariance matrix
    PnlVect *g; /// vector of iid samples from the normal centered reduced law

    // vectors created in constructor to gain  in performance
    PnlVect *newSpot;
    PnlVect *produit;
    PnlVect *coeff;
    PnlVect *pastVect;
    PnlVect *sigmaCarre;
    double(*ptr)(double);

    /**
     * @brief BlackScholesModel destructor
     */
    ~BlackScholesModel();

    /**
     * @brief Generates a path of the model and stocks it in a matrix path
     * @param[out] path contains a path of the model
     * its a matrix of size (nbTimeSteps+1) x d
     * @param[in] T  maturity
     * @param[in] nbTimeSteps number of observation dates
     * @param[in] rng Random PnlVect
     */
    void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

    /**
     * @brief Calculates a path of the underlying asset knowing the until the date t
     * @param[out] path  contains a path of the underlying asset
     * given by the matrix past until the date t
     * @param[in] t date until which we know the path.
     * t is not necessarily a date of discretization
     * @param[in] nbTimeSteps number of observation dates
     * @param[in] T date until which we simulate the path
     * @param[in] rng Random PnlVect
     * @param[in] past path known until the date t
     */
    void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past);

    /**
     * @brief Shift a path of the underlying asset
     * @param[in]  path contains in input the path
     * of the underlying asset
     * @param[out] shift_path contient a path for which the d^th
     * component has been shifted by (1+h) starting the date t.
     * @param[in] t date from which we start shifting.
     * @param[in] h the step of the finite difference discretization
     * @param[in] d index of the underlying asset to be shifted
     * @param[in] timestep step of observation for the underlying asset.
     */
    void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep);

    /**
     * @brief BlackScholesModel constructor
     * @param size : number of underlying assets
     * @param r    :  interest rate
     * @param rho  : correlation coefficient
     * @param sigma: standard deviation vector
     * @param spot : initial spots vector
     */
    BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot);

    /**
     *
     * @brief BlackScholesModel constructor
     * @param size : number of underlying assets.
     * @param r    : interest rate
     * @param correlations: correlations matrix
     * @param sigma: standard deviation vector
     * @param spot : initial spots vector
     */
    BlackScholesModel(int size, double r, PnlMat *correlations, PnlVect *sigma, PnlVect *spot);

    /**
     * @brief Generates a path of the model with the trend and stocks it
     * @param[out]  path contains a path of  underlying asset
     * @param[in] nbDates number of observation dates
     * @param[in] T option maturity
     * @param[in] rng random numbers generator
     */
    void simul_market(PnlMat* path, int nbDates, double T, PnlRng* rng);
};

#endif /* BLACKSCHOLESMODEL_H */
