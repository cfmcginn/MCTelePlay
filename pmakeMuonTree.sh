#!/bin/bash

if [ $# -ne 3 ]
then 
  echo "Usage: ./pmakeMuonTree.sh <inputList> <isCh2> <outDir>"
  exit 1
fi

now="muonTreeJob_$(date +"%m_%d_%Y__%H_%M_%S")"
mkdir $now
mkdir -p $3
len=`wc -l $1 | awk '{print $1}'`
cp makeMuonTree.sh $now
cp $1 $now

NAME="makeMuonTree.C"
g++ -std=c++11  $NAME $(root-config --cflags --libs) -Werror -Wall -O2 -o "${NAME/%.C/}.exe" 
cp makeMuonTree.exe $now

cat pmakeMuonTree.condor | sed "s@log_flag@$now@g" | sed "s@dir_flag@$PWD/$now@g" | sed "s@arg1@$1@g" | sed "s@arg2@$2@g" | sed "s@arg3@$3@g" | sed "s@njobs@$len@g" > $now/pmakeMuonTree.condor

echo -=-
cat $now/pmakeMuonTree.condor
echo -=-
