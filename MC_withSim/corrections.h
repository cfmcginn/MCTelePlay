//written by Austin Baty to smear jet pt spectra by 5%
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TSystem.h"
#include "TNtuple.h"
#include "TSVDUnfold.h"
#include "TRandom3.h"
#include <iostream>

TH1D* unFold(double resolution = 0.1, double energyScale = 1.7, double cutoff = 52.83, int nBins = 150, int reg = 3, int file = 0, const char *title = "result")
{
	gSystem->Load("libTree");
	TFile * fin;
	if(file == 0) fin = new TFile("C:\\Users\\Austin\\Desktop\\8_811_Project\\Advanced_MC\\11_24_MC_Signal_ESpectrum_10MEvents.root","read");
	else fin = new TFile("C:\\Users\\Austin\\Desktop\\8_811_Project\\Advanced_MC\\12_3_MC_UniSpectrum_1MEvents.root","read");
	TNtuple * ntup = (TNtuple*) fin->Get("Data");

	float E, ELostDetect;
	ntup->SetBranchAddress("E", &E);
	ntup->SetBranchAddress("ELostDetect", &ELostDetect);

	TH1D * genMC = new TH1D("genMC","genMC",nBins,0,150);
	TH1D * recoMC = new TH1D("recoMC", "recoMC", nBins, 0, 150);
	TH2D * response = new TH2D("response", "response", nBins,0,150, nBins,0,150);
	//TH1D * data = new TH1D("data","data",nBins,0,150);

	TH1D * data;
	TFile * dataF = new TFile("C:\\Users\\Austin\\Desktop\\8_811_Project\\Data\\lvmCh1_MERGE_MuonHists.root","read");
	TH1D * dataTemp = (TH1D*)dataF->Get("peakTwoMean2SumCh1_h");
	data = (TH1D*) dataTemp->Clone("data");
	
	data->Rebin(1200/nBins);

	TRandom3 R;

	int n = (int)ntup->GetEntries();
	int totalAccepted = 0;

	for(int i = 0; i<n; i++)
	{
		ntup->GetEntry(i);

		double recoE = ELostDetect*energyScale;
		recoE = R.Gaus(recoE, resolution*recoE);

		if(recoE>20 && E>20.0/energyScale) 
		{
			genMC->Fill(E);
			recoMC->Fill(recoE);
			response->Fill(recoE, E);
			totalAccepted++;
		}

		//data->Fill(recoE+R.Gaus(0,recoE*resolution));
	}

	TFile * outFile = new TFile(Form("C:\\Users\\Austin\\Desktop\\8_811_Project\\Advanced_MC\\Unfolding\\Unfolding_working_directory\\%s.root",title),"recreate");

	TSVDUnfold *tsvd = new TSVDUnfold(data, recoMC, genMC, response);
	TH1D* result = tsvd->Unfold(reg);
	TH1D* D = tsvd->GetD();
	TH1D* SV = tsvd->GetSV();
	//TH2D* Cov = tsvd->GetAdetCovMatrix(10);

	TH2D* errors = new TH2D("errors","errors",nBins,0,150,nBins,0,150);
	for(int i = 1; i<=nBins; i++)
	{
		errors->SetBinContent(i,i,data->GetBinError(i));
	}
	TH2D* unCov = tsvd->GetUnfoldCovMatrix(errors,100);

	D->Write();
	SV->Write();
	//Cov->Write();
	unCov->Write();

	//propagating stat errors
	for(int i=1; i<=nBins; i++)
	{
		for(int j=1; j<=nBins; j++)
		{
			result->SetBinError(j,result->GetBinError(j)+unCov->GetBinContent(i,j));
		}
	}

	genMC->Scale((double)nBins/((150.0*genMC->Integral(1,150))));

	genMC->Write();
	recoMC->Write();
	response->Write();
	data->Write();
	result->Write();

	TH1D* resultDiff;
	resultDiff = (TH1D*)result->Clone(title);
	resultDiff->SetDirectory(0);
	resultDiff->Scale((double)nBins/(150.0*resultDiff->Integral(1,150)));

	resultDiff->Write();
	std::cout << (150.0/(double)nBins)*resultDiff->Integral(1,150) << std::endl;
	std::cout << (150.0/(double)nBins)*genMC->Integral(1,150) << std::endl;

	delete ntup;
	delete tsvd;
	delete errors;
	delete genMC;
	delete recoMC;
	delete response;

	fin->Close();
	dataF->Close();
	outFile->Close();

	return resultDiff;
}