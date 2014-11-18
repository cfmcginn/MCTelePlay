//=============================================
// Author: Chris McGinn
//
// Muon Plotter
//
//=============================================

#include "MuonHists.h"
#include "TLatex.h"

void make2Plot(const std::string outName, const std::string plotName, const std::string title, TH1* hist1_p, const std::string label1, TH1* hist2_p, const std::string label2)
{
  SameFormat(hist1_p, hist2_p);
  TCanvas* plotPanel_p = new TCanvas(Form("%sPanel", plotName.c_str()), Form("%sPanel", plotName.c_str()), 2*300, 1*350);
  makeMultiPanelCanvas(plotPanel_p, 2, 1, 0.0, 0.0, 0.2, 0.2, 0.01);
  plotPanel_p->cd(1);
  TLatex* label_p = new TLatex();
  label_p->SetNDC();
  label_p->SetTextFont(43);
  label_p->SetTextSizePixels(28);

  hist1_p->GetXaxis()->SetTitleOffset(1.00);
  hist1_p->DrawCopy("E1");
  label_p->DrawLatex(0.3, 0.9, Form("%s", title.c_str()));
  label_p->DrawLatex(0.3, 0.8, Form("%s", label1.c_str()));
  plotPanel_p->cd(2);
  hist2_p->GetXaxis()->SetTitleOffset(1.00);
  hist2_p->DrawCopy("E1");
  label_p->DrawLatex(0.10, 0.8, Form("%s", label2.c_str()));

  TFile* outFile_p = new TFile(outName.c_str(), "UPDATE");
  plotPanel_p->Write("", TObject::kOverwrite);
  claverCanvasSaving(plotPanel_p, Form("pdfDir/%sPanel", plotName.c_str()), "pdf");
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

  for(Int_t iter = 0; iter < nHistPeaks; iter++){
    make2Plot(outName, Form("peak%sSum", peakStr[iter].c_str()), peakStr[iter].c_str(), peakSumCh1_p[iter], "Ch. 1", peakSumCh2_p[iter], "Ch. 2");
    make2Plot(outName, Form("peak%sStart", peakStr[iter].c_str()), peakStr[iter].c_str(), peakStartCh1_p[iter], "Ch. 1", peakStartCh2_p[iter], "Ch. 2");
    make2Plot(outName, Form("peak%sEnd", peakStr[iter].c_str()), peakStr[iter].c_str(), peakEndCh1_p[iter], "Ch. 1", peakEndCh2_p[iter], "Ch. 2");
    make2Plot(outName, Form("peak%sWidth", peakStr[iter].c_str()), peakStr[iter].c_str(), peakWidthCh1_p[iter], "Ch. 1", peakWidthCh2_p[iter], "Ch. 2");
  }

  make2Plot(outName, Form("uncutCutCh1Sum"), "Ch. 1", peakSumCh1_p[0], "Uncut", peakSumCh1_p[1], "Cut");
  make2Plot(outName, Form("uncutCutCh1Start"), "Ch. 1", peakStartCh1_p[0], "Uncut", peakStartCh1_p[1], "Cut");
  make2Plot(outName, Form("uncutCutCh1End"), "Ch. 1", peakEndCh1_p[0], "Uncut", peakEndCh1_p[1], "Cut");
  make2Plot(outName, Form("uncutCutCh1Width"), "Ch. 1", peakWidthCh1_p[0], "Uncut", peakWidthCh1_p[1], "Cut");

  make2Plot(outName, Form("uncutCutCh2Sum"), "Ch. 2", peakSumCh2_p[0], "Uncut", peakSumCh2_p[1], "Cut");
  make2Plot(outName, Form("uncutCutCh2Start"), "Ch. 2", peakStartCh2_p[0], "Uncut", peakStartCh2_p[1], "Cut");
  make2Plot(outName, Form("uncutCutCh2End"), "Ch. 2", peakEndCh2_p[0], "Uncut", peakEndCh2_p[1], "Cut");
  make2Plot(outName, Form("uncutCutCh2Width"), "Ch. 2", peakWidthCh2_p[0], "Uncut", peakWidthCh2_p[1], "Cut");

  inFile_p->Close();
  delete inFile_p;

  return 0;
}
