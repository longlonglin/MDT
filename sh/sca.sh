#!/bin/bash
#sed -i 's/\r$//'
file=("dblp_2s" "dblp_3s" "dblp_4s" "dblp_5s")

for f in "${file[@]}"   
do
   ./main_query_time ${f} 8 | tee ./ans/time/dblp_${f}__8_log_.txt &
   wait		 
done
