#!/bin/bash

handle_error(){
	echo "\n------------------------\n"
	echo "An Error Occurred on line $1"
	exit 1
}

trap 'handle_error $LINENO' ERR

argcnt=$#
run1=4
run2=4
if [ ${argcnt} -eq 1 ] 
	then 
		run1=$1
		run2=$1
fi
if [ ${argcnt} -gt 1 ]
	then
		run1=$1
		run2=$2
fi

echo 
echo "########## 0/3 ##########"
echo
make -C test_build

echo 
echo "########## 1/3 ##########"
echo
test_build/qwmockdatagenerator -r ${run1} -e 1:2000 --config qwparity_simple.conf --detectors mock_detectors.map --data testing/coda3/

echo 
echo "########## 2/3 ##########"
echo
test_build/qwparity -r ${run1} -e 1:2000 --config qwparity_simple.conf --detectors mock_detectors.map --data testing/coda3 --rootfiles testing/coda3

echo 
echo "########## 3/3 ##########"
echo
test_build/qwroot -l -q 'testing/compare.C('${run1},${run2}')'
