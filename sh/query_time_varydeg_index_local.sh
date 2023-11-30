#!/bin/bash
#sed -i 's/\r$//' 
file=("dblp")

rank=(100 80 60 40 20)

for name in "${file[@]}"   
do
    for d in "${rank[@]}"
    do
       	./main_query_time_verydeg_index_local ${name} ${d} | tee ./ans/time/${name}_log_${d}.txt &
	wait

    done
		 
done
