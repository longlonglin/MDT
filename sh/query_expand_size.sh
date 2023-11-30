#!/bin/bash
#sed -i 's/\r$//' 
file=("primaryschool" "lyon" "thiers" "twitter" "facebook" "enron" "lkml" "dblp")
rank=(8)
for name in "${file[@]}"   
do
    for d in "${rank[@]}"
    do
        ./main_query_expand_ans_size ${name} ${d}  | tee ./try.txt &
	wait
    done
done