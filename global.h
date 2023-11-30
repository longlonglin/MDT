#pragma once
#include"temp_truss.h"
#include"triangle_count.h"
#include"LinearHeap.h"
#include"local_search.h"
#include<sys/time.h>

vector<vector<sedge>> global_search(tgraph& G, int q, int delta)
{
	vector<vector<sedge>> solution;
	vector<unordered_set<int>> non_temptri;		


	G.eedge_sup.clear();
	G.etriangle.clear();

	G.etriangle.resize(G.edge_num );
	G.eedge_sup.resize(G.edge_num,-1);
	non_temptri.resize(G.edge_num);


	int ansk = 0, maxsup = 0;

	timeval s_t, e_t;
	int compsup_time = 0, shrink_time = 0, check_time = 0;

	gettimeofday(&s_t, 0);
	maxsup=count_global_sup(G, delta,non_temptri);
	gettimeofday(&e_t, 0);
	compsup_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;

	gettimeofday(&s_t, 0);
	vector<int> ans1=shrink_prune(G, q, maxsup,delta, ansk,0);				
	gettimeofday(&e_t, 0);
	shrink_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;

	//cout << "q: " << q<<"  ansk: " <<ansk;

	gettimeofday(&s_t, 0);
	check_connect(G,G, solution, ans1, q, delta, ansk, non_temptri);
	gettimeofday(&e_t, 0);
	check_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;

	// cout << "   compsup time: " << compsup_time << "ms";
	// cout << "   shrink time: " << shrink_time << "ms";
	// cout << "   check time: " << check_time << "ms"<<endl;

	//print_ans(solution);

	return solution;

}


