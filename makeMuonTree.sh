#!/bin/bash

if [ $# -ne 2 ]
then 
  echo "Usage: ./makeMuonTree.sh <inputList> <outDir>"
  exit 1
fi

echo | awk -v inputList=$1 '{print "./makeMuonTree.exe \""inputList"\""}' | bash

mv *MuonTree*.root $2
rm *.root 

echo "job done successfully"