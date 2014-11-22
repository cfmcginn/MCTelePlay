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


const Float_t sumCut = 15;

int makeMuonHists(const std::string inName, Bool_t isCh2 = false)
{
  TH1::SetDefaultSumw2();

  std::string outName = inName;
  const std::string cutString = "Tree";
  const std::string repString = "Hists";

  std::size_t strIndex = outName.find(cutString);
  if(!(strIndex == std::string::npos)) outName.replace(strIndex, cutString.length(), repString);

  TFile* inFile_p = new TFile(inName.c_str(), "READ");
  GetMuonTree(inFile_p, isCh2);

  BookHists(isCh2);

  Int_t nentries = muonTree_p->GetEntries();
  for(Int_t iter = 0; iter < nentries; iter++){
    muonTree_p->GetEntry(iter);

    if(nPeakCh1_ > 1){
      for(Int_t sumIter = 0; sumIter < nPeakSums; sumIter++){
	
	Float_t sumTot = peakSumCh1_[1][sumIter];
	for(Int_t peakIter = 2; peakIter < nPeakCh1_; peakIter++){
	  if(peakStartTimeCh1_[peakIter][sumIter] - peakEndTimeCh1_[1][sumIter] < .000001) sumTot += peakSumCh1_[peakIter][sumIter];
	}

	sumTot = TMath::Abs(sumTot);
	if(sumTot < sumCut) continue;

        peakSumCh1_p[sumIter]->Fill(sumTot);
        peakStartCh1_p[sumIter]->Fill((peakStartTimeCh1_[1][sumIter])*TMath::Power(10, 6));
	peakEndCh1_p[sumIter]->Fill((peakEndTimeCh1_[1][sumIter])*TMath::Power(10, 6));
        peakWidthCh1_p[sumIter]->Fill((peakEndTimeCh1_[1][sumIter] - peakStartTimeCh1_[1][sumIter])*TMath::Power(10,6));
	decayTimeCh1_p[sumIter]->Fill((peakStartTimeCh1_[1][sumIter] - peakStartTimeCh1_[0][sumIter])*TMath::Power(10,6));
      }
    }

  }

  FormatAllHists(isCh2);

  TFile* outFile_p = new TFile(outName.c_str(), "UPDATE");

  for(Int_t iter = 0; iter < nHistPeaks; iter++){
    peakSumCh1_p[iter]->Write("", TObject::kOverwrite);
    peakStartCh1_p[iter]->Write("", TObject::kOverwrite);
    peakEndCh1_p[iter]->Write("", TObject::kOverwrite);
    peakWidthCh1_p[iter]->Write("", TObject::kOverwrite);
    decayTimeCh1_p[iter]->Write("", TObject::kOverwrite);
  }

  outFile_p->Close();
  delete outFile_p;

  CleanupHists(isCh2);

  CleanupMuonTree();
  inFile_p->Close();
  delete inFile_p;

  return 0;
}
