#include <iostream>
#include <vector>
#include <stdlib.h>
#include "TFile.h"
//#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#define PI 3.141592653589793

float getRandomFraction(int fracSize);
float getPhi(int fracSize);
float getTheta(int fracSize);

float getPlanePos(int fracSize, float dimSize);

int main(int argc, char*argv[])
{
  if(argc!=3){
    std::cout << "Usage: testProcess <nEvtSim> <outFileName>" << std::endl;
    return 1;
  }

  const int nEvtSim = std::atoi(argv[1]);
  const float thetaBound = PI/2;
  const int fracSize = 10000;

  const float dimX1 = 10.0; 
  const float dimY1 = 10.0; 

  std::vector<float>* phiHemi_p = new std::vector<float>;
  std::vector<float>* thetaHemi_p = new std::vector<float>;

  std::vector<float>* planeX1_p = new std::vector<float>;
  std::vector<float>* planeY1_p = new std::vector<float>;

  for(int genIter = 0; genIter < nEvtSim; genIter++){
    phiHemi_p->push_back(getPhi(fracSize));
    thetaHemi_p->push_back(getTheta(fracSize));

    planeX1_p->push_back(getPlanePos(fracSize, dimX1));
    planeY1_p->push_back(getPlanePos(fracSize, dimY1));
  }

  TFile* outFile_p = new TFile(Form("%s.root", argv[2]), "UPDATE");
  TH2F* inPhiTheta_p = new TH2F("inPhiTheta_h", "inPhiTheta_h", 100, 0, 2*PI, 100, 0, thetaBound);
  TH2F* inXY1_p = new TH2F("inXY1_h", "inXY1_h", 100, -dimX1/2.0, dimX1/2.0, 100, -dimY1/2.0, dimY1/2.0);

  for(int genIter = 0; genIter < nEvtSim; genIter++){
    if(TMath::Abs(thetaBound/2 - thetaHemi_p->at(genIter)) > thetaBound) std::cout << "thetaHemi Error" << std::endl;

    inPhiTheta_p->Fill(phiHemi_p->at(genIter), thetaHemi_p->at(genIter));
    inXY1_p->Fill(planeX1_p->at(genIter), planeY1_p->at(genIter));
  }

  inPhiTheta_p->Write("", TObject::kOverwrite);
  delete inPhiTheta_p;

  inXY1_p->Write("", TObject::kOverwrite);
  delete inXY1_p;

  outFile_p->Close();
  delete outFile_p;

  phiHemi_p->clear();
  thetaHemi_p->clear();

  planeX1_p->clear();
  planeY1_p->clear();

  delete phiHemi_p;
  delete thetaHemi_p;

  delete planeX1_p;
  delete planeY1_p;

  return 0;
}


float getRandomFraction(int fracSize)
{
  float fracNum = std::rand()%(fracSize+1);
  return fracNum/((float)fracSize);
}

float getPhi(int fracSize)
{
  return 2*PI*getRandomFraction(fracSize);
}

float getTheta(int fracSize)
{
  float outTheta = TMath::ACos(2*getRandomFraction(fracSize) - 1);
  if(outTheta > PI/2.0) outTheta = PI - outTheta;
  return outTheta;
}

float getPlanePos(int fracSize, float dimSize)
{
  float outPos = dimSize*getRandomFraction(fracSize);
  if(outPos > dimSize/2.0) outPos = dimSize/2.0 - outPos;
  return outPos;
}
