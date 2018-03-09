#include "Option.hpp"

#ifndef BASKET_H
#define	BASKET_H

/// \brief Classe pour option panier
class Basket : public Option {
public:
    float strike; // prix d'exercice de l'option
    PnlVect *coefficients; // coefficients de pondération de chaque actif
    PnlVect *spotFinal; // vecteur contenant la valeur du sous-jacent le jour de la maturité


    /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe Basket
     *
     *  \param t       : maturité de l'option
     *  \param nbSteps : nombre de cours dont on a besoin pour calculer le payoff
     *  \param size    : nombre d'actifs composant l'option
     *  \param str     : prix d'exercice
     *  \param coeff   : vecteur des coefficients de pondération
     */
    Basket(double t, int nbSteps, int size, float str, PnlVect *coeff);

    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    double payoff(const PnlMat *path);


    /*!
     *  \brief Destructeur
     *
     *  Destructeur de la classe Basket
     */
    virtual ~Basket();

    Basket();

    Basket(const Basket& orig);
};

#endif	/* BASKET_H */
