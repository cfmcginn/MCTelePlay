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

const Int_t maxNSteps = 2500;
Float_t timeStamp_[maxNSteps];
Float_t voltCh1_[maxNSteps];
Float_t voltCh2_[maxNSteps];

const Int_t maxNPeaks = 4;
const Int_t nPeakSums = 2;

Int_t nPeakCh1_;
Float_t meanCh1_;
Float_t meanCutCh1_;
Int_t peakStartCh1_[maxNPeaks][nPeakSums];
Float_t peakStartTimeCh1_[maxNPeaks][nPeakSums];
Int_t peakEndCh1_[maxNPeaks][nPeakSums];
Float_t peakEndTimeCh1_[maxNPeaks][nPeakSums];
Float_t peakSumCh1_[maxNPeaks][nPeakSums];

Int_t nPeakCh2_;
Float_t meanCh2_;
Float_t meanCutCh2_;
Int_t peakStartCh2_[maxNPeaks][nPeakSums];
Float_t peakStartTimeCh2_[maxNPeaks][nPeakSums];
Int_t peakEndCh2_[maxNPeaks][nPeakSums];
Float_t peakEndTimeCh2_[maxNPeaks][nPeakSums];
Float_t peakSumCh2_[maxNPeaks][nPeakSums];

void SetMuonBranches(Bool_t isCh2)
{
  muonTree_p->Branch("timeStamp", timeStamp_, Form("timeStamp[%d]/F", maxNSteps));
  muonTree_p->Branch("voltCh1", voltCh1_, Form("voltCh1[%d]/F", maxNSteps));
  if(isCh2) muonTree_p->Branch("voltCh2", voltCh2_, Form("voltCh2[%d]/F", maxNSteps));

  muonTree_p->Branch("nPeakCh1", &nPeakCh1_, "nPeakCh1/I");
  muonTree_p->Branch("meanCh1", &meanCh1_, Form("meanCh1/F"));
  muonTree_p->Branch("meanCutCh1", &meanCutCh1_, Form("meanCutCh1/F"));
  muonTree_p->Branch("peakStartCh1", peakStartCh1_, Form("peakStartCh1[nPeakCh1][%d]/I", nPeakSums));
  muonTree_p->Branch("peakStartTimeCh1", peakStartTimeCh1_, Form("peakStartTimeCh1[nPeakCh1][%d]/F", nPeakSums));
  muonTree_p->Branch("peakEndCh1", peakEndCh1_, Form("peakEndCh1[nPeakCh1][%d]/I", nPeakSums));
  muonTree_p->Branch("peakEndTimeCh1", peakEndTimeCh1_, Form("peakEndTimeCh1[nPeakCh1][%d]/F", nPeakSums));
  muonTree_p->Branch("peakSumCh1", peakSumCh1_, Form("peakSumCh1[nPeakCh1][%d]/F", nPeakSums));

  if(isCh2){
    muonTree_p->Branch("nPeakCh2", &nPeakCh2_, "nPeakCh2/I");
    muonTree_p->Branch("meanCh2", &meanCh2_, Form("meanCh2/F"));
    muonTree_p->Branch("meanCutCh2", &meanCutCh2_, Form("meanCutCh2/F"));
    muonTree_p->Branch("peakStartCh2", peakStartCh2_, Form("peakStartCh2[nPeakCh2][%d]/I", nPeakSums));
    muonTree_p->Branch("peakStartTimeCh2", peakStartTimeCh2_, Form("peakStartTimeCh2[nPeakCh2][%d]/F", nPeakSums));
    muonTree_p->Branch("peakEndCh2", peakEndCh2_, Form("peakEndCh2[nPeakCh2][%d]/I", nPeakSums));
    muonTree_p->Branch("peakEndTimeCh2", peakEndTimeCh2_, Form("peakEndTimeCh2[nPeakCh2][%d]/F", nPeakSums));
    muonTree_p->Branch("peakSumCh2", peakSumCh2_, Form("peakSumCh2[nPeakCh2][%d]/F", nPeakSums));
  }    

  return;
}


void GetMuonBranches(Bool_t isCh2)
{
  muonTree_p->SetBranchAddress("timeStamp", timeStamp_);
  muonTree_p->SetBranchAddress("voltCh1", voltCh1_);
  if(isCh2) muonTree_p->SetBranchAddress("voltCh2", voltCh2_);

  muonTree_p->SetBranchAddress("nPeakCh1", &nPeakCh1_);
  muonTree_p->SetBranchAddress("meanCh1", &meanCh1_);
  muonTree_p->SetBranchAddress("meanCutCh1", &meanCutCh1_);
  muonTree_p->SetBranchAddress("peakStartCh1", peakStartCh1_);
  muonTree_p->SetBranchAddress("peakStartTimeCh1", peakStartTimeCh1_);
  muonTree_p->SetBranchAddress("peakEndCh1", peakEndCh1_);
  muonTree_p->SetBranchAddress("peakEndTimeCh1", peakEndTimeCh1_);
  muonTree_p->SetBranchAddress("peakSumCh1", peakSumCh1_);

  if(isCh2){
    muonTree_p->SetBranchAddress("nPeakCh2", &nPeakCh2_);
    muonTree_p->SetBranchAddress("meanCh2", &meanCh2_);
    muonTree_p->SetBranchAddress("meanCutCh2", &meanCutCh2_);
    muonTree_p->SetBranchAddress("peakStartCh2", peakStartCh2_);
    muonTree_p->SetBranchAddress("peakStartTimeCh2", peakStartTimeCh2_);
    muonTree_p->SetBranchAddress("peakEndCh2", peakEndCh2_);
    muonTree_p->SetBranchAddress("peakEndTimeCh2", peakEndTimeCh2_);
    muonTree_p->SetBranchAddress("peakSumCh2", peakSumCh2_);
  }

  return;
}


void InitMuonTree(Bool_t isCh2)
{
  std::cout << "Init Muon Tree" << std::endl;

  muonTree_p = new TTree("muonTree", "muonTree");
  SetMuonBranches(isCh2);

  return;
}


void CleanupMuonTree()
{
  if(muonTree_p != 0) delete muonTree_p;

  return;
}


void GetMuonTree(TFile* muonFile_p, Bool_t isCh2)
{
  std::cout << "Get Muon Tree" << std::endl;

  muonTree_p = (TTree*)muonFile_p->Get("muonTree");
  GetMuonBranches(isCh2);

  return;
}


void InitMuonVar(Bool_t isCh2)
{
  nPeakCh1_ = 0;
  meanCh1_ = 0;
  meanCutCh1_ = 0;

  if(isCh2){
    nPeakCh2_ = 0;
    meanCh2_ = 0;
    meanCutCh2_ = 0;
  }

  for(Int_t iter = 0; iter < maxNPeaks; iter++){
    for(Int_t iter2 = 0; iter2 < nPeakSums; iter2++){
      peakStartCh1_[iter][iter2] = 0;
      peakStartTimeCh1_[iter][iter2] = 0;
      peakEndCh1_[iter][iter2] = 0;
      peakEndTimeCh1_[iter][iter2] = 0;
      peakSumCh1_[iter][iter2] = 0;
      
      if(isCh2){
	peakStartCh2_[iter][iter2] = 0;
	peakStartTimeCh2_[iter][iter2] = 0;
	peakEndCh2_[iter][iter2] = 0;
	peakEndTimeCh2_[iter][iter2] = 0;
	peakSumCh2_[iter][iter2] = 0;
      }
    }
  }    

  for(Int_t iter = 0; iter < maxNSteps; iter++){
    timeStamp_[iter] = 0;
    voltCh1_[iter] = 0;
    if(isCh2) voltCh2_[iter] = 0;
  }

  return;
}

#endif
