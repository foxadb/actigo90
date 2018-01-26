#ifndef DATA_H
#define	DATA_H

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "Option.hpp"
#include "../quote/quote.hpp"

class Data{
public:
      PnlVect* euroStoxSpots;
      PnlVect* spUsdSpots;
      PnlVect* spAudSpots;
      PnlVect* eurUsd;
      PnlVect* eurAud;
      double rEur;
      double rUsd;
      double rAud;
      PnlMat* historicalDataMatrix;
      PnlMat* completeDataMatrix;

      Data(const char* debutDate, const char* currentDate);
      Data(PnlMat *matrixData);
      Data();
      ~Data();
      Data(const Data& orig);
      void getInitialSpots(PnlVect *initialSpots);
      void getTodaySpots(PnlVect *toDaySpots);
      void completeData(int remainingDates, Option *option, PnlVect *currentSpots,
                        PnlVect *volatilities, PnlMat *correlations, PnlVect* trends, PnlRng* rng);
};
#endif
