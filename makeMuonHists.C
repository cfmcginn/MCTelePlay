//=============================================
// Author: Chris McGinn
//
// Muon Histogram Maker
//
//=============================================

#include "MuonTree.h"
#include "MuonHists.h"
#include "TH1F.h"
#include "TMath.h"


int makeMuonHists(const std::string inName)
{
  TH1::SetDefaultSumw2();

  std::string outName = inName;
  const std::string cutString = "Tree";
  const std::string repString = "Hists";

  std::size_t strIndex = outName.find(cutString);
  if(!(strIndex == std::string::npos)) outName.replace(strIndex, cutString.length(), repString);

  TFile* inFile_p = new TFile(inName.c_str(), "READ");
  GetMuonTree(inFile_p);

  BookHists();

  Int_t nentries = muonTree_p->GetEntries();
  for(Int_t iter = 0; iter < nentries; iter++){
    muonTree_p->GetEntry(iter);

    for(Int_t peakIter = 0; peakIter < nPeaks; peakIter++){
      peakSumCh1_p[peakIter]->Fill(TMath::Abs(peakSumCh1_[peakIter]));
      peakSumCh2_p[peakIter]->Fill(TMath::Abs(peakSumCh2_[peakIter]));

      peakStartCh1_p[peakIter]->Fill(timeStamp_[peakStartCh1_[peakIter]]*TMath::Power(10, 6));
      peakStartCh2_p[peakIter]->Fill(timeStamp_[peakStartCh2_[peakIter]]*TMath::Power(10, 6));
      
      peakEndCh1_p[peakIter]->Fill(timeStamp_[peakEndCh1_[peakIter]]*TMath::Power(10, 6));
      peakEndCh2_p[peakIter]->Fill(timeStamp_[peakEndCh2_[peakIter]]*TMath::Power(10, 6));
      
      peakWidthCh1_p[peakIter]->Fill((timeStamp_[peakEndCh1_[peakIter]] - timeStamp_[peakStartCh1_[peakIter]])*TMath::Power(10, 6));
      peakWidthCh2_p[peakIter]->Fill((timeStamp_[peakEndCh2_[peakIter]] - timeStamp_[peakStartCh2_[peakIter]])*TMath::Power(10, 6));
    }
  }

  FormatAllHists();

  TFile* outFile_p = new TFile(outName.c_str(), "UPDATE");

  for(Int_t iter = 0; iter < nPeaks; iter++){
    peakSumCh1_p[iter]->Write("", TObject::kOverwrite);
    peakSumCh2_p[iter]->Write("", TObject::kOverwrite);

    peakStartCh1_p[iter]->Write("", TObject::kOverwrite);
    peakStartCh2_p[iter]->Write("", TObject::kOverwrite);
    
    peakEndCh1_p[iter]->Write("", TObject::kOverwrite);
    peakEndCh2_p[iter]->Write("", TObject::kOverwrite);
    
    peakWidthCh1_p[iter]->Write("", TObject::kOverwrite);
    peakWidthCh2_p[iter]->Write("", TObject::kOverwrite);
  }

  outFile_p->Close();
  delete outFile_p;

  CleanupHists();

  CleanupMuonTree();
  inFile_p->Close();
  delete inFile_p;

  return 0;
}
