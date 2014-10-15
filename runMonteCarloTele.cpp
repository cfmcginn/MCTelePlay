#include <iostream>
#include <vector>
#include <stdlib.h>
#include "TFile.h"
//#include "TH1F.h"
#include "TH2F.h"

#define PI 3.141592653589793

float getRandomFraction(int fracSize);

int main(int argc, char*argv[])
{
  if(argc!=3){
    std::cout << "Usage: testProcess <nEvtSim> <outFileName>" << std::endl;
    return 1;
  }

  const int nEvtSim = std::atoi(argv[1]);
  const float phiBound = 2*PI;
  const float thetaBound = PI;

  std::vector<float>* phiIn_p = new std::vector<float>;
  std::vector<float>* thetaIn_p = new std::vector<float>;
  std::vector<float>* phiOut_p = new std::vector<float>;
  std::vector<float>* thetaOut_p = new std::vector<float>;

  for(int genIter = 0; genIter < nEvtSim; genIter++){
    phiIn_p->push_back(phiBound*getRandomFraction(10000));
    phiOut_p->push_back(phiBound*getRandomFraction(10000));

    thetaIn_p->push_back(thetaBound*getRandomFraction(10000));
    thetaOut_p->push_back(thetaBound*getRandomFraction(10000));
  }

  TFile* outFile_p = new TFile(Form("%s.root", argv[2]), "UPDATE");

  TH2F* inPhiTheta_p = new TH2F("inPhiTheta_h", "inPhiTheta_h", 100, 0, phiBound, 100, 0, thetaBound);
  TH2F* outPhiTheta_p = new TH2F("outPhiTheta_h", "outPhiTheta_h", 100, 0, phiBound, 100, 0, thetaBound);

  for(int genIter = 0; genIter < nEvtSim; genIter++){
    inPhiTheta_p->Fill(phiIn_p->at(genIter), thetaIn_p->at(genIter));
    outPhiTheta_p->Fill(phiOut_p->at(genIter), thetaOut_p->at(genIter));
  }

  inPhiTheta_p->Write("", TObject::kOverwrite);
  outPhiTheta_p->Write("", TObject::kOverwrite);

  delete inPhiTheta_p;
  delete outPhiTheta_p;

  outFile_p->Close();
  delete outFile_p;

  phiIn_p->clear();
  thetaIn_p->clear();
  phiOut_p->clear();
  thetaOut_p->clear();

  delete phiIn_p;
  delete thetaIn_p;
  delete phiOut_p;
  delete thetaOut_p;

  return 0;
}


float getRandomFraction(int fracSize)
{
  float fracNum = std::rand()%(fracSize+1);
  return fracNum/((float)fracSize);
}
