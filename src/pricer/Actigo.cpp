#include "Actigo.hpp"
#include <iostream>

using namespace std;

Actigo::Actigo() {}

Actigo::Actigo(const Actigo& orig){}

Actigo::~Actigo(){
    pnl_vect_free(&semestrialSpot_);
}

Actigo::Actigo(double maturity, int nbSteps, int size, double euroStoxSpot, double spUsa, double spAus){
    size_ = size;
    euroStoxSpot_ = euroStoxSpot;
    spUsaSpot_ = spUsa;
    spAusSpot_ = spAus;
    T_ = maturity;
    nbTimeSteps_ = nbSteps;
    semestrialSpot_ = pnl_vect_create(3);
}

double Actigo::indexPerf(double init, double current){
    return (current - init)/init ;
}

double Actigo::payoff(const PnlMat* path){
    double rDoll = 0.05;
    double rAus = 0.05;
    //cout << lastDate;
    double totalPerf= 0.;
    for (int currentSemesterDate = 1; currentSemesterDate <=15; currentSemesterDate++){
        double semestrialPerf = 0.;
        pnl_mat_get_row(semestrialSpot_, path, currentSemesterDate);
	      LET(semestrialSpot_,1) = GET(semestrialSpot_,1)*rDoll/GET(semestrialSpot_,3);
	      LET(semestrialSpot_,2) = GET(semestrialSpot_,2)*rAus/GET(semestrialSpot_,4);
        semestrialPerf+= indexPerf(euroStoxSpot_, pnl_vect_get(semestrialSpot_, 0));
        semestrialPerf+= indexPerf(spUsaSpot_, pnl_vect_get(semestrialSpot_, 1));
        semestrialPerf+= indexPerf(spAusSpot_, pnl_vect_get(semestrialSpot_, 2));
        semestrialPerf/= 3;
        if (semestrialPerf < 0.)
            semestrialPerf = 0. ;
        totalPerf += semestrialPerf;
    }
    return  (totalPerf - 0.1);
}
