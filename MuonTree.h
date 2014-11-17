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

Int_t peakStartCh1_;
Int_t peakEndCh1_;
Float_t peakSumCh1_;

Int_t peakStartCh2_;
Int_t peakEndCh2_;
Float_t peakSumCh2_;

void SetMuonBranches()
{
  muonTree_p->Branch("timeStamp", timeStamp_, Form("timeStamp[%d]/F", nSteps));
  muonTree_p->Branch("voltCh1", voltCh1_, Form("voltCh1[%d]/F", nSteps));
  muonTree_p->Branch("voltCh2", voltCh2_, Form("voltCh2[%d]/F", nSteps));

  muonTree_p->Branch("peakStartCh1", &peakStartCh1_, "peakStartCh1/I");
  muonTree_p->Branch("peakEndCh1", &peakEndCh1_, "peakEndCh1/I");
  muonTree_p->Branch("peakSumCh1", &peakSumCh1_, "peakSumCh1/F");

  muonTree_p->Branch("peakStartCh2", &peakStartCh2_, "peakStartCh2/I");
  muonTree_p->Branch("peakEndCh2", &peakEndCh2_, "peakEndCh2/I");
  muonTree_p->Branch("peakSumCh2", &peakSumCh2_, "peakSumCh2/F");

  return;
}


void GetMuonBranches()
{
  muonTree_p->SetBranchAddress("timeStamp", timeStamp_);
  muonTree_p->SetBranchAddress("voltCh1", voltCh1_);
  muonTree_p->SetBranchAddress("voltCh2", voltCh2_);

  muonTree_p->SetBranchAddress("peakStartCh1", &peakStartCh1_);
  muonTree_p->SetBranchAddress("peakEndCh1", &peakEndCh1_);
  muonTree_p->SetBranchAddress("peakSumCh1", &peakSumCh1_);

  muonTree_p->SetBranchAddress("peakStartCh2", &peakStartCh2_);
  muonTree_p->SetBranchAddress("peakEndCh2", &peakEndCh2_);
  muonTree_p->SetBranchAddress("peakSumCh2", &peakSumCh2_);

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


#endif
