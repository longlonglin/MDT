#!/bin/bash
#sed -i 's/\r$//' 
file=("facebook" "lkml" "twitter" "enron" "rmin" "primaryschool" "lyon" "thiers" "dblp")

delta=(8)

for name in "${file[@]}"   
do
    for d in "${delta[@]}"
    do
       	./main_query_ans ${name} ${d} | tee ./ans/node/${name}_log_${d}.txt &
	wait
    done
		 
done
