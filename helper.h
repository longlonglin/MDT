#pragma once
#include"temp_truss.h"
#include"index_search.h"


void select_target_seed(tgraph& G,string file)
{
	string index = "./index/" + file + "_edge.txt";

	string sort_truss = "./seed/help_seed/" + file + "_truss.txt";
	string seed_truss = "./seed/help_seed/" + file + "_seed.txt";
	string nd_truss = "./seed/help_seed/" + file + "_node.txt";

	recover_index(G, index);
	
	vector < set<pair<int, int>>> node_truss(G.maxid+1);

	for (int i = 0; i < G.edge_num; i++)
	{
		int eid = i;
		int u = G.eid_touv[eid].first, v = G.eid_touv[eid].second;
		for (auto& pa : G.skytruss[i])
		{
			node_truss[u].insert({ pa.first,pa.second });
			node_truss[v].insert({ pa.first,pa.second });
		}
	}


	ofstream fout, seed,nod;
	fout.open(sort_truss,ios::out | ios::trunc);
	seed.open(seed_truss, ios::out | ios::trunc);
	nod.open(nd_truss, ios::out | ios::trunc);

	
	vector<int> s_node;

	for (int i = 0; i < G.maxid + 1; i++)
	{
		if (node_truss[i].size() == 0)
			continue;
		fout << i << " ";
		for (auto& pa : node_truss[i])
		{
			fout << pa.first << " " << pa.second << " ";

			if (pa.first <= 6 && pa.second >= 50)
			{
				nod << i <<" delta: "<<pa.first<<endl;
				s_node.push_back(i);
				break;
			}
		}
		fout << endl;
	}

	fout.close();
	nod.close();

	set<int> query_set;
	int n = s_node.size();

	int round_num = 100;

	while (round_num)
	{
		int q_index = 0;
		int q;
		while (1)
		{
			q_index = rand() % n;

			q = s_node[q_index];
			if (query_set.find(q) == query_set.end())
				break;
		}
		seed << q << endl;
		round_num--;
	}
	
}