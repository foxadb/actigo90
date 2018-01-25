#ifndef DATA_H
#define	DATA_H
#include "pnl/pnl_vector.h"
#include "Option.hpp"
#include "../quote/quote.hpp"
#include "../quote/forex.hpp"

class Data{
public:
      PnlVect* euroStoxSpots;
      PnlVect* spUsdSpots;
      PnlVect* spAudSpots;
      PnlVect* forexEurUsd;
      PnlVect* forexEurAud;
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
      void completeData(int remainingDates, Option *option, PnlVect *currentSpots, PnlVect *volatilities, PnlMat *correlations);
};
#endif
