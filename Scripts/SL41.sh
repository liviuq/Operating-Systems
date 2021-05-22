#!bin/bash

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
done