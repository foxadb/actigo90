#include "Option.hpp"

#ifndef ACTIGO_H
#define	ACTIGO_H

/// \brief Classe pour le produit Actigo
class Actigo : public Option {
public:
    double euroStoxSpot_; // spot initial de l'indice euroStoxx.
    double spUsaSpot_; // spot initial de l'indice S&P 500 Usa.
    double spAusSpot_; // spot initial de l'indice S&P 200 Aus.
    PnlVect *semestrialSpot_; // vecteur representant le valeur des indices le semstre courant.

    Actigo(double maturity, int nbSteps, int size, double euroStoxSpot, double spotUsa, double spAus);

    double indexPerf(double init, double current);

    double payoff(const PnlMat* path);

    virtual ~Actigo();

    Actigo();

    Actigo(const Actigo& orig);
};

#endif	/* ACTIGO_H */
