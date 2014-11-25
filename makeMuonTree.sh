#!/bin/bash

if [ $# -ne 3 ]
then 
  echo "Usage: ./makeMuonTree.sh <inputList> <isCh2> <outDir>"
  exit 1
fi

echo $2
echo | awk -v inputList=$1 -v isCh2=$2 '{print "./makeMuonTree.exe \""inputList"\" \""isCh2"\""}' | bash

mv *MuonTree*.root $3
mv *TwoPeak*.txt $3
rm *.root 

echo "job done successfully"