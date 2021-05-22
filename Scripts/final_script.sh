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

parcurgere $directory#!bin/bash

declare -i number=0

if [ $# -eq 0 ]
then
    read -p "Introduceti numarul pana la care se va face suma! " number
else
    number=$1
    shift
fi

declare -i sum=0
declare -i currentElement=0
for currentElement in $(seq 1 $number)
do
    sum=sum+currentElement
    echo $currentElement
done

echo "Suma este" $sum #!bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: $0 <list_of_numbers_to_list_>"
    exit -1
else
    list=$@
fi



declare -i pos=1
for param_i in $list
do
    echo "parameter #$pos=$param_i"
    pos=pos+1
done#!bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: $0 <file_path>"
    exit 1
else
    file=$1
fi

declare -i val1=0
declare -i val2=0
declare -i rez=0
declare -i nrop=0

echo "" > $file
while [ true ]
do
    read -p "Primul nr: " val1
    read -p "Al doilea nr: " val2
    read -p "Operatorul: " operator

    case "$operator" in
        "+" ) rez=val1+val2 ;;
        "-" ) rez=val1-val2 ;;
        "*" ) rez=val1*val2 ;;
        "/" ) rez=val1/val2 ;;
        "q" ) break         ;;
    esac
    nrop=nrop+1
    echo "$nrop:  $val1 $operator $val2 = $rez" >> $file
done


echo "Numarul total de operatii efectuate: $nrop" >> $file#!bin/bash

if [ $# -eq 0 ]
then
    read -p "Introduceti directorul" directory
else
    #directory=$1
    if [ ! -d $1 -o ! -r $1 ]
    then
        echo "directorul nu exista sau nu ai drepturi sa intri in el!"
        exit 1
    else
        directory=$1
    fi
fi

function compilare()
{
    gcc -Wall $1 -o ${1%.*}
}

function afisare()
{
    cat $1
}

files=$(ls $directory)

for fileToBeCompiled in $files
do
    #cat $fileToBeCompiled
    #gcc -Wall $fileToBeCompiled -o ${fileToBeCompiled%.*}

    case $fileToBeCompiled in
        *.c ) compilare $fileToBeCompiled ; echo $?;;
        *.txt ) afisare $fileToBeCompiled ;;
    esac
done#!/bin/bash
### Calculează recursiv factorial de n. ###

if [ $# -eq 0 ]
then
    read -p "Dati n: " n
else
    n=$1
fi

function factorial()
{
    if [ $1 -le 1 ]
    then
        echo 1
    else
        let n_1=$1-1
        rez=$(factorial $n_1)
        expr $1 \* $rez
    fi
}

echo -n "Factorial($n)="
factorial $n#!/bin/bash

if [ ! $# -eq 2 ]
then
    read -p "Introduceti cele doua valori, n si m: " n m
else
    n=$1
    m=$2
fi
echo "$m si $n"