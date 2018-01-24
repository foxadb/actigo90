#ifndef DATA_H
#define	DATA_H
#include "pnl/pnl_vector.h"

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
      void completeData(int totalNumberOfdates);
};
#endif
