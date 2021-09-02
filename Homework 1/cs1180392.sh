#!/bin/bash

if [ "$1" == "-apriori" ]
then
    ./apriori $2 $3 $4
elif [ "$2" == "-plot" ] 
then
    python3 plot.py "$1"
fi