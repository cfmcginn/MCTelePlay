#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#define PI 3.141592653589793

float getRandomFraction(int fracSize);
float getPhi(int fracSize);
float getTheta(int fracSize);
float getPlanePos(int fracSize, float dimSize);
float getRExit(float trajPhi, float& xExit, float& yExit);

int main(int argc, char*argv[])
{
  if(argc!=3){
    std::cout << "Usage: testProcess <nEvtSim> <outFileName>" << std::endl;
    return 1;
  }

  const int nEvtSim = std::atoi(argv[1]);
  const float thetaBound = PI/2;
  const int fracSize = 10000;

  const float dimPlane = 10.0; 
  const float rHemi = dimPlane/2.0*TMath::Sqrt(2.0);

  std::vector<float>* phiHemi_p = new std::vector<float>;
  std::vector<float>* thetaHemi_p = new std::vector<float>;
  std::vector<float>* hemiX_p = new std::vector<float>;
  std::vector<float>* hemiY_p = new std::vector<float>;

  std::vector<float>* planeX1_p = new std::vector<float>;
  std::vector<float>* planeY1_p = new std::vector<float>;

  std::vector<float>* delR_p = new std::vector<float>;
  std::vector<float>* delL_p = new std::vector<float>;

  std::vector<float>* trajPhi_p = new std::vector<float>;
  std::vector<float>* rLeftTraj_p = new std::vector<float>;

  for(int genIter = 0; genIter < nEvtSim; genIter++){
    phiHemi_p->push_back(getPhi(fracSize));
    thetaHemi_p->push_back(getTheta(fracSize));

    hemiX_p->push_back(rHemi*cos(thetaHemi_p->at(genIter))*cos(phiHemi_p->at(genIter)));
    hemiY_p->push_back(rHemi*cos(thetaHemi_p->at(genIter))*sin(phiHemi_p->at(genIter)));

    planeX1_p->push_back(getPlanePos(fracSize, dimPlane));
    planeY1_p->push_back(getPlanePos(fracSize, dimPlane));

    delL_p->push_back(rHemi*sin(thetaHemi_p->at(genIter)));

    float delX = hemiX_p->at(genIter) - planeX1_p->at(genIter);
    float delY = hemiY_p->at(genIter) - planeY1_p->at(genIter);

    delR_p->push_back(TMath::Sqrt(delX*delX + delY*delY));
    trajPhi_p->push_back(TMath::ATan2(delY, delX));

    float xExit = -999.0;
    float yExit = -999.0;

    getXYExit(trajPhi_p->push_back(genIter), planeDim, xExit, yExit);


  }

  TFile* outFile_p = new TFile(Form("%s.root", argv[2]), "UPDATE");
  TH2F* inPhiTheta_p = new TH2F("inPhiTheta_h", "inPhiTheta_h", 100, 0, 2*PI, 100, 0, thetaBound);
  TH2F* inXY1_p = new TH2F("inXY1_h", "inXY1_h", 100, -dimPlane/2.0, dimPlane/2.0, 100, -dimPlane/2.0, dimPlane/2.0);
  TH1F* delLOverDelR_p = new TH1F("delLOverDelR_h", "delLOverDelR_h", 100, 0, 20);

  for(int genIter = 0; genIter < nEvtSim; genIter++){
    if(TMath::Abs(thetaBound/2 - thetaHemi_p->at(genIter)) > thetaBound) std::cout << "thetaHemi Error" << std::endl;

    inPhiTheta_p->Fill(phiHemi_p->at(genIter), thetaHemi_p->at(genIter));
    inXY1_p->Fill(planeX1_p->at(genIter), planeY1_p->at(genIter));
    delLOverDelR_p->Fill(delL_p->at(genIter)/delR_p->at(genIter));
  }


  inPhiTheta_p->Write("", TObject::kOverwrite);
  delete inPhiTheta_p;

  inXY1_p->Write("", TObject::kOverwrite);
  delete inXY1_p;

  delLOverDelR_p->Write("", TObject::kOverwrite);
  delete delLOverDelR_p;

  outFile_p->Close();
  delete outFile_p;

  phiHemi_p->clear();
  thetaHemi_p->clear();
  hemiX_p->clear();
  hemiY_p->clear();

  planeX1_p->clear();
  planeY1_p->clear();

  delR_p->clear();
  delL_p->clear();

  trajPhi_p->clear();
  rLeftTraj_p->clear();

  delete phiHemi_p;
  delete thetaHemi_p;
  delete hemiX_p;
  delete hemiY_p;

  delete planeX1_p;
  delete planeY1_p;

  delete delR_p;
  delete delL_p;

  delete trajPhi_p;
  delete rLeftTraj_p;

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

float getRExit(float trajPhi, float planeDim, float& xExit, float& yExit)
{
  float xHi = planeDim/2.0;
  float xLow = -planeDim/2.0;
  float yHi = planeDim/2.0;
  float yLow = -planeDim/2.0;

  if(trajPhi >= 0) yExit = yHi;
  else yExit = yLow;

  if(TMath::Abs(trajPhi) < TMath::Pi()/2) xExit = xHi;
  else xExit = xLow;

  return;
}
