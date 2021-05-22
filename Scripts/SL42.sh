#!bin/bash

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


echo "Numarul total de operatii efectuate: $nrop" >> $file