#!/bin/bash

if [ $# -lt 1 ]
then
    read -p "Introduceti directorul: " directory
else
    directory=$1
fi

function parcurgere()
{
    files=$(ls -A $1)

    for file in $files
    do
        path=$1/$file
        if [ -d $path ]
        then
            parcurgere $path
        else
            if [[ $path == *.sh ]]
            then
                cat $path >> final_script.sh
            fi
        fi
    done
}

parcurgere $directory