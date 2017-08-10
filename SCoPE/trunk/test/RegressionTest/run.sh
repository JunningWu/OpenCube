#!/bin/bash

# Define some colors first:
RED='\e[0;31m'
NC='\e[0m' # No Color

function test_all {
	for i in `ls`
	do
		if [ -d "$i" ]  ; then
			cd $i
			test_all
			cd ..
		else
			if [ "Makefile" = "$i" ] ; then
				CUR_DIR=`pwd | awk 'BEGIN {FS="/"} {print $NF}'` 
				make clean &> /dev/null
				make &> /tmp/scope_regression_test
				if [ "$?" = 0 ] ; then
					./run.x &> /tmp/scope_regression_test
					if [ "$?" = 0 ] ; then
						echo "Test $CUR_DIR OK"
					else
						echo -e "${RED}Test $CUR_DIR FAILED${NC}"
						cat /tmp/scope_regression_test
					fi
				else
					echo -e "${RED}make $CUR_DIR FAILED${NC}"
					cat /tmp/scope_regression_test
				fi
			fi
		fi   
	done
}

cd scope
test_all
rm /tmp/scope_regression_test
