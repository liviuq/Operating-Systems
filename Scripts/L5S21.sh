#!/bin/bash

if [ $# -eq 0 ]
then
    read -p "Introduceti directorul" directory
else
    directory=$1
fi

declare -i nrHid=0
declare -i nrAlias=0
declare -i nrSubdir=0

function passthrough()
{
    files=$(ls -A $1)
    for file in $files
    do
        path=$1/$file

        if [ ${file:0:1} == "." ]
        then
            nrHid=nrHid+1
        fi


        if [ -d $path ]
        then
            nrSubdir=nrSubdir+1
            passthrough $path
        fi

        if [ -L $file ]
        then
            nrAlias=nrAlias+1
        fi
    done
}

passthrough $directory

echo "$directory are $nrHid fisiere ascunse, $nrAlias fisiere alias si $nrSubdir subdirectoare"