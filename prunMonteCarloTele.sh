#!/bin/bash                                                                           

if [ $# -ne 0 ]
then
  echo "Usage: ./prunMonteCarloTele.sh"
  return
fi

now="mcJob_$(date +"%m_%d_%Y__%H_%M_%S")"
mkdir $now

NAME="runMonteCarloTele.cpp"
g++ $NAME $(root-config --cflags --libs) -Werror -Wall -O2 -o "${NAME/%.cpp/}.exe" -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 

mv *.exe $now