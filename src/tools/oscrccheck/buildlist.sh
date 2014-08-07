#!/bin/bash

checkcrc=$1
crcfile=$2
srcpath=$3

function usage()
{
    echo -e "\nusage: $0 <path_to_checkcrc> <path_to_crcfile> <path_to_source>\n";
}

if [ ! -f $checkcrc ] ; then
    usage
    echo -e "\t You have to provide the path to the checkcrc utility\n";
    exit 1;
fi

if [ ! -f $crcfile ] ; then
    usage
    echo -e "\t You have to provide the path to the checkcrc file\n";
    exit 1;
fi

first=$(head -n1 $crcfile | sed -e 's/\\/\//' | sed -e 's/\r//' | cut -d ';' -f2)
if [ ! -d $srcpath ] ; then
    usage
    echo -e "\t You have to provide the path to the sources\n";
    exit 1;
fi

first=$(realpath $PWD/$srcpath/$first)
if [ ! -f $first ] ; then
    usage
    echo -e "\t The provided path does not contain the correct sources\n";
    exit 1;
fi

checksums=$(cat $crcfile | sed -e 's/\\/\//' | sed -e 's/\r//')

if [ -e $crcfile.bak ] ; then
    rm $crcfile.bak
fi

for lin in $checksums ; do
    filename=${lin#*;}
    linpath=$(realpath $PWD/$srcpath/${lin#*;})
    crc=$($checkcrc -f $linpath | cut -f 1)
    crc=${crc#* }
    echo -e $crc";"$filename"\r" >> $crcfile.bak
done

mv $crcfile.bak $crcfile