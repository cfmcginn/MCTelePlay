#!/bin/bash

if [ $# -ne 3 ]
then 
  echo "Usage: ./makeMuonTree.sh <inputList> <outDir> <#>"
  exit 1
fi

echo | awk -v inputList=$1 -v num=$3 '{print "./makeMuonTree.exe \""inputList"\" \""num"\""}' | bash

mv *MuonTree*.root $2
rm *.root 

echo "job done successfully"