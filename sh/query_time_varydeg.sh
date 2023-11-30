#!/bin/bash
#sed -i 's/\r$//' 
file=("enron")

rank=(100)

for name in "${file[@]}"   
do
    for d in "${rank[@]}"
    do
       	./main_query_varydeg ${name} ${d} | tee ./ans/time/${name}_log_${d}.txt &
	wait
    done
		 
done
