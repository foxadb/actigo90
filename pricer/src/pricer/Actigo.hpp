#ifndef ACTIGO_H
#define	ACTIGO_H

#include "Option.hpp"

/**
 * @brief Actigo class
 */
class Actigo : public Option {
public:
    double euroStoxSpot_; // euroStox initial spot
    double spUsaSpot_; // S&P 500 initial spot
    double spAusSpot_; // S&P 200 initial spot
    double rEur; // eur free risk rate
    double rUsd; // Usd free risk rate
    double rAud; // Aud free risk rate
    PnlVect *semestrialSpot_; // spots at the current semester date

    /**
     * @brief Actigo constructor
     * @param maturity       : actigo's maturity = 8 years
     * @param nbSteps : hedging dates number
     * @param size    : actigo option's size = 5
     * @param euroStoxSpot    : euroStox initial spot
     * @param spotUsa: S&P 500 initial spot
     * @param spotAus: S&P200 initial spot
     */
    Actigo(double maturity, int nbSteps, int size, double euroStoxSpot, double spotUsa, double spotAus, double rEur,
            double rUsd, double rAud);

    /**
    * @param init initial spot
    * @param current current spot
    * @return return (current - init)/init
    */
    double indexPerf(double init, double current);

    /**
    * @param path matrix containing the market data
    * @return Actgo's payoff
    */
    double payoff(const PnlMat* path);

    /**
    * @brief Actigo destructor
    */
    virtual ~Actigo();

    /**
    * @brief Acitgo constructor
    */
    Actigo();

    /**
    * @brief Actigo constructor.
    * @brief copy constructor
    * @param orig atigo option to copy
    */
    Actigo(const Actigo& orig);
};

#endif	/* ACTIGO_H */
