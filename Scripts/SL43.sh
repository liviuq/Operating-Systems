#!bin/bash

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
done