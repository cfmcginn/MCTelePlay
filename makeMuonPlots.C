//=============================================
// Author: Chris McGinn
//
// Muon Plotter
//
//=============================================

#include "MuonHists.h"
#include "TLatex.h"
#include "TLine.h"

void drawMeanLine(TH1* inHist_p, Int_t lineStyle = 2, Int_t lineColor = 1)
{
  TLine* oneLine_p = new TLine(inHist_p->GetMean(), 0.0, inHist_p->GetMean(), TMath::Power( inHist_p->GetBinContent(inHist_p->GetMaximumBin()), 1.5));
  oneLine_p->SetLineColor(lineColor);
  oneLine_p->SetLineStyle(lineStyle);
  oneLine_p->Draw("Same");
  return;
}


void make3Plot(const std::string outName, const std::string plotName, const std::string title, TH1* hist1_p, const std::string label1, TH1* hist2_p, const std::string label2, TH1* hist3_p, const std::string label3)
{
  ThreeFormat(hist1_p, hist2_p, hist3_p);
  TCanvas* plotPanel_p = new TCanvas(Form("%sPanel", plotName.c_str()), Form("%sPanel", plotName.c_str()), 3*300, 1*350);
  makeMultiPanelCanvas(plotPanel_p, 3, 1, 0.0, 0.0, 0.2, 0.2, 0.01);
  plotPanel_p->cd(1);
  TLatex* label_p = new TLatex();
  label_p->SetNDC();
  label_p->SetTextFont(43);
  label_p->SetTextSizePixels(28);

  hist1_p->GetXaxis()->SetTitleOffset(1.00);
  hist1_p->DrawCopy("E1");
  label_p->DrawLatex(0.3, 0.9, Form("%s", title.c_str()));
  label_p->DrawLatex(0.7, 0.8, Form("%s", label1.c_str()));
  if(strcmp("voltSumCh1", plotName.c_str()) == 0) drawMeanLine(hist1_p);

  plotPanel_p->cd(2);
  hist2_p->GetXaxis()->SetTitleOffset(1.00);
  hist2_p->DrawCopy("E1");
  label_p->DrawLatex(0.60, 0.8, Form("%s", label2.c_str()));
  if(strcmp("voltSumCh1", plotName.c_str()) == 0) drawMeanLine(hist2_p);

  plotPanel_p->cd(3);
  hist3_p->GetXaxis()->SetTitleOffset(1.00);
  hist3_p->DrawCopy("E1");
  label_p->DrawLatex(0.60, 0.8, Form("%s", label3.c_str()));
  if(strcmp("voltSumCh1", plotName.c_str()) == 0) drawMeanLine(hist3_p);

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

  make3Plot(outName, Form("voltSumCh1"), "Ch. 1", peakSumCh1_p[0], "Mean 1", peakSumCh1_p[1], "Mean 2", peakSumCh1_p[2], "Mean 3");
  make3Plot(outName, Form("peakStartCh1"), "Ch. 1", peakStartCh1_p[0], "Mean 1", peakStartCh1_p[1], "Mean 2", peakStartCh1_p[2], "Mean 3");
  make3Plot(outName, Form("peakEndCh1"), "Ch. 1", peakEndCh1_p[0], "Mean 1", peakEndCh1_p[1], "Mean 2", peakEndCh1_p[2], "Mean 3");
  make3Plot(outName, Form("peakWidthCh1"), "Ch. 1", peakWidthCh1_p[0], "Mean 1", peakWidthCh1_p[1], "Mean 2", peakWidthCh1_p[2], "Mean 3");
  make3Plot(outName, Form("decayTimeCh1"), "Ch. 1", decayTimeCh1_p[0], "Mean 1", decayTimeCh1_p[1], "Mean 2", decayTimeCh1_p[2], "Mean 3");

  inFile_p->Close();
  delete inFile_p;

  return 0;
}
