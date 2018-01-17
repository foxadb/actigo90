/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Performance.hpp
 * Author: elfilalz
 *
 * Created on September 19, 2017, 11:09 AM
 */

#ifndef PERFORMANCE_HPP
#define PERFORMANCE_HPP

#include "Option.hpp"

/// \brief Classe pour option performance
class Performance : public Option{
public:
    PnlVect *coefficients_; // coefficients de pondération de chaque actif
  /*
   * Ces deux vecteurs ont été créés dans le constructeur
   * pour gagner en temps lors du calcul du payoff
   */
    PnlVect *spot;
    PnlVect *spotAvant;

  Performance();
    Performance(const Performance& orig);
 /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe Performance
     *
     *  \param T           : maturité de l'option
     *  \param nbTimeSteps : nombre de cours dont on a besoin pour calculer le payoff
     *  \param size        : nombre d'actifs composant l'option
     *  \param coeff       : vecteur des coefficients de pondération
     */
  Performance(double T, int nbTimeSteps, int size, PnlVect *coefficients);

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
     *  Destructeur de la classe Performance
     */
    virtual ~Performance();
private:

};

#endif /* PERFORMANCE_HPP */

