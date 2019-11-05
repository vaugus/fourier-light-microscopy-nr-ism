#!/bin/bash

# save output in txt file
# echo -n ${var//,/.} > $DIR/time_wrapper_output/$2.txt]

DIR=$(pwd)
TIMEFORMAT=%R

for (( i=5; i<=100; i=i+5))
do
	var=$({ time $1 $2/$i'.txt'; } 2>&1)
	echo -n ${var//,/.}
	echo
done

# for a in {10..60}
# do
# 	var=$({ time $1 $2; } 2>&1)
# 	echo -n ${var//,/.}
# 	echo
# done
