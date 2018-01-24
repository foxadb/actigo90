#ifndef DATA_H
#define	DATA_H
#include "pnl/pnl_vector.h"
#include "Option.hpp"

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
      PnlMat* dataMatrix;

      Data(const char* debutDate, const char* currentDate);
      void completeData(int remainingDates, Option *option, PnlVect *currentSpots, PnlVect *volatilities, PnlMat *correlations);
};
#endif
