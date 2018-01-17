#include "Actigo.hpp"
#include <iostream>

using namespace std;


Actigo::Actigo() {
}

Actigo::Actigo(const Actigo& orig){
}

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
  int currentSemesterDate = 125; //Nombre de jours ouvrés dans un semestre -1:
  int lastDate = path->m;
  //cout << lastDate;
  double totalPerf= 0.; 
  while (currentSemesterDate <= lastDate){
    double semestrialPerf = 0.;
    pnl_mat_get_row(semestrialSpot_, path, currentSemesterDate);
    semestrialPerf+= indexPerf(euroStoxSpot_, pnl_vect_get(semestrialSpot_, 0)); 
    semestrialPerf+= indexPerf(spUsaSpot_, pnl_vect_get(semestrialSpot_, 1));
    semestrialPerf+= indexPerf(spAusSpot_, pnl_vect_get(semestrialSpot_, 2));
    semestrialPerf/= 3;
    if (semestrialPerf < 0.)
      semestrialPerf = 0. ;
    totalPerf += semestrialPerf;
    currentSemesterDate += 126; 
  }
  return 0.9 + totalPerf; 
}