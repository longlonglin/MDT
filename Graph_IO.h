#pragma once
#include"temp_truss.h"
#include"assert.h"

using namespace std;


void getVE(tgraph& G,int& V, string filename) {
	int u, v, t, tmin = INT32_MAX, tmax = INT32_MIN;
	ifstream fin;
	fin.open(filename);
	assert(fin);

	while (fin.peek() != EOF) {
		fin >> u >> v >> t;
		//cout << u << " " << v << " " << t << " " << endl;
		V = max(V, max(u, v));    
		tmin = min(tmin, t); tmax = max(tmax, t);			   
	}
	G.tmax = tmax; G.tmin = tmin;
	fin.close();
}


void Readin(string filename, tgraph& G)
{
	int V = 0;
	getVE(G,V, filename);
	G.maxid = V;

	G.nodes.resize(V + 1);
	G.vedge_set.resize(V + 1);
	G.etuv.resize(G.tmax + 1);

	ifstream fin;
	fin.open(filename);
	assert(fin);
	int u, v, t;
	
	int eid = 0;
	while (fin>>u>>v>>t)
	{
		if (fin.eof())
			break;
		if (u == v)
			continue;
		if (u > v)
			swap(u, v);
		sedge eg({ u,v });
		if (G.nodes[u].TE.find(v) == G.nodes[u].TE.end())			
		{
			G.eid_touv.push_back(eg);	
			//G.eid_touv[eid] = make_pair(u, v);
			G.vedge_set[u][v] = eid;
			eid++;
		}
		if (find(G.nodes[v].TE[u].TS.begin(), G.nodes[v].TE[u].TS.end(), t) == G.nodes[v].TE[u].TS.end())		
		{
			G.nodes[v].TE[u].TS.push_back(t);  G.nodes[u].TE[v].TS.push_back(t);
			G.etuv[t].insert(G.vedge_set[u][v]);				
		}
		//G.vtuv[t][u].insert(v);
		G.etuv[t].insert(G.vedge_set[u][v]);
	}
	G.edge_num = eid;	
	G.msup = 0;
}