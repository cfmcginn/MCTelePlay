//=============================================                                 
// Author: Chris McGinn                                                         
//                                                                              
// Muon Hists Class (MC)                                                    
//                                                                              
//=============================================  
#ifndef MuonHists_h
#define MuonHists_h

#include <iostream>

#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TCanvas.h"

TH1F* peakSumCh1_p;
TH1F* peakSumCh2_p;


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


void BookHists()
{
  peakSumCh1_p = new TH1F("peakSumCh1_h", "peakSumCh1_h", 100, 0.0001, 1999.9999);
  peakSumCh2_p = new TH1F("peakSumCh2_h", "peakSumCh2_h", 100, 0.0001, 1999.9999);

  return;
}


void FormatHist(TH1* inHist_p)
{
  Float_t maxVal = inHist_p->GetBinContent(inHist_p->GetMaximumBin());
  maxVal = maxVal + TMath::Sqrt(maxVal);
  niceTH1(inHist_p, maxVal, 0.00, 505, 505);

  return;
}


void SameFormat(TH1* inHist1_p, TH1* inHist2_p)
{
  Float_t maxVal = inHist1_p->GetBinContent(inHist1_p->GetMaximumBin());
  if(maxVal < inHist2_p->GetBinContent(inHist2_p->GetMaximumBin())) maxVal = inHist2_p->GetBinContent(inHist2_p->GetMaximumBin());

  maxVal += TMath::Sqrt(maxVal);

  niceTH1(inHist1_p, maxVal, 0.00, 505, 505);
  niceTH1(inHist2_p, maxVal, 0.00, 505, 505);

  return;
}


void SetTitleHist(TH1* inHist_p, const std::string xTitle, const std::string yTitle)
{
  inHist_p->SetXTitle(xTitle.c_str());
  inHist_p->SetYTitle(yTitle.c_str());

  return;
}


void FormatAllHists()
{
  FormatHist(peakSumCh1_p);
  FormatHist(peakSumCh2_p);

  SetTitleHist(peakSumCh1_p, "peakSum (Ch. 1)", "Counts");
  SetTitleHist(peakSumCh2_p, "peakSum (Ch. 2)", "Counts");

  return;
}


void CleanupHists()
{
  delete peakSumCh1_p;
  delete peakSumCh2_p;

  return;
}


void GetMuonHists(TFile* muonHistFile_p)
{
  std::cout << "Get Muon Hists" << std::endl;

  peakSumCh1_p = (TH1F*)muonHistFile_p->Get("peakSumCh1_h");
  peakSumCh2_p = (TH1F*)muonHistFile_p->Get("peakSumCh2_h");

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

      if(i==0 && j==1) pad[i][j] = new TPad(padName.Data(),padName.Data(),Xlow[i],Ylow[j],Xup[i]*0.890,Yup[j]*0.935);
      else if(i==0 && j==0) pad[i][j] = new TPad(padName.Data(),padName.Data(),Xlow[i],Ylow[j]*0.934,Xup[i],Yup[j]*.9985);
      else if(i==1 && j==1) pad[i][j] = new TPad(padName.Data(),padName.Data(),0.80*Xlow[i],Ylow[j],Xup[i],Yup[j]*.995);
      else if(j == 0) pad[i][j] = new TPad(padName.Data(),padName.Data(),Xlow[i],Ylow[j],Xup[i],Yup[j]*.998);
      else pad[i][j] = new TPad(padName.Data(),padName.Data(),Xlow[i],Ylow[j],Xup[i],Yup[j]*.995);

      if(i==0){
        if(j == 0) pad[i][j]->SetLeftMargin(leftMargin*1.2);
	else pad[i][j]->SetLeftMargin(leftMargin);
      }
      else if(i==1 && j==1){
	pad[i][j]->SetLeftMargin(PadWidth*.65);
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
      else if(i==0 && j==0) pad[i][j]->SetBottomMargin(0.17*PadHeight);
      else pad[i][j]->SetBottomMargin(0);

      pad[i][j]->Draw();
      pad[i][j]->cd();
      pad[i][j]->SetNumber(columns*j+i+1);
    }
  }
  pad[0][0]->cd();

  return;
}


#endif
