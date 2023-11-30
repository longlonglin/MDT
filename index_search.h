#pragma once
#include"temp_truss.h"
#include"triangle_count.h"
#include"LinearHeap.h"
#include"print.h"
#include"local_search.h"

using namespace std;



int  find_index_binary(tgraph& G, int eid, int delta)
{
	int ans = 0;
	int low = 0, high = G.skytruss[eid].size() - 1;
	while (low <= high)
	{
		int mid = low + ((high - low) >> 1);
		if (G.skytruss[eid][mid].first <= delta)
		{
			ans = mid;
			low = mid + 1;
		}
		else
			high = mid - 1;
	}
	return ans;
}



int find_index(tgraph& G, int eid, int delta)
{
	int max_k = 0;

	if (G.skytruss[eid].size() == 0)			
		return 0;

	for (auto dk : G.skytruss[eid])
	{
		int d1 = dk.first, k1 = dk.second;
		if (d1 > delta)							
			break;	
		max_k = max(max_k, k1);
	}
	return max_k;
}





bool is_qualified(tgraph& G, int eid, int delta, int k)
{
	if (G.skytruss[eid].size() == 0)			
		return false;

	/*int ans = 0;
	int low = 0, high = G.skytruss[eid].size() - 1;
	while (low <= high)
	{
		int mid = low + ((high - low) >> 1);
		if (G.skytruss[eid][mid].first <= delta)
		{
			ans = mid;
			low = mid + 1;
		}
		else
			high = mid - 1;
	}

	if (k <= G.skytruss[eid][ans].second)
		return true;
	return false;*/

	for (auto dk : G.skytruss[eid])
	{
		int d1 = dk.first, k1 = dk.second;
		if (delta >= d1 && k <= k1)       
			return true;
		else if (d1 > delta)			
			break;
	}
	return false;
}




vector<unordered_set<sedge, sedge_hash, sedge_equal>> index_search_withk(tgraph& G, int q, int delta, int k)
{
	unordered_set<sedge, sedge_hash, sedge_equal> vis;
	vector<unordered_set<sedge, sedge_hash, sedge_equal>> ans_comm;

	for (auto& v_ifo : G.nodes[q].TE)
	{
		int u = q, v = v_ifo.first;
		if (u > v)
			swap(u, v);
		sedge eg({ u,v });
		int eid = G.vedge_set[u][v];
		if (vis.find(eg) == vis.end() && is_qualified(G, eid, delta, k))
		{
			int maxid = 0;

			queue<sedge> que;
			que.push(eg);
			vis.insert(eg);
			unordered_set<sedge, sedge_hash, sedge_equal> comu;
			while (!que.empty())
			{
				sedge cur = que.front();
				que.pop();
				comu.insert(cur);
				int x = cur.first, y = cur.second;
				maxid = max(maxid,max(x, y));
				if (G.nodes[x].TE.size() > G.nodes[y].TE.size())				
					swap(x, y);
				for (auto& z_ifo : G.nodes[x].TE)								
				{
					int z = z_ifo.first;
					if (x == z || y == z)
						continue;
					int x0 = x, y0 = y, z0 = z;
					if (G.nodes[y].TE.find(z) != G.nodes[y].TE.end())
					{
						if (x0 > z0)
							swap(x0, z0);
						if (y0 > z)
							swap(y0, z);

						sedge exz = make_pair(x0, z0);
						sedge eyz = make_pair(y0, z);

						if (is_qualified(G, G.vedge_set[x0][z0], delta, k) && is_qualified(G, G.vedge_set[y0][z], delta, k))
						{
							if (vis.find(exz) == vis.end())
							{
								que.push(exz); vis.insert(exz);
							}
							if (vis.find(eyz) == vis.end())
							{
								que.push(eyz); vis.insert(eyz);
							}
						}
					}
				}
			}
			if (comu.size() >= 3)
			{
				ans_comm.push_back(comu);

				
				/*tgraph Gc = scan_subgraph(G, comu, maxid);
				count_global_sup(Gc, delta);
				print_edge_unvalid(Gc, k);*/
			}

			
		}
	}
	return ans_comm;
}



vector<vector<sedge>> index_search(tgraph& G, int q, int delta)
{
	//unordered_set<int> vis;
	vector<bool> vis(G.edge_num,false);
	vector<vector<sedge>> ans_comm;

	//set<tuple<int, int, int>> trian_vis;
	vector<unordered_set<int>> trian_vis(G.edge_num);

	int maxid = 0;

	int maxk = 0;
	cout << "q: "<<q <<"  " ;
	for (auto& v_ifo : G.nodes[q].TE)
	{
		int u = q, v = v_ifo.first;
		if (u > v)
			swap(u, v);
		int eid = G.vedge_set[u][v];
		
		int mk = find_index(G, eid, delta);
		maxk = max(maxk, mk);
	}

	cout << " maxk: " << maxk << endl;

	for (auto& v_ifo : G.nodes[q].TE)
	{
		int u = q, v = v_ifo.first;
		if (u > v)
			swap(u, v);
		//sedge eg({ u,v });
		int cur = G.vedge_set[u][v];

		bool is_valid = is_qualified(G, cur, delta, maxk);
		if (!vis[cur] && is_valid)
		{
			queue<int> que;
			que.push(cur);
			vis[cur] = true;
			vector<sedge> comu;
			while (!que.empty())
			{
				int eid = que.front();
				que.pop();

				int x = G.eid_touv[eid].first, y = G.eid_touv[eid].second;
				maxid = max(maxid, max(x, y));
				sedge eg{ x,y };
				comu.emplace_back(eg);
				//maxid = max(maxid, max(x, y));
				//if (G.nodes[x].TE.size() > G.nodes[y].TE.size())
				//	swap(x, y);
				//for (auto& z_ifo : G.nodes[x].TE)								
				//{
				//	int z = z_ifo.first;
				//	if (x == z || y == z)
				//		continue;
				//	int x0 = x, y0 = y, z0 = z;
				//	if (G.nodes[y].TE.find(z) != G.nodes[y].TE.end())
				//	{
				for(auto& pa:G.tdtri[eid])
				{
					int z = pa.second, d = pa.first;
					if (d > delta)										
						break;

					int eidxz, eidyz;
					if (y < z)
					{
						if (trian_vis[eid].find(z) != trian_vis[eid].end())
							continue;
						else
						{
							trian_vis[eid].insert(z);
							eidxz = G.vedge_set[x][z];
							eidyz = G.vedge_set[y][z];
						}
					}
					else
					{
						eidxz = x < z ? G.vedge_set[x][z] : G.vedge_set[z][x];
						if (trian_vis[eidxz].find(y) != trian_vis[eidxz].end())
							continue;
						else
						{
							trian_vis[eidxz].insert(y);
							eidyz =  G.vedge_set[z][y];
						}
					}

					if (is_qualified(G, eidxz, delta, maxk) && is_qualified(G, eidyz, delta, maxk))
					{
						if (vis[eidxz] == false)
						{
							que.push(eidxz); vis[eidxz]=true;
						}
						if (vis[eidyz] ==false)
						{
							que.push(eidyz); vis[eidyz]=true;
						}
					}
					//}
				}
			}
			if (comu.size() >= 3)
			{
				ans_comm.push_back(comu);
				
				
				/*tgraph Gc;
				vector<int> aedge;
				for (auto& eg : comu)
					aedge.emplace_back(G.vedge_set[eg.first][eg.second]);
				scan_subgraph(G,Gc, aedge, maxid);
				count_global_sup(Gc, delta);
				print_edge_unvalid(Gc, maxk);*/

				//tgraph Gc = scan_subgraph(G, comu, maxid);
				//count_global_sup(Gc, delta);
				////print_edge_unvalid(Gc,k);
				//print_triangle_unvalid(Gc);
			}
		}else if(!is_valid)
			vis[cur]=true;		
	}
	return ans_comm;
}




void Stringsplit(tgraph& G, string str, char split)
{
	istringstream iss(str);	
	string token;			
	int eid = -1, d = -1;
	while (getline(iss, token, split))
	{
		if (eid == -1)
			eid = atoi(token.c_str());
		else if (d == -1)
		{
			d = atoi(token.c_str());
		}
		else
		{
			int k = atoi(token.c_str());
			G.skytruss[eid].push_back(make_pair(d, k));
			d = -1;
		}
	}
}

void recover_index(tgraph& G, string str)
{
	G.skytruss.clear();
	G.skytruss.resize(G.edge_num + 1);
	char split = ' ';
	ifstream infile(str);
	assert(infile);

	string esky;
	while (getline(infile, esky)) {
		Stringsplit(G, esky, split);
	}

}




void Stringsplit_tri(tgraph& G, string str, char split)
{
	istringstream iss(str);	// 
	string token;			// 
	int id1 = -1,id2=-1, d = -1;
	while (getline(iss, token, split))	// 
	{
		if (id1 == -1)
			id1 = atoi(token.c_str());
		else if (id2 == -1)
		{
			id2 = atoi(token.c_str());
			//cout << id1 << " " << id2 << " ";
		}
		else if (d == -1)
		{
			d = atoi(token.c_str());
		}
		else
		{
			int k = atoi(token.c_str());
			//cout << d << " " << k << " ";
			G.tdtri[G.vedge_set[id1][id2]].insert({k,d });
			G.tdtri[G.vedge_set[id2][d]].insert({ k,id1 });
			G.tdtri[G.vedge_set[id1][d]].insert({ k,id2 });
			d = -1;
		}
		
	}
	//cout << endl;
}



void recover_triangle(tgraph& G, string str)
{
	G.tdtri.resize(G.edge_num);		
	char split = ' ';
	ifstream infile(str);
	assert(infile);

	string esky;
	while (getline(infile, esky)) {
		Stringsplit_tri(G, esky, split);
	}
	//print_tdtri(G);
}