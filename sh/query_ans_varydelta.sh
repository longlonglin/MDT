#!/bin/bash
#sed -i 's/\r$//' 
file=("dblp")
rank=(2 4 6 10 12 14)
for name in "${file[@]}"   
do
    for d in "${rank[@]}"
    do
        ./query_ans_varydelta ${name} ${d} | tee ./try.txt &
	wait
    done
done