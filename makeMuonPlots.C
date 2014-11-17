//=============================================
// Author: Chris McGinn
//
// Muon Plotter
//
//=============================================

#include "MuonHists.h"
#include "TLatex.h"

void make2Plot(const std::string outName, const std::string histName, TH1* ch1_p, TH1* ch2_p)
{
  SameFormat(ch1_p, ch2_p);
  TCanvas* plotPanel_p = new TCanvas(Form("peak%sPanel", histName.c_str()), Form("peak%sPanel", histName.c_str()), 2*300, 1*350);
  makeMultiPanelCanvas(plotPanel_p, 2, 1, 0.0, 0.0, 0.2, 0.2, 0.01);
  plotPanel_p->cd(1);
  TLatex* label_p = new TLatex();
  label_p->SetNDC();
  label_p->SetTextFont(43);
  label_p->SetTextSizePixels(28);

  ch1_p->GetXaxis()->SetTitleOffset(1.00);
  ch1_p->DrawCopy("E1");
  label_p->DrawLatex(0.3, 0.9, "Ch. 1");
  plotPanel_p->cd(2);
  ch2_p->GetXaxis()->SetTitleOffset(1.00);
  ch2_p->DrawCopy("E1");
  label_p->DrawLatex(0.10, 0.9, "Ch. 2");

  TFile* outFile_p = new TFile(outName.c_str(), "UPDATE");
  plotPanel_p->Write("", TObject::kOverwrite);
  outFile_p->Close();
  delete outFile_p;

  delete plotPanel_p;

  return;
}

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

  make2Plot(outName, "Sum", peakSumCh1_p, peakSumCh2_p);
  make2Plot(outName, "Start", peakStartCh1_p, peakStartCh2_p);
  make2Plot(outName, "End", peakEndCh1_p, peakEndCh2_p);
  make2Plot(outName, "Width", peakWidthCh1_p, peakWidthCh2_p);

  inFile_p->Close();
  delete inFile_p;

  return 0;
}
