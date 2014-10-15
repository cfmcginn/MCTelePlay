#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"

#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"

float getRandomFraction(int fracSize);
float getPhi(int fracSize);
float getTheta(int fracSize);
float getPlanePos(int fracSize, float dimSize);
float getRExit(float trajPhi, float planeDim, float xExit, float yExit);
float getLFraction(const int nEvtSim, const float planeL);

boost::random::mt19937 gen;

int main(int argc, char*argv[])
{
  if(argc!=5){
    std::cout << "Usage: testProcess <nEvtSim> <outFileName> <nL> <maxL>" << std::endl;
    return 1;
  }

  const int nEvtSim = std::atoi(argv[1]);
  const int nL = std::atoi(argv[3]);
  const int maxL = std::atoi(argv[4]);


  TFile* outFile_p = new TFile(Form("%s.root", argv[2]), "UPDATE");
  TH1F* lAcceptance_p = new TH1F("lAcceptance_h", "lAcceptance_h", nL, 0, maxL);

  for(int lIter = 0; lIter < nL; lIter++){
    float planeL = ((float)lIter)/((float)nL)*(float)maxL;
    lAcceptance_p->SetBinContent(lIter+1, getLFraction(nEvtSim, planeL));
  }

  lAcceptance_p->Write("", TObject::kOverwrite);
  delete lAcceptance_p;

  outFile_p->Close();
  delete outFile_p;

  return 0;
}

float getRandomFraction(int fracSize)
{
  boost::random::uniform_int_distribution<> dist(0, fracSize);
  float fracNum = dist(gen);
  return fracNum/((float)fracSize);
}

float getPhi(int fracSize)
{
  return 2*TMath::Pi()*getRandomFraction(fracSize);
}

float getTheta(int fracSize)
{
  float outTheta = TMath::ACos(2*getRandomFraction(fracSize) - 1);
  if(outTheta > TMath::Pi()/2.0) outTheta = TMath::Pi() - outTheta;
  return outTheta;
}

float getPlanePos(int fracSize, float dimSize)
{
  float outPos = dimSize*getRandomFraction(fracSize);
  if(outPos > dimSize/2.0) outPos = dimSize/2.0 - outPos;
  return outPos;
}

float getRExit(float trajPhi, float planeDim, float xHit, float yHit)
{
  float xEdge = planeDim/2.0;
  if(TMath::Abs(trajPhi) > TMath::Pi()/2.0) xEdge = -planeDim/2.0;
  float yEdge = planeDim/2.0;
  if(trajPhi < 0) yEdge = -planeDim/2.0;

  trajPhi = TMath::Abs(trajPhi);

  float delX = TMath::Abs(xEdge - xHit);
  float delY = TMath::Abs(yEdge - yHit);

  if(trajPhi > TMath::Pi()/2) trajPhi = TMath::Pi() - trajPhi;

  float delRX = delX/cos(trajPhi);
  float delRY = delY/sin(trajPhi);

  if(delRX < delRY) return delRX;
  else return delRY;
}

float getLFraction(const int nEvtSim, const float planeL)
{
  float nEvtPass = 0;

  std::cout << planeL << std::endl;

  const int fracSize = 10000;
  const float dimPlane = 10.0;
  const float rHemi = dimPlane/2.0*TMath::Sqrt(2.0);

  for(int genIter = 0; genIter < nEvtSim; genIter++){
    float phiHemi = getPhi(fracSize);
    float thetaHemi = getTheta(fracSize);
    float planeX1 = getPlanePos(fracSize, dimPlane);
    float planeY1 = getPlanePos(fracSize, dimPlane);

    float hemiX = rHemi*cos(thetaHemi)*cos(phiHemi);
    float hemiY = rHemi*cos(thetaHemi)*sin(phiHemi);
    float delL = rHemi*sin(thetaHemi);

    float delX = hemiX - planeX1;
    float delY = hemiY - planeY1;
    float delR = TMath::Sqrt(delX*delX + delY*delY);
    float trajPhi = TMath::ATan2(delY, delX);

    float rExit = getRExit(trajPhi, dimPlane, planeX1, planeY1);

    if((delL/delR)*rExit >= planeL) nEvtPass++;
  }

  return nEvtPass/((float)nEvtSim);
}
