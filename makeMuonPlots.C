//=============================================
// Author: Chris McGinn
//
// Muon Plotter
//
//=============================================

#include "MuonHists.h"

int makeMuonPlots(const std::string inName)
{
  TH1::SetDefaultSumw2();

  std::string outName = inName;
  const std::string cutString = "Hists";
  const std::string repString = "Plots";

  std::size_t strIndex = outName.find(cutString);
  if(!(strIndex == std::string::npos)) outName.replace(strIndex, cutString.length(), repString);

  TFile* inFile_p = new TFile(inName.c_str(), "READ");
  GetMuonHists(inFile_p);
  SameFormat(peakSumCh1_p, peakSumCh2_p);

  TCanvas* plotPanel_p = new TCanvas("peakSumPanel", "peakSumPanel", 2*300, 1*350);
  makeMultiPanelCanvas(plotPanel_p, 2, 1, 0.0, 0.0, 0.2, 0.2, 0.01);
  plotPanel_p->cd(1);
  peakSumCh1_p->Draw("E1");
  plotPanel_p->cd(2);
  peakSumCh2_p->Draw("E1");

  TFile* outFile_p = new TFile(outName.c_str(), "UPDATE");
  plotPanel_p->Write("", TObject::kOverwrite);
  outFile_p->Close();
  delete outFile_p;

  delete plotPanel_p;

  inFile_p->Close();
  delete inFile_p;

  return 0;
}
