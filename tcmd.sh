#!/bin/bash

handle_error(){
	echo "\n------------------------\n"
	echo "An Error Occurred on line $1"
	exit 1
}

trap 'handle_error $LINENO' ERR

echo 
echo "########## 1/3 ##########"
echo
test_build/qwmockdatagenerator -r 4 -e 1:2000 --config qwparity_simple.conf --detectors mock_detectors.map --data testing/coda3/

echo 
echo "########## 2/3 ##########"
echo
test_build/qwparity -r 4 -e 1:2000 --config qwparity_simple.conf --detectors mock_detectors.map --data testing/coda3 --rootfiles testing/coda3

echo 
echo "########## 3/3 ##########"
echo
test_build/qwroot -l -q testing/compare.C
