#!/bin/bash
#sed -i 's/\r$//' 
#file=("facebook" "lkml" "twitter" "enron" "rmin" "primaryschool" "lyon" "thiers" "dblp")

delta=(2 4 8 10 12 14)

for name in "${file[@]}"   
do
    for d in "${delta[@]}"
    do
       	./main_query_time ${name} ${d} | tee ./ans/time/${name}_log_${d}.txt &
	wait
    done
		 
done
