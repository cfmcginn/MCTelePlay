//=============================================                      
// Author: Chris McGinn
//                     
// Muon Tree Maker
//                                                                            
//=============================================     

#include <string>
#include <vector>
#include <algorithm>

#include "TDatime.h"
#include "MuonTree.h"

#include <fstream>
#include <iostream>


Float_t getMean(std::vector<Float_t>* inVect_p)
{
  Float_t mean = 0;
  Int_t nentries = (Int_t)(inVect_p->size());
  for(Int_t iter = 0; iter < nentries; iter++){
    mean += inVect_p->at(iter);
  }

  return mean/nentries;
}


Float_t getMedian(std::vector<Float_t> inVect)
{
  std::sort(inVect.begin(), inVect.end());

  if(inVect.size()%2 == 0) return inVect.at(inVect.size()/2);
  else return inVect.at((inVect.size()+1)/2);
}


Float_t getCutMean(std::vector<Float_t> inVect, Int_t startCut, Int_t stopCut)
{
  inVect.erase(inVect.begin() + startCut, inVect.begin() + stopCut);

  return getMean(&inVect);
}


Float_t getCutMedian(std::vector<Float_t> inVect, Int_t startCut, Int_t stopCut)
{
  inVect.erase(inVect.begin() + startCut, inVect.begin() + stopCut);

  return getMedian(inVect);
}


void findPeak1(std::vector<Float_t>* inVect_p, Float_t& mean, Int_t& peakStart, Int_t& peakEnd)
{
  mean = getMean(inVect_p);
  //  const Float_t meanThresh = -0.5;
  //  mean += meanThresh;

  std::cout << "mean 1: " << mean << std::endl;

  peakStart = -1;
  peakEnd = -1;
  const Int_t peakWidth = 10;

  Int_t nentries = (Int_t)(inVect_p->size());

  for(Int_t i = 0; i < nentries; i++){
    Bool_t peakBool = true;
    for(Int_t j = i; j < i+peakWidth; j++){
      if(inVect_p->at(j) > mean){
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
      if(inVect_p->at(j) < mean){
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


void findPeak2(std::vector<Float_t>* inVect_p, Float_t& mean, Int_t& peakStart, Int_t& peakEnd)
{
  findPeak1(inVect_p, mean, peakStart, peakEnd);
  mean = getCutMean(*inVect_p, peakStart, peakEnd);
  //  const Float_t meanThresh = -0.5;
  //  mean += meanThresh;

  std::cout << "mean 2: " << mean << std::endl;

  Int_t nentries = (Int_t)(inVect_p->size());
  for(Int_t i = peakStart; i > 0; i--){
    if(inVect_p->at(i) > mean){
      peakStart = i;
      break;
    }
  }

  for(Int_t i = peakEnd; i < nentries; i++){
    if(inVect_p->at(i) > mean){
      peakEnd = i;
      break;
    }
  }
  
  return;
}


Float_t getPeakSum(std::vector<Float_t>* inVect_p, const Int_t peakStart, const Int_t peakEnd)
{
  Float_t sum = 0;
  for(Int_t iter = peakStart; iter < peakEnd; iter++){
    sum += inVect_p->at(iter);
  }
  return sum;
}


int makeMuonTree(const std::string fList = "")
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

  for(Int_t fileIter = 0; fileIter < (Int_t)(listOfFiles.size()); fileIter++){
    std::cout << listOfFiles[fileIter] << std::endl;
  }

  //  setFileTag(inName);

  std::string outName = listOfFiles[0];
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

  TFile* outFile_p = new TFile(Form("%s_MuonTree_MERGE.root", outName.c_str()), "UPDATE");
  InitMuonTree();

  for(Int_t fileIter = 0; fileIter < (Int_t)(listOfFiles.size()); fileIter++){
    std::cout << "Event Number: " <<  fileIter << std::endl;
    InitMuonVar();

    std::ifstream csvFile(listOfFiles[fileIter].c_str());
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

    findPeak1(voltOutCh1_p, meanCh1_[0], peakStartCh1_[0], peakEndCh1_[0]);
    findPeak1(voltOutCh2_p, meanCh2_[0], peakStartCh2_[0], peakEndCh2_[0]);
    findPeak2(voltOutCh1_p, meanCh1_[1], peakStartCh1_[1], peakEndCh1_[1]);
    findPeak2(voltOutCh2_p, meanCh2_[1], peakStartCh2_[1], peakEndCh2_[1]);

    peakSumCh1_[0] = getPeakSum(voltOutCh1_p, peakStartCh1_[0], peakEndCh1_[0]);
    peakSumCh1_[1] = getPeakSum(voltOutCh1_p, peakStartCh1_[1], peakEndCh1_[1]);
    peakSumCh2_[0] = getPeakSum(voltOutCh2_p, peakStartCh2_[0], peakEndCh2_[0]);
    peakSumCh2_[1] = getPeakSum(voltOutCh2_p, peakStartCh2_[1], peakEndCh2_[1]);

    muonTree_p->Fill();

    timeStamp_p->clear();
    voltOutCh1_p->clear();
    voltOutCh2_p->clear();
    delete timeStamp_p;
    delete voltOutCh1_p;
    delete voltOutCh2_p;
  }

  muonTree_p->Write("", TObject::kOverwrite);

  CleanupMuonTree();
  outFile_p->Close();
  delete outFile_p;

  return 1;
}


int main(int argc, char* argv[])
{
  if(argc != 2){
    std::cout << "Usage: makeMuonTree <inputFile>" << std::endl;
    return 1;
  }

  int rStatus = -1;

  rStatus = makeMuonTree(argv[1]);

  return rStatus;
}
