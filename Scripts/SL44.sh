#!/bin/bash
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
factorial $n