#pragma once
#include"temp_truss.h"

using namespace std;


double comp_TC(tgraph& G, vector<sedge>& ans_edge)
{
	set<int> ans;
		
	for (auto& eg : ans_edge)
	{
		ans.insert(eg.first);
		ans.insert(eg.second);
	}


	int tcut=0, tvol1=0, tvol2=0;
	for (int i = 0; i < G.maxid + 1; i++)
	{
		int u = i;
		if (G.nodes[i].TE.size() == 0)
			continue;
		bool u_isin = ans.find(u) == ans.end() ? 0 : 1;
		for (auto& mp : G.nodes[i].TE)
		{
			int v = mp.first, tnum = mp.second.TS.size();
			if (u > v)
				continue;
			
			bool v_isin = ans.find(v) == ans.end() ? 0 : 1;
			if (u_isin ^ v_isin)			
				tcut += tnum;
			if (u_isin && v_isin)		
				tvol1 += tnum;
			if (!u_isin && !v_isin)	
				tvol2 += tnum;
		}
	}

	return (double)tcut / (min(tvol1, tvol2));
}



double comp_EDB(tgraph& G, vector<sedge>& ans_edge)
{
	set<int> ans;

	for (auto& eg : ans_edge)
	{
		ans.insert(eg.first);
		ans.insert(eg.second);
	}


	int in_edge=0, min_t=INT16_MAX, max_t=0;
	for (int i = 0; i < G.maxid + 1; i++)
	{
		int u = i;
		if (G.nodes[i].TE.size() == 0)
			continue;
		bool u_isin = ans.find(u) == ans.end() ? 0 : 1;
		for (auto& mp : G.nodes[i].TE)
		{
			int v = mp.first, tnum = mp.second.TS.size();
			if (u > v)
				continue;

			bool v_isin = ans.find(v) == ans.end() ? 0 : 1;
			if (u_isin && v_isin)			
			{
				in_edge += tnum;
				min_t = min(mp.second.TS[0], min_t);
				max_t = max(mp.second.TS.back(), max_t);
			}
			
		}
	}

	int S = ans.size();
	return double(in_edge) / S / (S - 1) / (max_t - min_t + 1);
}