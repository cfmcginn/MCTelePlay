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

const Int_t nHeaderLines = 21;

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


Float_t getCutMean(std::vector<Float_t> inVect, Int_t nCut, Int_t startCut[], Int_t stopCut[])
{
  for(Int_t iter = nCut-1; iter >= 0; iter--){
    inVect.erase(inVect.begin() + startCut[iter], inVect.begin() + stopCut[iter]);
  }

  return getMean(&inVect);
}


Float_t getCutMedian(std::vector<Float_t> inVect, Int_t startCut, Int_t stopCut)
{
  inVect.erase(inVect.begin() + startCut, inVect.begin() + stopCut);

  return getMedian(inVect);
}


void findPeak(std::vector<Float_t>* inVect_p, Float_t mean, Int_t startPos, Int_t endPos, Int_t& peakStart, Int_t& peakEnd)
{
  peakStart = -1;
  peakEnd = -1;
  const Int_t peakWidth = 10;

  for(Int_t i = startPos; i < endPos; i++){
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

  if(peakStart == -1) return;

  for(Int_t i = peakStart; i < endPos; i++){
    if(i == endPos - 1 - peakWidth){
      peakEnd = endPos;
      break;
    }

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


void refinePeak(std::vector<Float_t>* inVect_p, Float_t mean, Int_t startPos, Int_t endPos, Int_t& peakStart, Int_t& peakEnd)
{
  for(Int_t i = peakStart; i > startPos; i--){
    if(inVect_p->at(i) > mean){
      peakStart = i;
      break;
    }
    if(i ==  startPos+1) peakStart = startPos;
 }

 for(Int_t i = peakEnd; i < endPos; i++){
   if(inVect_p->at(i) > mean){
     peakEnd = i;
     break;
   }
   if(i == endPos - 1) peakEnd = endPos;
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


int makeMuonTree(const std::string fList = "", Bool_t isCh2 = false)
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
  InitMuonTree(isCh2);

  for(Int_t fileIter = 0; fileIter < (Int_t)(listOfFiles.size()); fileIter++){
    std::cout << "Event Number: " <<  fileIter << std::endl;
    InitMuonVar(isCh2);

    std::ifstream csvFile(listOfFiles[fileIter].c_str());
    Int_t iter = 0;  
    std::vector<Float_t>* timeStamp_p = new std::vector<Float_t>;
    std::vector<Float_t>* voltOutCh1_p = new std::vector<Float_t>;
    std::vector<Float_t>* voltOutCh2_p = new std::vector<Float_t>;
    std::string outVal;

    while(true){
      if(iter < nHeaderLines) std::getline(csvFile, outVal);
      else if(isCh2){
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
      }
      else{
        if(iter%2!=1) std::getline(csvFile, outVal, ',');
        else std::getline(csvFile, outVal);

        if(csvFile.eof()) break;

	std::string cutString2 = "\\^M";
	std::size_t strIndex2 = outVal.find(cutString2);
        if(!(strIndex2 == std::string::npos)) outName.replace(strIndex2, cutString2.length(), "");

        if(iter > 1){
          if(iter%2 == 0) timeStamp_p->push_back(std::stof(outVal));
          if(iter%2 == 1) voltOutCh1_p->push_back(std::stof(outVal));
        }
      }

      iter++;
    }

    Int_t nentries = (Float_t)(voltOutCh1_p->size());
    for(Int_t j = 0; j < nentries; j++){
      //    std::cout << j << ": " << voltOutCh1_p->at(j) << ", " << voltOutCh2_p->at(j) << std::endl;
      timeStamp_[j] = timeStamp_p->at(j);
      voltCh1_[j] = voltOutCh1_p->at(j);
      if(isCh2) voltCh2_[j] = voltOutCh2_p->at(j);
    }

    meanCh1_ = getMean(voltOutCh1_p);

    for(Int_t peakIter = 0; peakIter < maxNPeaks; peakIter++){
      Int_t startPos = 0;
      if(nPeakCh1_ != 0) startPos = peakEndCh1_[peakIter-1];

      findPeak(voltOutCh1_p, meanCh1_-0.1, startPos, (Int_t)(voltOutCh1_p->size()), peakStartCh1_[peakIter], peakEndCh1_[peakIter]);
      if(peakStartCh1_[peakIter] == -1) break;
      nPeakCh1_++;
    }

    meanCutCh1_ = getCutMean(*voltOutCh1_p, nPeakCh1_, peakStartCh1_, peakEndCh1_);

    for(Int_t peakIter = 0; peakIter < nPeakCh1_; peakIter++){
      Int_t startPos = 0;
      if(peakIter != 0) startPos = peakEndCh1_[peakIter-1];
      Int_t endPos = voltOutCh1_p->size();
      if(peakIter != nPeakCh1_-1) endPos = peakStartCh1_[peakIter+1];

      refinePeak(voltOutCh1_p, meanCutCh1_, startPos, endPos, peakStartCh1_[peakIter], peakEndCh1_[peakIter]);
      peakSumCh1_[peakIter] = getPeakSum(voltOutCh1_p, peakStartCh1_[peakIter], peakEndCh1_[peakIter]);
    }

    if(isCh2){
      meanCh2_ = getMean(voltOutCh2_p);

      for(Int_t peakIter = 0; peakIter < maxNPeaks; peakIter++){
	Int_t startPos = 0;
	if(nPeakCh2_ != 0) startPos = peakEndCh2_[peakIter-1];

	findPeak(voltOutCh2_p, meanCh2_, startPos, (Int_t)(voltOutCh2_p->size()), peakStartCh2_[peakIter], peakEndCh2_[peakIter]);
	if(peakStartCh2_[peakIter] == -1) break;
	nPeakCh2_++;
      }      

      meanCutCh2_ = getCutMean(*voltOutCh2_p, nPeakCh2_, peakStartCh2_, peakEndCh2_);

      for(Int_t peakIter = 0; peakIter < nPeakCh2_; peakIter++){
	Int_t startPos = 0;
	if(nPeakCh2_ != 0) startPos = peakEndCh2_[peakIter-1];
	Int_t endPos = voltOutCh2_p->size();
	if(nPeakCh2_ != maxNPeaks-1) endPos = peakStartCh2_[peakIter+1];
	
	refinePeak(voltOutCh2_p, meanCutCh2_, startPos, endPos, peakStartCh2_[peakIter], peakEndCh2_[peakIter]);
	peakSumCh2_[peakIter] = getPeakSum(voltOutCh2_p, peakStartCh2_[peakIter], peakEndCh2_[peakIter]);
      }
    }

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
  if(argc != 3){
    std::cout << "Usage: makeMuonTree <inputFile> <isCh2>" << std::endl;
    return 1;
  }

  int rStatus = -1;

  rStatus = makeMuonTree(argv[1], (Bool_t)argv[2]);

  return rStatus;
}
