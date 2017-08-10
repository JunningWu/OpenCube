#!/bin/bash

function clean_all {

	for i in `ls`
	do
		if [ -d "$i" ]  ; then
			cd $i
			clean_all
			cd ..
		else
			if [ "Makefile" = "$i" ] ; then
				CUR_DIR=`pwd | awk 'BEGIN {FS="/"} {print $NF}'` 
				make distclean &> /dev/null
				echo "Clean $CUR_DIR"
			fi
		fi   
	done
}

cd scope
clean_all
