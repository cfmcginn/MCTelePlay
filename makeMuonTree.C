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

const Int_t nHeaderLines = 200;

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


Float_t getCutMean(std::vector<Float_t> inVect, Int_t nCut, Int_t startCut[][nPeakSums], Int_t stopCut[][nPeakSums])
{
  for(Int_t iter = nCut-1; iter >= 0; iter--){
    inVect.erase(inVect.begin() + startCut[iter][0], inVect.begin() + stopCut[iter][0]);
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
  endPos = endPos - peakWidth;

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
    if(i == endPos - 1){
      peakEnd = endPos+peakWidth;
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


int makeMuonTree(const std::string fList = "", const Bool_t isCh2 = false)
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
  /*
  for(Int_t fileIter = 0; fileIter < (Int_t)(listOfFiles.size()); fileIter++){
    std::cout << listOfFiles[fileIter] << std::endl;
  }
  */
  //  setFileTag(inName);

  std::string outName = fList;
  const std::string cullString = "/";
  const std::string cutString[3] = {".csv", ".lvm", ".txt"};
  const std::string repString[3] = {"", "", ""};

  std::cout << "Cull string" << std::endl;
  while(true){
    std::size_t strIndex = outName.find(cullString);
    if(strIndex == std::string::npos) break;
    outName.replace(0, strIndex + 1, "");
  }

  for(Int_t iter = 0; iter < 3; iter++){
    std::cout << "Replace string" << std::endl;
    std::size_t strIndex = outName.find(cutString[iter]);
    if(!(strIndex == std::string::npos)){
      outName.replace(strIndex, cutString[iter].length(), repString[iter]);
    }
  }

  std::cout << outName << std::endl;

  TFile* outFile_p = new TFile(Form("%s_MuonTree.root", outName.c_str()), "UPDATE");
  InitMuonTree(isCh2);

  for(Int_t fileIter = 0; fileIter < (Int_t)(listOfFiles.size()); fileIter++){
    if(fileIter%100 == 0) std::cout << "Event Number: " <<  fileIter << std::endl;
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
        if(iter%2==0) std::getline(csvFile, outVal, ',');
        else std::getline(csvFile, outVal);

        if(csvFile.eof()) break;

	std::string cutString2 = "\\^M";
	std::size_t strIndex2 = outVal.find(cutString2);
        if(!(strIndex2 == std::string::npos)) outName.replace(strIndex2, cutString2.length(), "");

	if(iter%2 == 0) timeStamp_p->push_back(std::stof(outVal));
	if(iter%2 == 1) voltOutCh1_p->push_back(std::stof(outVal));
      }
      iter++;
    }

    Int_t nentries = (Int_t)(voltOutCh1_p->size());
    for(Int_t j = 0; j < nentries; j++){
      timeStamp_[j] = timeStamp_p->at(j);
      voltCh1_[j] = voltOutCh1_p->at(j);
      if(isCh2) voltCh2_[j] = voltOutCh2_p->at(j);
      nCh_++;
    }

    meanCh1_[0] = getMean(voltOutCh1_p);

    for(Int_t peakIter = 0; peakIter < maxNPeaks; peakIter++){
      Int_t startPos = 0;
      if(nPeakCh1_ != 0) startPos = peakEndCh1_[peakIter-1][0];

      findPeak(voltOutCh1_p, meanCh1_[0], startPos, (Int_t)(voltOutCh1_p->size()), peakStartCh1_[peakIter][0], peakEndCh1_[peakIter][0]);
      if(peakStartCh1_[peakIter][0] == -1) break;

      for(Int_t sumIter = 1; sumIter < nPeakSums; sumIter++){
	peakStartCh1_[peakIter][sumIter] = peakStartCh1_[peakIter][0];
	peakEndCh1_[peakIter][sumIter] = peakEndCh1_[peakIter][0];
      }

      nPeakCh1_++;
    }

    meanCh1_[2] = getCutMean(*voltOutCh1_p, nPeakCh1_, peakStartCh1_, peakEndCh1_);
    meanCh1_[1] = (meanCh1_[2] + meanCh1_[0])/2.0;

    for(Int_t peakIter = 0; peakIter < nPeakCh1_; peakIter++){
      Int_t startPos = 0;
      if(peakIter != 0) startPos = peakEndCh1_[peakIter-1][1];
      Int_t endPos = voltOutCh1_p->size();
      if(peakIter != nPeakCh1_-1) endPos = peakStartCh1_[peakIter+1][1];

      refinePeak(voltOutCh1_p, meanCh1_[1], startPos, endPos, peakStartCh1_[peakIter][1], peakEndCh1_[peakIter][1]);
      refinePeak(voltOutCh1_p, meanCh1_[2], startPos, endPos, peakStartCh1_[peakIter][2], peakEndCh1_[peakIter][2]);

      for(Int_t sumIter = 0; sumIter < nPeakSums; sumIter++){
	peakSumCh1_[peakIter][sumIter] = getPeakSum(voltOutCh1_p, peakStartCh1_[peakIter][sumIter], peakEndCh1_[peakIter][sumIter]);
	peakStartTimeCh1_[peakIter][sumIter] = timeStamp_[peakStartCh1_[peakIter][sumIter]];
	peakEndTimeCh1_[peakIter][sumIter] = timeStamp_[peakEndCh1_[peakIter][sumIter]];
      }
    }

    if(isCh2){
      meanCh2_[0] = getMean(voltOutCh2_p);

      for(Int_t peakIter = 0; peakIter < maxNPeaks; peakIter++){
	Int_t startPos = 0;
	if(nPeakCh2_ != 0) startPos = peakEndCh2_[peakIter-1][0];

	findPeak(voltOutCh2_p, meanCh2_[0], startPos, (Int_t)(voltOutCh2_p->size()), peakStartCh2_[peakIter][0], peakEndCh2_[peakIter][0]);
	if(peakStartCh2_[peakIter][0] == -1) break;

	for(Int_t sumIter = 1; sumIter < nPeakSums; sumIter++){
	  peakStartCh2_[peakIter][sumIter] = peakStartCh2_[peakIter][0];
	  peakEndCh2_[peakIter][sumIter] = peakEndCh2_[peakIter][0];
	}
	nPeakCh2_++;
      }      

      meanCh2_[2] = getCutMean(*voltOutCh2_p, nPeakCh2_, peakStartCh2_, peakEndCh2_);
      meanCh2_[1] = (meanCh2_[2] + meanCh2_[0])/2.0;

      for(Int_t peakIter = 0; peakIter < nPeakCh2_; peakIter++){
	Int_t startPos = 0;
	if(nPeakCh2_ != 0) startPos = peakEndCh2_[peakIter-1][1];
	Int_t endPos = voltOutCh2_p->size();
	if(nPeakCh2_ != maxNPeaks-1) endPos = peakStartCh2_[peakIter+1][1];
	
	refinePeak(voltOutCh2_p, meanCh2_[1], startPos, endPos, peakStartCh2_[peakIter][1], peakEndCh2_[peakIter][1]);
	refinePeak(voltOutCh2_p, meanCh2_[2], startPos, endPos, peakStartCh2_[peakIter][2], peakEndCh2_[peakIter][2]);

	for(Int_t sumIter = 0; sumIter < nPeakSums; sumIter++){
	  peakSumCh2_[peakIter][sumIter] = getPeakSum(voltOutCh2_p, peakStartCh2_[peakIter][sumIter], peakEndCh2_[peakIter][sumIter]);
	  peakStartTimeCh2_[peakIter][sumIter] = timeStamp_[peakStartCh2_[peakIter][sumIter]];
	  peakEndTimeCh2_[peakIter][sumIter] = timeStamp_[peakEndCh2_[peakIter][sumIter]];
	}
      }
    }


    if(nPeakCh1_ < 2) nCh_ = 0;
    else if(TMath::Abs(peakSumCh1_[1][2]) > 5){
      std::ofstream outTxt;
      outTxt.open(Form("%s_TwoPeaks.txt", outName.c_str()), std::ios_base::app);
      outTxt << listOfFiles[fileIter].c_str() << std::endl;
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

  rStatus = makeMuonTree(argv[1], (Bool_t)(std::atoi(argv[2])));

  return rStatus;
}
