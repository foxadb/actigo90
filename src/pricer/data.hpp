#ifndef DATA_H
#define	DATA_H

class data:{
public:
      PnlVect* euroStoxSpots;
      PnlVect* sAndpUsd;
      PnlVect* sAndpAud;
      PnlVect* forexEurUsd;
      PnlVect* forexEurAud;
      double rEur;
      double rUsd;
      double rAud;
      PnlMat* dataMatrix;

      data(string debutDate, string currentDate);
      completeDate(int totalNumberOfdates);
};
