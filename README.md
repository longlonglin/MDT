# Code for MDT  Algorithm

This repository contains a reference implementation of the algorithms for the paper:

Pingpeng Yuan, Longlong Lin*, Chunxue Zhu, Hai Jin. Anonymous. Submit to ICDE2024.

## Environment

Codes run on a server with an Xeon 2.00GHz and 256GB memory running Ubuntu 18.04

## Dataset file format

- Each event (i.e., temporal edge) consists of vertex1, vertex2, timestamp

	- vertex1, vertex2: participate in the vertices of the event

	- timestamp: the time when the event happens

- the timestamp of any event must be sorted in ascending order

## Compile and Run

The code includes three search strategies: global strategy, local strategy, and index strategy.

Graph_IO.h reads the dataset from /data and stores it in memory.

global.h implements the global strategy: it first calculates the temporal support(TSup) value for all edges, then iteratively removes the edge with the minimum temporal support, and updates the temporal support of other edges within the same triangle. This process continues until the current query vertex q cannot participate in any triangle, and the algorithm terminates.

local_search.h implements the local strategy: it starts with the edges induced by the query vertex q, calculates their temporal support, and obtains the upper and lower bounds of k*. It uses binary search to select an expansion threshold k and collects the edges with global temporal support greater than k as the expansion subgraph. Then, based on the global strategy, it searches for the target community within the current expansion subgraph. Initially, the value of k is large, so the expansion subgraph may be relatively small. It may take several rounds of expanding the subgraph to find the final target community. Therefore, each subsequent round of expansion is built upon the previous round, and the edges that were not included in the expansion subgraph under the current k are marked for priority checking in the next round.

build_index.h builds the index: it calculates the temporal support of all edges from 0 to tmax. The specific implementation follows the ideas presented in the referenced paper, with the main idea being to minimize accessing edges and triangles and calculate the temporal support at \delta based on \delta-1.

index_search.h implements the index strategy: it utilizes the query index to quickly determine the value of k* by obtaining the temporal support of edges induced by q. Therefore, it only needs to continuously expand outward using the edges in the target community to quickly determine whether the current edge can appear in the final target community.

print.h contains some print output functions.

/data: contains processed datasets.

/index: contains the built index.

/seed: contains various selected query nodes, which can provide you with a reference.

To compile and run the code:

g++ -std=c++11 main.cpp -o main // Compile

./main 				// Run

Ps: Modify the function calls in the main function to invoke different algorithms.
