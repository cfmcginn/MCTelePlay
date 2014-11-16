//=============================================                      
// Author: Chris McGinn
//                     
// DiJet Histogram Maker, Missing Pt
//                                                                            
//=============================================     

#include <string>
#include <vector>
#include <algorithm>

#include "TTree.h"
#include "TDatime.h"
#include "TFile.h"
#include "MuonTree.h"

#include <fstream>
#include <iostream>

TFile* outFile_p = 0;


Float_t getMedian(std::vector<Float_t> inVect)
{
  std::sort(inVect.begin(), inVect.end());

  if(inVect.size()%2 == 0) return inVect.at(inVect.size()/2);
  else return inVect.at((inVect.size()+1)/2);
}


void findPeak(std::vector<Float_t>* inVect_p, Int_t& peakStart, Int_t& peakEnd)
{
  Float_t median = getMedian(*inVect_p);
  median -= 0.5;

  std::cout << median << std::endl;

  peakStart = -1;
  peakEnd = -1;
  const Int_t peakWidth = 10;

  Int_t nentries = (Int_t)(inVect_p->size());

  for(Int_t i = 0; i < nentries; i++){
    Bool_t peakBool = true;
    for(Int_t j = i; j < i+peakWidth; j++){
      if(inVect_p->at(j) > median){
	peakBool = false;
	break;
      }
    }
    if(peakBool){
      peakStart = i;
      break;
    }
  }

  for(Int_t i = peakStart; i < nentries; i++){
    Bool_t peakBool = true;
    for(Int_t j = i; j < i+peakWidth; j++){
      if(inVect_p->at(j) < median){
        peakBool = false;
        break;
      }
    }
    if(peakBool){
      peakEnd = i;
      break;
    }
  }
  
  return;
}


int makeMuonTree(const std::string fList = "", Int_t num = 0)
{
  std::string buffer;
  std::vector<std::string> listOfFiles;
  int nLines = 0;
  ifstream inFile(fList.data());

  std::cout << fList << std::endl;
  std::cout << inFile.is_open() << std::endl;

  if(!inFile.is_open()){
    std::cout << "Error opening file. Exiting." <<std::endl;
    return 1;
  }
  else{
    while(true){
      inFile >> buffer;
      if(inFile.eof()) break;
      listOfFiles.push_back(buffer);
      nLines++;
    }
  }

  std::cout << "FileList Loaded" << std::endl;

  for(Int_t iter = 0; iter < (Int_t)(listOfFiles.size()); iter++){
    std::cout << listOfFiles[iter] << std::endl;
  }

  std::cout << "FileJob: " << listOfFiles[num] << std::endl;

  //  setFileTag(inName);

  std::string outName = listOfFiles[num];
  const std::string cullString = "/";
  const std::string cutString = ".csv";
  const std::string repString = "";

  std::cout << "Cull string" << std::endl;
  while(true){
    std::size_t strIndex = outName.find(cullString);
    if(strIndex == std::string::npos) break;
    outName.replace(0, strIndex + 1, "");
  }

  std::cout << "Replace string" << std::endl;
  std::size_t strIndex = outName.find(cutString);
  if(!(strIndex == std::string::npos)){
    outName.replace(strIndex, cutString.length(), repString);
  }

  std::cout << outName << std::endl;

  TFile* outFile_p = new TFile(Form("%s_MuonTree_%d.root", outName.c_str(), num), "UPDATE");
  InitMuonTree();

  std::ifstream csvFile(listOfFiles[num].c_str());
  Int_t iter = 0;  
  std::vector<Float_t>* timeStamp_p = new std::vector<Float_t>;
  std::vector<Float_t>* voltOutCh1_p = new std::vector<Float_t>;
  std::vector<Float_t>* voltOutCh2_p = new std::vector<Float_t>;
  std::string outVal;

  while(true){
    //    std::cout << iter << std::endl;
    if(iter%3!=2) std::getline(csvFile, outVal, ',');
    else std::getline(csvFile, outVal);

    if(csvFile.eof()) break;

    std::string cutString2 = "\\^M";
    std::size_t strIndex2 = outVal.find(cutString2);
    if(!(strIndex2 == std::string::npos)) outName.replace(strIndex2, cutString2.length(), "");

    if(iter > 2){
      if(iter%3 == 0) timeStamp_p->push_back(std::stof(outVal));
      if(iter%3 == 1) voltOutCh1_p->push_back(std::stof(outVal));
      if(iter%3 == 2) voltOutCh2_p->push_back(std::stof(outVal));
    }

    //    std::cout << iter << ": " << outVal << std::endl;
    iter++;
  }

  Int_t nentries = (Float_t)(voltOutCh1_p->size());
  for(Int_t j = 0; j < nentries; j++){
    //    std::cout << j << ": " << voltOutCh1_p->at(j) << ", " << voltOutCh2_p->at(j) << std::endl;
    timeStamp_[j] = timeStamp_p->at(j);
    voltCh1_[j] = voltOutCh1_p->at(j);
    voltCh2_[j] = voltOutCh2_p->at(j);
  }

  findPeak(voltOutCh1_p, peakStartCh1_, peakEndCh1_);
  findPeak(voltOutCh2_p, peakStartCh2_, peakEndCh2_);

  for(Int_t j = peakStartCh1_; j < peakEndCh1_; j++){
    peakSumCh1_ += voltOutCh1_p->at(j);
  }

  for(Int_t j = peakStartCh2_; j < peakEndCh2_; j++){
    peakSumCh2_ += voltOutCh2_p->at(j);
  }

  muonTree_p->Fill();
  muonTree_p->Write("", TObject::kOverwrite);

  timeStamp_p->clear();
  voltOutCh1_p->clear();
  voltOutCh2_p->clear();
  delete timeStamp_p;
  delete voltOutCh1_p;
  delete voltOutCh2_p;

  CleanupMuonTree();
  outFile_p->Close();
  delete outFile_p;

  return 1;
}


int main(int argc, char* argv[])
{
  if(argc != 3){
    std::cout << "Usage: makeMuonTree <inputFile> <#>" << std::endl;
    return 1;
  }

  int rStatus = -1;

  rStatus = makeMuonTree(argv[1], atoi(argv[2]));

  return rStatus;
}
