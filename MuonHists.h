//=============================================                                 
// Author: Chris McGinn                                                         
//                                                                              
// Muon Hists Class (MC)                                                    
//                                                                              
//=============================================  
#ifndef MuonHists_h
#define MuonHists_h

#include <iostream>
#include <string>

#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TCanvas.h"

const Int_t nHistPeaks = 3;
const std::string peakStr[nHistPeaks] = {"Mean1", "Mean2", "Mean3"};

TH1F* peakTwoSumCh1_p[nHistPeaks];
TH1F* peakTwoSumCh2_p[nHistPeaks];
TH1F* peakStartCh1_p[nHistPeaks];
TH1F* peakStartCh2_p[nHistPeaks];
TH1F* peakEndCh1_p[nHistPeaks];
TH1F* peakEndCh2_p[nHistPeaks];
TH1F* peakWidthCh1_p[nHistPeaks];
TH1F* peakWidthCh2_p[nHistPeaks];

TH1F* decayTimeCh1_p[nHistPeaks];
TH1F* peakOneSumCh1_p[nHistPeaks];

void handsomeTH1(TH1 *a = 0, Int_t col = 1, Float_t size = 1, Int_t markerstyle = 20)
{
  a->SetMarkerColor(col);
  a->SetMarkerSize(size);
  a->SetMarkerStyle(markerstyle);
  a->SetLineColor(col);
  a->GetYaxis()->SetTitleOffset(1.25);
  a->GetXaxis()->SetTitleOffset(.75);
  a->GetXaxis()->CenterTitle();
  a->GetYaxis()->CenterTitle();

  return;
}


void niceTH1(TH1* uglyTH1, float max , float min, float ndivX, float ndivY, Int_t col = 1, Float_t size = 1, Int_t style = 20)
{
  handsomeTH1(uglyTH1, col, size, style);
  uglyTH1->SetMaximum(max);
  uglyTH1->SetMinimum(min);
  uglyTH1->SetNdivisions(ndivX);
  uglyTH1->SetNdivisions(ndivY, "Y");

  return;
}


void BookHists(Bool_t isCh2 = false)
{
  for(Int_t iter = 0; iter < nHistPeaks; iter++){
    peakTwoSumCh1_p[iter] = new TH1F(Form("peakTwo%sSumCh1_h", peakStr[iter].c_str()), Form("peakTwo%sSumCh1_h", peakStr[iter].c_str()), 60, 0.0001, 149.9999);
    peakStartCh1_p[iter] = new TH1F(Form("peak%sStartCh1_h", peakStr[iter].c_str()), Form("peak%sStartCh1_h", peakStr[iter].c_str()), 60, 0.0001, 9.9999);
    peakEndCh1_p[iter] = new TH1F(Form("peak%sEndCh1_h", peakStr[iter].c_str()), Form("peak%sEndCh1_h", peakStr[iter].c_str()), 60, 0.0001, 9.9999);
    peakWidthCh1_p[iter] = new TH1F(Form("peak%sWidthCh1_h", peakStr[iter].c_str()), Form("peak%sWidthCh1_h", peakStr[iter].c_str()), 60, 0.0001, 9.9999);

    decayTimeCh1_p[iter] = new TH1F(Form("decayTime%sCh1_h", peakStr[iter].c_str()), Form("decayTime%sCh1_h", peakStr[iter].c_str()), 30, 0.0001, 8.9999);
    peakOneSumCh1_p[iter] = new TH1F(Form("peakOne%sSumCh1_h", peakStr[iter].c_str()), Form("peakOne%sSumCh1_h", peakStr[iter].c_str()), 100, 0.0001, 199.9999);

    if(isCh2){
      peakTwoSumCh2_p[iter] = new TH1F(Form("peakTwo%sSumCh2_h", peakStr[iter].c_str()), Form("peakTwo%sSumCh2_h", peakStr[iter].c_str()), 60, 0.0001, 149.9999);
      peakStartCh2_p[iter] = new TH1F(Form("peak%sStartCh2_h", peakStr[iter].c_str()), Form("peak%sStartCh2_h", peakStr[iter].c_str()), 60, -0.1999, .9999);
      peakEndCh2_p[iter] = new TH1F(Form("peak%sEndCh2_h", peakStr[iter].c_str()), Form("peak%sEndCh2_h", peakStr[iter].c_str()), 60, -0.1999, .9999);
      peakWidthCh2_p[iter] = new TH1F(Form("peak%sWidthCh2_h", peakStr[iter].c_str()), Form("peak%sWidthCh2_h", peakStr[iter].c_str()), 60, 0.0001, 1.1999);
    }
  }

  return;
}


void FormatHist(TH1* inHist_p)
{
  Float_t maxVal = inHist_p->GetBinContent(inHist_p->GetMaximumBin());
  maxVal += TMath::Sqrt(maxVal);
  niceTH1(inHist_p, maxVal, 0.00, 505, 505);

  return;
}


void TwoFormat(TH1* inHist1_p, TH1* inHist2_p)
{
  Float_t maxVal = inHist1_p->GetBinContent(inHist1_p->GetMaximumBin());
  if(maxVal < inHist2_p->GetBinContent(inHist2_p->GetMaximumBin())) maxVal = inHist2_p->GetBinContent(inHist2_p->GetMaximumBin());

  maxVal += TMath::Sqrt(maxVal);

  niceTH1(inHist1_p, maxVal, 0.00, 505, 505);
  niceTH1(inHist2_p, maxVal, 0.00, 505, 505);

  return;
}


void ThreeFormat(TH1* inHist1_p, TH1* inHist2_p, TH1* inHist3_p)
{
  Float_t maxVal = inHist1_p->GetBinContent(inHist1_p->GetMaximumBin());
  if(maxVal < inHist2_p->GetBinContent(inHist2_p->GetMaximumBin())) maxVal = inHist2_p->GetBinContent(inHist2_p->GetMaximumBin());
  if(maxVal < inHist3_p->GetBinContent(inHist3_p->GetMaximumBin())) maxVal = inHist3_p->GetBinContent(inHist3_p->GetMaximumBin());

  maxVal += TMath::Sqrt(maxVal);

  niceTH1(inHist1_p, maxVal, 0.00, 505, 505);
  niceTH1(inHist2_p, maxVal, 0.00, 505, 505);
  niceTH1(inHist3_p, maxVal, 0.00, 505, 505);

  return;
}


void FullThreeFormat(TH1* inHist1_p, TH1* inHist2_p, TH1* inHist3_p)
{
  Float_t maxVal = 0;
  Float_t minVal = 0;
  for(Int_t iter = 0; iter < inHist1_p->GetNbinsX(); iter++){
    if(inHist1_p->GetBinContent(iter+1) + inHist1_p->GetBinError(iter+1) > maxVal) maxVal = inHist1_p->GetBinContent(iter+1) + inHist1_p->GetBinError(iter+1);
    if(inHist1_p->GetBinContent(iter+1) - inHist1_p->GetBinError(iter+1) < minVal) minVal = inHist1_p->GetBinContent(iter+1) - inHist1_p->GetBinError(iter+1);
  }

  for(Int_t iter = 0; iter < inHist2_p->GetNbinsX(); iter++){
    if(inHist2_p->GetBinContent(iter+1) + inHist2_p->GetBinError(iter+1) > maxVal) maxVal = inHist2_p->GetBinContent(iter+1) + inHist2_p->GetBinError(iter+1);
    if(inHist2_p->GetBinContent(iter+1) - inHist2_p->GetBinError(iter+1) < minVal) minVal = inHist2_p->GetBinContent(iter+1) - inHist2_p->GetBinError(iter+1);
  }

  for(Int_t iter = 0; iter < inHist3_p->GetNbinsX(); iter++){
    if(inHist3_p->GetBinContent(iter+1) + inHist3_p->GetBinError(iter+1) > maxVal) maxVal = inHist3_p->GetBinContent(iter+1) + inHist3_p->GetBinError(iter+1);
    if(inHist3_p->GetBinContent(iter+1) - inHist3_p->GetBinError(iter+1) < minVal) minVal = inHist3_p->GetBinContent(iter+1) - inHist3_p->GetBinError(iter+1);
  }

  niceTH1(inHist1_p, maxVal, minVal, 505, 505);
  niceTH1(inHist2_p, maxVal, minVal, 505, 505);
  niceTH1(inHist3_p, maxVal, minVal, 505, 505);

  return;
}


void SetTitleHist(TH1* inHist_p, const std::string xTitle, const std::string yTitle)
{
  inHist_p->SetXTitle(xTitle.c_str());
  inHist_p->GetXaxis()->SetTitleFont(43);
  inHist_p->GetXaxis()->SetTitleSize(26);

  inHist_p->SetYTitle(yTitle.c_str());
  inHist_p->GetYaxis()->SetTitleFont(43);
  inHist_p->GetYaxis()->SetTitleSize(26);

  return;
}


void FormatAllHists(Bool_t isCh2 = false)
{
  for(Int_t iter = 0; iter < nHistPeaks; iter++){
    FormatHist(peakTwoSumCh1_p[iter]);
    SetTitleHist(peakTwoSumCh1_p[iter], Form("peakTwoSum [Volt]"), "Counts");
    
    FormatHist(peakStartCh1_p[iter]);
    SetTitleHist(peakStartCh1_p[iter], Form("peakStartx10^{6} [#mus]"), "Counts");
    
    FormatHist(peakEndCh1_p[iter]);
    SetTitleHist(peakEndCh1_p[iter], Form("peakEndx10^{6} [#mus]"), "Counts");
    
    FormatHist(peakWidthCh1_p[iter]);
    SetTitleHist(peakWidthCh1_p[iter], Form("peakWidthx10^{6} [#mus]"), "Counts");

    FormatHist(decayTimeCh1_p[iter]);
    SetTitleHist(decayTimeCh1_p[iter], Form("decayTimex10^{6} [#mus]"), "Counts");
 
    FormatHist(peakOneSumCh1_p[iter]);
    SetTitleHist(peakOneSumCh1_p[iter], Form("peakOneSum [Volt]"), "Counts");

    if(isCh2){
      FormatHist(peakTwoSumCh2_p[iter]);
      SetTitleHist(peakTwoSumCh2_p[iter], Form("peakTwoSum [Volt]"), "Counts");
      
      FormatHist(peakStartCh2_p[iter]);
      SetTitleHist(peakStartCh2_p[iter], Form("peakStartx10^{6} [#mus]"), "Counts");
      
      FormatHist(peakEndCh2_p[iter]);
      SetTitleHist(peakEndCh2_p[iter], Form("peakEndx10^{6} [#mus]"), "Counts");
      
      FormatHist(peakWidthCh2_p[iter]);
      SetTitleHist(peakWidthCh2_p[iter], Form("peakWidthx10^{6} [#mus]"), "Counts");
    }
  }

  return;
}


void CleanupHists(Bool_t isCh2 = false)
{
  for(Int_t iter = 0; iter < nHistPeaks; iter++){
    delete peakTwoSumCh1_p[iter];
    delete peakStartCh1_p[iter];
    delete peakEndCh1_p[iter];
    delete peakWidthCh1_p[iter];
    delete decayTimeCh1_p[iter];
    delete peakOneSumCh1_p[iter];


    if(isCh2){
      delete peakTwoSumCh2_p[iter];
      delete peakStartCh2_p[iter];
      delete peakEndCh2_p[iter];
      delete peakWidthCh2_p[iter];
    }
  }

  return;
}


void GetMuonHists(TFile* muonHistFile_p, Bool_t isCh2 = false)
{
  std::cout << "Get Muon Hists" << std::endl;

  for(Int_t iter = 0; iter < nHistPeaks; iter++){
    peakTwoSumCh1_p[iter] = (TH1F*)muonHistFile_p->Get(Form("peakTwo%sSumCh1_h", peakStr[iter].c_str()));
    peakStartCh1_p[iter] = (TH1F*)muonHistFile_p->Get(Form("peak%sStartCh1_h", peakStr[iter].c_str()));
    peakEndCh1_p[iter] = (TH1F*)muonHistFile_p->Get(Form("peak%sEndCh1_h", peakStr[iter].c_str()));
    peakWidthCh1_p[iter] = (TH1F*)muonHistFile_p->Get(Form("peak%sWidthCh1_h", peakStr[iter].c_str()));
    decayTimeCh1_p[iter] = (TH1F*)muonHistFile_p->Get(Form("decayTime%sCh1_h", peakStr[iter].c_str()));
    peakOneSumCh1_p[iter] = (TH1F*)muonHistFile_p->Get(Form("peakOne%sSumCh1_h", peakStr[iter].c_str()));

    if(isCh2){
      peakTwoSumCh2_p[iter] = (TH1F*)muonHistFile_p->Get(Form("peakTwo%sSumCh2_h", peakStr[iter].c_str()));
      peakStartCh2_p[iter] = (TH1F*)muonHistFile_p->Get(Form("peak%sStartCh2_h", peakStr[iter].c_str()));
      peakEndCh2_p[iter] = (TH1F*)muonHistFile_p->Get(Form("peak%sEndCh2_h", peakStr[iter].c_str()));
      peakWidthCh2_p[iter] = (TH1F*)muonHistFile_p->Get(Form("peak%sWidthCh2_h", peakStr[iter].c_str()));
    }
  }

  return;
}


void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns, const Int_t rows, const Float_t leftOffset, const Float_t bottomOffset, const Float_t leftMargin, const Float_t bottomMargin, const Float_t edge)
{
  if(canv==0){
    Error("makeMultiPanelCanvas","Got null canvas.");
    return;
  }
  canv->Clear();

  TPad* pad[columns][rows];

  Float_t Xlow[columns];
  Float_t Xup[columns];
  Float_t Ylow[rows];
  Float_t Yup[rows];
  Float_t PadWidth = (1.0-leftOffset)/((1.0/(1.0-leftMargin)) + (1.0/(1.0-edge))+(Float_t)columns-2.0);
  Float_t PadHeight = (1.0-bottomOffset)/((1.0/(1.0-bottomMargin)) + (1.0/(1.0-edge))+(Float_t)rows-2.0);
  Xlow[0] = leftOffset;
  Xup[0] = leftOffset + PadWidth/(1.0-leftMargin);
  Xup[columns-1] = 1;
  Xlow[columns-1] = 1.0-PadWidth/(1.0-edge);

  Yup[0] = 1;
  Ylow[0] = 1.0-PadHeight/(1.0-edge);
  Ylow[rows-1] = bottomOffset;
  Yup[rows-1] = bottomOffset + PadHeight/(1.0-bottomMargin);

  for(Int_t i=1; i<columns-1; i++){
    Xlow[i] = Xup[0] + (i-1)*PadWidth;
    Xup[i] = Xup[0] + (i)*PadWidth;
  }
  Int_t ct = 0;
  for(Int_t i=rows-2; i>0; i--){
    Ylow[i] = Yup[rows-1] + ct*PadHeight;
    Yup[i] = Yup[rows-1] + (ct+1)*PadHeight;
    ct++;
  }

  TString padName;
  for(Int_t i=0; i<columns; i++){
    for(Int_t j=0; j<rows; j++){
      canv->cd();
      padName = Form("p_%d_%d",i,j);

      //      if(i==0 && j==1) pad[i][j] = new TPad(padName.Data(),padName.Data(),Xlow[i],Ylow[j],Xup[i]*0.890,Yup[j]*0.935);
      if(i==0 && j==1) pad[i][j] = new TPad(padName.Data(),padName.Data(),Xlow[i],Ylow[j]*0.934,Xup[i],Yup[j]*0.995);
      else if(i==0 && j==0) pad[i][j] = new TPad(padName.Data(),padName.Data(),Xlow[i],Ylow[j]*0.934,Xup[i],Yup[j]*.9985);
      else if(i==1 && j==1) pad[i][j] = new TPad(padName.Data(),padName.Data(),Xlow[i],Ylow[j],Xup[i],Yup[j]*.995);
      else if(j == 0) pad[i][j] = new TPad(padName.Data(),padName.Data(),Xlow[i],Ylow[j],Xup[i],Yup[j]*.998);
      else pad[i][j] = new TPad(padName.Data(),padName.Data(),Xlow[i],Ylow[j],Xup[i],Yup[j]*.995);

      if(i==0){
        if(j == 0) pad[i][j]->SetLeftMargin(leftMargin*1.2);
	else pad[i][j]->SetLeftMargin(leftMargin*1.2);
      }
      else if(i==1 && j==1){
	pad[i][j]->SetLeftMargin(0);
      }
      else pad[i][j]->SetLeftMargin(0);
 
      if(i==(columns-1)) pad[i][j]->SetRightMargin(edge);
      else pad[i][j]->SetRightMargin(0);

      if(j==0){
        if(i==0)pad[i][j]->SetTopMargin(edge);
        else pad[i][j]->SetTopMargin(edge);
      }
      else pad[i][j]->SetTopMargin(0);

      if(j==(rows-1)) pad[i][j]->SetBottomMargin(bottomMargin);
      else if(i==0 && (j==0 || j == 1)) pad[i][j]->SetBottomMargin(0.17*PadHeight);
      else pad[i][j]->SetBottomMargin(0);

      pad[i][j]->Draw();
      pad[i][j]->cd();
      pad[i][j]->SetNumber(columns*j+i+1);
    }
  }
  pad[0][0]->cd();

  return;
}


void claverCanvasSaving(TCanvas* c, TString s,TString format="gif")
{
  TDatime* date = new TDatime();
  c->SaveAs(Form("%s_%d.%s",s.Data(),date->GetDate(), format.Data()));
  return;
}


#endif
