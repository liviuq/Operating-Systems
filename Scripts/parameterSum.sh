#!bin/bash

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

echo "Suma este" $sum 