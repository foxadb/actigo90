#pragma once

#include "Option.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"

/// \brief Classe pour simulation MonteCarlo
class MonteCarlo {
public:
    BlackScholesModel *mod_; /*! pointeur vers le modèle */
    Option *opt_; /*! pointeur sur l'option */
    PnlRng *rng_; /*! pointeur sur le générateur */
    double fdStep_; /*! pas de différence finie */
    int nbSamples_; /*! nombre de tirages Monte Carlo */
    PnlMat *path;   /*! matrice contenant la trajectoire */
    PnlMat *shiftPath; /*! matrice contenant la trajectoire shiftée */

    /**
     * Calcule le prix de l'option à la date 0
     *
     * @param[out] prix valeur de l'estimateur Monte Carlo
     * @param[out] ic largeur de l'intervalle de confiance
     */
    void price(double &prix, double &ic);

    /**
     * Calcule le prix de l'option à la date t
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du prix
     */
    void price(const PnlMat *past, double t, double &prix, double &ic);

    /**
     * Calcule le delta de l'option à la date t
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] delta contient le vecteur de delta
     * de confiance sur le calcul du delta
     */
    void delta(const PnlMat *past, double t, PnlVect *delta);

      /**
     * Calcule le delta de l'option à la date t pour actigo
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] delta contient le vecteur de delta
     * de confiance sur le calcul du delta
     */

    void delta(const PnlMat *past, double t, PnlVect *delta, double rDoll, double rAusDoll);

     /**
     * Calcule la P&L de l'option
     *
     * @param[in] data contient la trajectoire du sous-jacent
     * sur toute la subdivision de rebalancement
     */
    double pAndL(PnlMat *data);


     /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe MonteCarlo
     *
     *  \param mod       : un BlackScholesModel pour générer la trajectoire des actifs
     *  \param opt       : option à pricer/couvrir
     *  \param rng       : générateur de nombres aléatoires
     *  \param fdStep    : pas de différentiation
     *  \param nbSamples : nombre de simulations MonteCarlo
     */
    MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, int nbSamples);

     /*!
     *  \brief Destructeur
     *
     *  Destructeur de la classe MonteCarlo
     */
    virtual ~MonteCarlo();


    void updatePast(PnlMat *past, PnlMat *data, int i);

    
};


