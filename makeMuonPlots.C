//=============================================
// Author: Chris McGinn
//
// Muon Plotter
//
//=============================================

#include "MuonHists.h"
#include "TLatex.h"
#include "TLine.h"
#include "TF1.h"

void drawMeanLine(TH1* inHist_p, Int_t lineStyle = 2, Int_t lineColor = 1)
{
  TLine* oneLine_p = new TLine(inHist_p->GetMean(), 0.0, inHist_p->GetMean(), TMath::Power( inHist_p->GetBinContent(inHist_p->GetMaximumBin()), 1.5));
  oneLine_p->SetLineColor(lineColor);
  oneLine_p->SetLineStyle(lineStyle);
  oneLine_p->Draw("Same");
  return;
}


void fitExpo(TH1* inHist_p, TF1* inFit_p)
{
  Float_t bound = inHist_p->GetBinLowEdge(inHist_p->GetMaximumBin());
  Float_t max = inHist_p->GetBinContent(inHist_p->GetMaximumBin());
  Float_t mean = inHist_p->GetMean();

  inFit_p->SetParameter(0, max);
  inFit_p->SetParameter(1, 1/mean);

  inHist_p->Fit(inFit_p, "ML", "", bound, 10);

  std::cout << inFit_p->GetParameter(0) << std::endl;
  std::cout << inFit_p->GetParameter(1) << std::endl;

  return;
}


void makeResiduals(TH1* inHist_p, TF1* inFit_p, TH1* outHist_p)
{
  const Int_t nBins = inHist_p->GetNbinsX();
  Int_t maxBin = inHist_p->GetMaximumBin();
  for(Int_t iter = 0; iter < nBins; iter++){
    if(iter < maxBin){
      outHist_p->SetBinContent(iter+1, 0);
    }
    else{
      outHist_p->SetBinContent(iter+1, inHist_p->GetBinContent(iter+1) - inFit_p->Eval(inHist_p->GetXaxis()->GetBinCenter(iter+1)));		
      outHist_p->SetBinError(iter+1, inHist_p->GetBinError(iter+1));
    }
  }

  return;
}


void make3Plot(const std::string outName, const std::string plotName, const std::string title, TH1* hist1_p, const std::string label1, TH1* hist2_p, const std::string label2, TH1* hist3_p, const std::string label3)
{
  ThreeFormat(hist1_p, hist2_p, hist3_p);
  TCanvas* plotPanel_p;

  TH1F* resHist_p[3];
  const Int_t nBins = hist1_p->GetNbinsX();
  Float_t bins[nBins+1];
  for(Int_t iter = 0; iter < nBins+1; iter++){
    bins[iter] = hist1_p->GetXaxis()->GetBinLowEdge(iter+1);
  }

  TF1* expoFit_p = new TF1("expoFit", "[0]*TMath::Exp(-x/[1])");

  if(strcmp("decayTimeCh1", plotName.c_str()) != 0){
    plotPanel_p = new TCanvas(Form("%sPanel", plotName.c_str()), Form("%sPanel", plotName.c_str()), 3*300, 1*350);
    makeMultiPanelCanvas(plotPanel_p, 3, 1, 0.0, 0.0, 0.2, 0.2, 0.01);
  }
  else{
    for(Int_t iter = 0; iter < 3; iter++){
      resHist_p[iter] = new TH1F(Form("resHist_%d", iter), Form("resHist_%d", iter), nBins, bins);
    }

    plotPanel_p = new TCanvas(Form("%sPanel", plotName.c_str()), Form("%sPanel", plotName.c_str()), 3*300, 2*350);
    makeMultiPanelCanvas(plotPanel_p, 3, 2, 0.0, 0.0, 0.2, 0.2, 0.01);
  }

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
  if(strcmp("decayTimeCh1", plotName.c_str()) == 0){
    hist1_p->GetYaxis()->SetTitleOffset(2.50);
    fitExpo(hist1_p, expoFit_p);
    makeResiduals(hist1_p, expoFit_p, resHist_p[0]);
  }
  else hist1_p->GetYaxis()->SetTitleOffset(1.50);


  plotPanel_p->cd(2);
  hist2_p->GetXaxis()->SetTitleOffset(1.00);
  hist2_p->DrawCopy("E1");
  label_p->DrawLatex(0.60, 0.8, Form("%s", label2.c_str()));
  if(strcmp("voltSumCh1", plotName.c_str()) == 0) drawMeanLine(hist2_p);
  if(strcmp("decayTimeCh1", plotName.c_str()) == 0){
    fitExpo(hist2_p, expoFit_p);
    makeResiduals(hist2_p, expoFit_p, resHist_p[1]);
    hist2_p->GetYaxis()->SetTitleOffset(2.50);
  }
  else hist2_p->GetYaxis()->SetTitleOffset(1.50);

  plotPanel_p->cd(3);
  hist3_p->GetXaxis()->SetTitleOffset(1.00);
  hist3_p->DrawCopy("E1");
  label_p->DrawLatex(0.60, 0.8, Form("%s", label3.c_str()));
  if(strcmp("voltSumCh1", plotName.c_str()) == 0) drawMeanLine(hist3_p);
  if(strcmp("decayTimeCh1", plotName.c_str()) == 0){
    fitExpo(hist3_p, expoFit_p);
    makeResiduals(hist3_p, expoFit_p, resHist_p[2]);
    hist3_p->GetYaxis()->SetTitleOffset(2.50);
  }
  else hist3_p->GetYaxis()->SetTitleOffset(1.50);

  if(strcmp("decayTimeCh1", plotName.c_str()) == 0){
    FullThreeFormat(resHist_p[0], resHist_p[1], resHist_p[2]);
    TLine* zeroLine_p = new TLine(0.0, 0.0, 10.0, 0.0);
    zeroLine_p->SetLineColor(1);
    zeroLine_p->SetLineStyle(2);

    for(Int_t iter = 0; iter < 3; iter++){
      plotPanel_p->cd(iter+4);
      resHist_p[iter]->DrawCopy("E1");
      zeroLine_p->Draw("Same");
    }
  }
    
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

  make3Plot(outName, Form("voltTwoSumCh1"), "Ch. 1", peakTwoSumCh1_p[0], "Mean 1", peakTwoSumCh1_p[1], "Mean 2", peakTwoSumCh1_p[2], "Mean 3");
  make3Plot(outName, Form("peakStartCh1"), "Ch. 1", peakStartCh1_p[0], "Mean 1", peakStartCh1_p[1], "Mean 2", peakStartCh1_p[2], "Mean 3");
  make3Plot(outName, Form("peakEndCh1"), "Ch. 1", peakEndCh1_p[0], "Mean 1", peakEndCh1_p[1], "Mean 2", peakEndCh1_p[2], "Mean 3");
  make3Plot(outName, Form("peakWidthCh1"), "Ch. 1", peakWidthCh1_p[0], "Mean 1", peakWidthCh1_p[1], "Mean 2", peakWidthCh1_p[2], "Mean 3");
  make3Plot(outName, Form("decayTimeCh1"), "Ch. 1", decayTimeCh1_p[0], "Mean 1", decayTimeCh1_p[1], "Mean 2", decayTimeCh1_p[2], "Mean 3");
  make3Plot(outName, Form("voltOneSumCh1"), "Ch. 1", peakOneSumCh1_p[0], "Mean 1", peakOneSumCh1_p[1], "Mean 2", peakOneSumCh1_p[2], "Mean 3");

  inFile_p->Close();
  delete inFile_p;

  return 0;
}
