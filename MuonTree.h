//=============================================                                 
// Author: Chris McGinn                                                         
//                                                                              
// Muon Tree Class (MC)                                                    
//                                                                              
//=============================================  
#ifndef MuonTree_h
#define MuonTree_h

#include <iostream>
#include "TTree.h"
#include "TFile.h"

TTree* muonTree_p = 0;

//Muon Tree Variables

Float_t trigTime_;
Float_t decayTime_;
Float_t gammaE_;

const Int_t nSteps = 2500;
Float_t timeStamp_[nSteps];
Float_t voltCh1_[nSteps];
Float_t voltCh2_[nSteps];

const Int_t nPeaks = 2;

Float_t meanCh1_[nPeaks];
Int_t peakStartCh1_[nPeaks];
Int_t peakEndCh1_[nPeaks];
Float_t peakSumCh1_[nPeaks];

Float_t meanCh2_[nPeaks];
Int_t peakStartCh2_[nPeaks];
Int_t peakEndCh2_[nPeaks];
Float_t peakSumCh2_[nPeaks];

void SetMuonBranches()
{
  muonTree_p->Branch("timeStamp", timeStamp_, Form("timeStamp[%d]/F", nSteps));
  muonTree_p->Branch("voltCh1", voltCh1_, Form("voltCh1[%d]/F", nSteps));
  muonTree_p->Branch("voltCh2", voltCh2_, Form("voltCh2[%d]/F", nSteps));

  muonTree_p->Branch("meanCh1", meanCh1_, Form("meanCh1[%d]/F", nPeaks));
  muonTree_p->Branch("peakStartCh1", peakStartCh1_, Form("peakStartCh1[%d]/I", nPeaks));
  muonTree_p->Branch("peakEndCh1", peakEndCh1_, Form("peakEndCh1[%d]/I", nPeaks));
  muonTree_p->Branch("peakSumCh1", peakSumCh1_, Form("peakSumCh1[%d]/F", nPeaks));

  muonTree_p->Branch("meanCh2", meanCh2_, Form("meanCh2[%d]/F", nPeaks));
  muonTree_p->Branch("peakStartCh2", peakStartCh2_, Form("peakStartCh2[%d]/I", nPeaks));
  muonTree_p->Branch("peakEndCh2", peakEndCh2_, Form("peakEndCh2[%d]/I", nPeaks));
  muonTree_p->Branch("peakSumCh2", peakSumCh2_, Form("peakSumCh2[%d]/F", nPeaks));

  return;
}


void GetMuonBranches()
{
  muonTree_p->SetBranchAddress("timeStamp", timeStamp_);
  muonTree_p->SetBranchAddress("voltCh1", voltCh1_);
  muonTree_p->SetBranchAddress("voltCh2", voltCh2_);

  muonTree_p->SetBranchAddress("meanCh1", meanCh1_);
  muonTree_p->SetBranchAddress("peakStartCh1", peakStartCh1_);
  muonTree_p->SetBranchAddress("peakEndCh1", peakEndCh1_);
  muonTree_p->SetBranchAddress("peakSumCh1", peakSumCh1_);

  muonTree_p->SetBranchAddress("meanCh2", meanCh2_);
  muonTree_p->SetBranchAddress("peakStartCh2", peakStartCh2_);
  muonTree_p->SetBranchAddress("peakEndCh2", peakEndCh2_);
  muonTree_p->SetBranchAddress("peakSumCh2", peakSumCh2_);

  return;
}


void InitMuonTree()
{
  std::cout << "Init Muon Tree" << std::endl;

  muonTree_p = new TTree("muonTree", "muonTree");
  SetMuonBranches();

  return;
}


void CleanupMuonTree()
{
  if(muonTree_p != 0) delete muonTree_p;

  return;
}


void GetMuonTree(TFile* muonFile_p)
{
  std::cout << "Get Muon Tree" << std::endl;

  muonTree_p = (TTree*)muonFile_p->Get("muonTree");
  GetMuonBranches();

  return;
}


void InitMuonVar()
{
  for(Int_t iter = 0; iter < nPeaks; iter++){
    meanCh1_[iter] = 0;
    peakStartCh1_[iter] = 0;
    peakEndCh1_[iter] = 0;
    peakSumCh1_[iter] = 0;
    
    meanCh2_[iter] = 0;
    peakStartCh2_[iter] = 0;
    peakEndCh2_[iter] = 0;
    peakSumCh2_[iter] = 0;
  }

  for(Int_t iter = 0; iter < nSteps; iter++){
    timeStamp_[iter] = 0;
    voltCh1_[iter] = 0;
    voltCh2_[iter] = 0;
  }

  return;
}


#endif
