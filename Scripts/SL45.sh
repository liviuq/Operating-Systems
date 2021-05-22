#!/bin/bash

if [ ! $# -eq 2 ]
then
    read -p "Introduceti cele doua valori, n si m: " n m
else
    n=$1
    m=$2
fi
echo "$m si $n"