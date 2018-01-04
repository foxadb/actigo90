/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Asian.hpp
 * Author: elfilalz
 *
 * Created on September 19, 2017, 10:39 AM
 */

#ifndef ASIAN_HPP
#define ASIAN_HPP

#include "Option.hpp"

/// \brief Classe pour option asiatique
class Asian : public Option{
    public:
  float strike_; // prix d'exercice de l'option
  PnlVect *coefficients_; // coefficients de pondération de chaque actif
  PnlVect *spot; // Vecteur créé dans le constructeur pour gagner en temps lors du calcul du payoff

        /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe Asian
     *
     *  \param T           : maturité de l'option
     *  \param nbTimeSteps : nombre de cours dont on a besoin pour calculer le payoff
     *  \param size        : nombre d'actifs composant l'option
     *  \param str         : prix d'exercice
     *  \param coeff       : vecteur des coefficients de pondération
     */
        Asian(double T, int nbTimeSteps, int size, float strike, PnlVect *coefficients);

   /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
  double payoff(const PnlMat *path);
        Asian();
        Asian(const Asian& orig);
        /*!
     *  \brief Destructeur
     *
     *  Destructeur de la classe Asian
     */
        virtual ~Asian();
    private:
        
};

#endif /* ASIAN_HPP */

