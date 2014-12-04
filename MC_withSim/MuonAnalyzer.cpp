// MuonAnalyzer.cpp : Defines the entry point for the console application.
//
#include "corrections.h"
#include "stdafx.h"
#include "TMath.h"
#include "TF1.h"


int _tmain(int argc, _TCHAR* argv[])
{
	TH1D::SetDefaultSumw2();
	TH2D::SetDefaultSumw2();

	int nBins = 60;
	float reso = 0.1;
	float EC = 1.65;
	float cutoff = 52.83;
	int reg = 3;
		
	TH1D * data; 
	data = unFold(reso ,EC , cutoff, nBins, reg, 0, "resultRaw");

	TH1D * data_EC_UP;
	data_EC_UP = unFold(reso ,EC+0.15 , cutoff, nBins, reg, 0, "EC_Up");

	TH1D * data_EC_DOWN;
	data_EC_DOWN = unFold(reso ,EC-0.15 , cutoff, nBins, reg, 0, "EC_Down");

	TH1D * data_Reso_UP; 
	data_Reso_UP = unFold(reso-0.05 ,EC , cutoff, nBins, reg, 0, "reso_Up");

	TH1D * data_Reso_DOWN; 
	data_Reso_DOWN = unFold(reso+0.05 ,EC , cutoff, nBins, reg, 0, "reso_Down");

	TH1D * data_Reg_UP; 
	data_Reg_UP = unFold(reso ,EC , cutoff, nBins, reg+1, 0, "reg_Up");

	TH1D * data_Reg_DOWN; 
	data_Reg_DOWN = unFold(reso ,EC , cutoff, nBins, reg-1, 0, "reg_Down");

	TH1D * data_MC_input; 
	data_MC_input = unFold(reso ,EC , cutoff, nBins, reg, 1, "MC_Input");

	data_EC_UP->Add(data,-1);
	data_EC_DOWN->Add(data,-1);
	data_Reso_UP->Add(data,-1);
	data_Reso_DOWN->Add(data,-1);
	data_Reg_UP->Add(data,-1);
	data_Reg_DOWN->Add(data,-1);
	data_MC_input->Add(data,-1);

	TH1D * sys_UP = new TH1D("sys_UP","sys_UP",nBins,0,150);
	TH1D * sys_DOWN = new TH1D("sys_DOWN","sys_DOWN",nBins,0,150);

	for(int i = 1; i<=nBins; i++)
	{
		float errorUp = TMath::Power(data_MC_input->GetBinContent(i),2);
		float errorDown = TMath::Power(data_MC_input->GetBinContent(i),2);

		if(data_EC_UP->GetBinContent(i) >0 || data_EC_DOWN->GetBinContent(i) >0) errorUp = errorUp + TMath::Power(TMath::Max(data_EC_UP->GetBinContent(i),data_EC_DOWN->GetBinContent(i)),2);
		if(data_EC_UP->GetBinContent(i) <0 || data_EC_DOWN->GetBinContent(i) <0) errorDown = errorDown + TMath::Power(TMath::Min(data_EC_UP->GetBinContent(i),data_EC_DOWN->GetBinContent(i)),2);

		if(data_Reso_UP->GetBinContent(i) >0 || data_Reso_DOWN->GetBinContent(i) >0) errorUp = errorUp + TMath::Power(TMath::Max(data_Reso_UP->GetBinContent(i),data_Reso_DOWN->GetBinContent(i)),2);
		if(data_Reso_UP->GetBinContent(i) <0 || data_Reso_DOWN->GetBinContent(i) <0) errorDown = errorDown + TMath::Power(TMath::Min(data_Reso_UP->GetBinContent(i),data_Reso_DOWN->GetBinContent(i)),2);

		if(data_Reg_UP->GetBinContent(i) >0 || data_Reg_DOWN->GetBinContent(i) >0) errorUp = errorUp + TMath::Power(TMath::Max(data_Reg_UP->GetBinContent(i),data_Reg_DOWN->GetBinContent(i)),2);
		if(data_Reg_UP->GetBinContent(i) <0 || data_Reg_DOWN->GetBinContent(i) <0) errorDown = errorDown + TMath::Power(TMath::Min(data_Reg_UP->GetBinContent(i),data_Reg_DOWN->GetBinContent(i)),2);

		errorUp = TMath::Power(errorUp,0.5);
		errorDown = TMath::Power(errorDown,0.5);

		sys_UP->SetBinContent(i,errorUp);
		sys_DOWN->SetBinContent(i,errorDown);
	}
		
	TFile * outf = new TFile("C:\\Users\\Austin\\Desktop\\8_811_Project\\Advanced_MC\\Unfolding\\Unfolding_working_directory\\result_and_systematics.root","recreate");
	data->Write();
	data_EC_UP->Write();
	data_EC_DOWN->Write();
	data_Reso_UP->Write();
	data_Reso_DOWN->Write();
	data_Reg_UP->Write();
	data_Reg_DOWN->Write();
	data_MC_input->Write();
	sys_UP->Write();
	sys_DOWN->Write();
	

	TF1 * theory = new TF1("theory","[0]*TMath::Power((x/52.83),2)*(6-4*(x/52.83)+0.0023*5.33*(5/(3*TMath::Power((x/52.83),2))+16*(x/52.83)/3.0+4./(x/52.83)+(12-8*(x/52.83))*TMath::Log(1/(x/52.83)-1)-8))",20/EC,cutoff);
	theory->SetParameter(0,1.0);
	double integral = 0;
	for(int i = 1+(int)10000*(20.0/EC/cutoff); i<10000; i++)
	{
		integral = integral + cutoff/10000.0 * (double)theory->Eval((double)i/10000.0*cutoff);
		//std::cout << i << theory->Eval((double)i/10000.0*cutoff) <<" "<< integral<< std::endl;
	}
	std::cout << integral << std::endl;
	theory->SetParameter(0,1.0/integral);
	theory->Write();
	return 0;
}

