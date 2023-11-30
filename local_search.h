#pragma once
#include"temp_truss.h"
#include"triangle_count.h"
#include"LinearHeap.h"
#include"print.h"
#include<sys/time.h>

using namespace std;

int local_access_edge_num;
int local_access_node_num;

unordered_set<int> acc_node;
unordered_set<int> acc_edge;


unordered_set<int> scan_subgraph(tgraph& G, tgraph& Gs, vector<int>& vec, int maxnode)
{
	unordered_set<int> nid_edge;

	if (Gs.nodes.size() == 0)								
		Gs.edge_num = 0;


	if (Gs.nodes.size() == 0 || maxnode > Gs.maxid)			 
	{
		Gs.nodes.resize(maxnode + 1);
		Gs.vedge_set.resize(maxnode + 1);
		Gs.maxid = maxnode;
		//cout << "maxid: " << maxnode << endl;
	}

	
	int cur_eid = Gs.edge_num;								

	for (auto& eid : vec)
	{
		int id1 = G.eid_touv[eid].first, id2 = G.eid_touv[eid].second;
		Gs.nodes[id1].TE[id2].TS = G.nodes[id1].TE[id2].TS;
		Gs.nodes[id2].TE[id1].TS = G.nodes[id2].TE[id1].TS;
		if (id1 > id2)
			swap(id1, id2);
		Gs.vedge_set[id1][id2] = cur_eid;				   //u£¬v->eid
		sedge eg{ id1,id2 };
		Gs.eid_touv.emplace_back(eg);						
		nid_edge.insert(cur_eid);
		cur_eid++;
	}
	Gs.edge_num = cur_eid;
	
	//cout << "edge_num: " << Gs.eid_touv.size() << endl;
	Gs.eedge_sup.resize(cur_eid,-1);						  
	Gs.etriangle.resize(cur_eid);
	return nid_edge;
}



void update_sup(tgraph& G, unordered_set<int>& nid_edeg, int delta,int& maxsup)
{
	/*cout << "new_edge: " << nid_edeg.size() << endl;*/
	unordered_map<int, unordered_map<int, unordered_set<int>>> vis_trian;		
	for (auto& eid : nid_edeg)														
	{
		int u = G.eid_touv[eid].first, v = G.eid_touv[eid].second;
		maxsup=max(maxsup,count_edge_sup(G,eid, delta));

		for (auto& w_ifo : G.etriangle[eid])				
		{
			int w = w_ifo.first, tsup = w_ifo.second;
			if (tsup == 0)									
				continue;
			int x = u, y = v, z = w;
			sort_triangle_node(x, y, z);
			if (vis_trian[x][y].find(z) != vis_trian[x][y].end())
				continue;
			vis_trian[x][y].insert(z);

			int eiduw = u < w ? G.vedge_set[u][w] : G.vedge_set[w][u];
			int eidvw = v < w ? G.vedge_set[v][w] : G.vedge_set[w][v];

			if (nid_edeg.find(eiduw) == nid_edeg.end())			
				maxsup=max(maxsup,G.eedge_sup[eiduw] += tsup);

			if (nid_edeg.find(eidvw) == nid_edeg.end())			  
				maxsup=max(maxsup,G.eedge_sup[eidvw] += tsup);
		}
	}
}





void check_connect(tgraph& G, tgraph& Gs, vector<vector<sedge>>& solution, vector<int>& ans, int q, int delta, int k, vector<unordered_set<int>>& non_temptri)
{
	tgraph Gf;
	scan_subgraph(Gs, Gf, ans, Gs.maxid);								
	vector<bool> vis(Gf.edge_num,false);								
	vector<unordered_set<int>> trian_vis(Gf.edge_num);

	for (auto& v_ifo : Gf.nodes[q].TE)									
	{
		int u = q, v = v_ifo.first;
		if (u > v)														
			swap(u, v);
		int eid = Gf.vedge_set[u][v];									
		if (vis[eid] ==false)											
		{
			queue<int> que;												
			que.push(eid);
			vis[eid] = true;						
			vector<sedge> comu;											
			while (!que.empty())										
			{
				int eid = que.front();
				que.pop();
				int x = Gf.eid_touv[eid].first, y = Gf.eid_touv[eid].second;
				sedge eg{ x,y};
				comu.emplace_back(eg);											

				if (Gf.nodes[x].TE.size() > Gf.nodes[y].TE.size())				
					swap(x, y);
				for (auto& z_ifo : Gf.nodes[x].TE)
				{
					int z = z_ifo.first;
					if (x == z || y == z)
						continue;
					if (Gf.nodes[y].TE.find(z) != Gf.nodes[y].TE.end())
					{
						int eidxz, eidyz;
						if (x < z && y < z)
						{
							if (non_temptri[G.vedge_set[min(x, y)][max(x, y)]].find(z) != non_temptri[G.vedge_set[min(x, y)][max(x, y)]].end())			//trian_vis[eid].find(z) != trian_vis[eid].end() || 
								continue;
							//trian_vis[eid].insert(z);
							eidxz = Gf.vedge_set[x][z];
							eidyz = Gf.vedge_set[y][z];
						}
						else if (x < y && z < y)
						{
							eidxz = x < z ? Gf.vedge_set[x][z] : Gf.vedge_set[z][x];
							if (non_temptri[G.vedge_set[min(x, z)][max(x, z)]].find(y) != non_temptri[G.vedge_set[min(x, z)][max(x, z)]].end())		//trian_vis[eidxz].find(y) != trian_vis[eidxz].end() || 
								continue;
							//trian_vis[eidxz].insert(y);
							eidyz = Gf.vedge_set[z][y];
						}
						else if (z < x && y < x)//
						{
							eidyz = y < z ? Gf.vedge_set[y][z] : Gf.vedge_set[z][y];		//trian_vis[eidyz].find(x) != trian_vis[eidyz].end() || 
							if (non_temptri[G.vedge_set[min(z,y)][max(z,y)]].find(x) != non_temptri[G.vedge_set[min(z, y)][max(z, y)]].end())		
								continue;
							//trian_vis[eidyz].insert(x);
							eidxz = Gf.vedge_set[z][x];
						}
						
						if (!vis[eidxz])													
						{
							que.push(eidxz); vis[eidxz]=true;
						}

						if (!vis[eidyz])
						{
							que.push(eidyz); vis[eidyz] = true;
						}
					}
				}
			}
			if (comu.size() >= 3)
				solution.push_back(comu);

			//tgraph Gc;
			//vector<int> aedge;
			//for (auto& eg : comu)
			//	aedge.emplace_back(Gf.vedge_set[eg.first][eg.second]);
			//scan_subgraph(Gf, Gc, aedge, Gf.maxid);
			//count_global_sup(Gc, delta);
			//print_edge_unvalid(Gc, k);
		}
	}
}



vector<int> shrink_prune(tgraph& Gs, int q, int maxsup, int delta, int& ansk,int midk)
{
	int q_edge = 0;
	q_edge = Gs.nodes[q].TE.size();						

	vector<bool> edge_del(Gs.edge_num,false);
	ListLinearHeap heap(Gs.edge_num, maxsup);			

	int* Ids = new int[Gs.edge_num];
	int* Keys = new int[Gs.edge_num];
	int i_ids = 0;

	int mmax = 0;
	for (int i=0;i<Gs.edge_num;i++)
	{
		Ids[i_ids] = i;
		Keys[i_ids] = Gs.eedge_sup[i];				
		i_ids++;
		
	}
	
	heap.init(Gs.edge_num, maxsup, Ids, Keys);			
	vector<int> recover_edge;							

	while (!heap.empty() && q_edge >= 2)
	{
		recover_edge.clear();						
		int eid_min, sup_min;
		heap.get_min(eid_min, sup_min);				
		ansk = sup_min;								
		
		while (!heap.empty() && q_edge >= 2)							
		{
			int eid_i, supl;
			heap.get_min(eid_i, supl);

			if (supl <= sup_min)					
			{
				edge_del[eid_i]=true;
				heap.pop_min(eid_i, supl);
				recover_edge.push_back(eid_i);
				
				int id1 = Gs.eid_touv[eid_i].first, id2 = Gs.eid_touv[eid_i].second;		
				if (q == id1 || q == id2)
					q_edge--;

				for (auto& id3_ifo : Gs.etriangle[eid_i])					
				{					
					int id3 = id3_ifo.first, dec = id3_ifo.second;
					if (dec == 0)
						continue;
					
					int eid13 = id1 < id3 ? Gs.vedge_set[id1][id3] : Gs.vedge_set[id3][id1];
					int eid23 = id2 < id3 ? Gs.vedge_set[id2][id3] : Gs.vedge_set[id3][id2];


					if (edge_del[eid13]==false && edge_del[eid23] == false)
					{
						if (heap.get_key(eid13) > sup_min)				  
							heap.decrement(eid13, dec);
						if (heap.get_key(eid23) > sup_min)
							heap.decrement(eid23, dec);
					}			
				}
			}
			else
				break;
		}
	}
	
	if ( ansk>=midk && q_edge < 2)			
	{
		while (!heap.empty())				
		{
			int e, s;
			heap.pop_min(e, s);
			recover_edge.emplace_back(e);
		}
	}
	
	/*for (auto& eg : recover_edge)
		cout << Gs.eid_touv[eg].first << "+" << Gs.eid_touv[eg].second << " ";
	cout << endl;*/

	delete[] Ids;
	delete[] Keys;
	heap.~ListLinearHeap();
	return recover_edge;
}


void expand(tgraph& G,vector<int>& expand_comu,vector<int>& new_edge, vector<unordered_set<int>>& non_temptri, queue<int>& que, vector<int8_t>& vis,int midk, int delta, int& maxnode, int check_size)
{

	vector<unordered_set<int>> trian_vis(G.edge_num);			
	unordered_set<int> cur_que;
	unordered_set<int> cur_vis;									

	while (!que.empty())																			
	{
		int cur = que.front();
		que.pop();
		int x = G.eid_touv[cur].first, y = G.eid_touv[cur].second;

		if ((check_size > 0 && G.eedge_sup[cur] >= midk) || check_size <= 0)					 
		{
			check_size--;
			expand_comu.emplace_back(cur);
			new_edge.emplace_back(cur);																
			maxnode = max(maxnode, max(x, y));
			vis[cur] = 1;															
			//maxsup = max(maxsup, G.eedge_sup[cur]);

			for (auto& z_ifo : G.etriangle[cur])		
			{
				int z = z_ifo.first;
				if (z_ifo.second == 0)				
				{
					if (y < z)
						non_temptri[cur].insert(z);
					else
						non_temptri[G.vedge_set[min(x, z)][max(x, z)]].insert(y);
					continue;
				}

				int eidxz, eidyz;

				if (y < z)						//x<y<z													
				{
					if (trian_vis[cur].find(z) != trian_vis[cur].end())
						continue;
					else
					{
						trian_vis[cur].insert(z);
						eidxz =G.vedge_set[x][z];
						if (count_edge_sup(G, eidxz, delta) < midk)
						{
							cur_que.insert(eidxz);
							vis[eidxz] = -1;
							continue;
						}
						eidyz = G.vedge_set[y][z];
						if (count_edge_sup(G, eidyz, delta) < midk)
						{
							cur_que.insert(eidyz);
							vis[eidyz] = -1;
							continue;
						}
					}
				}
				else                         //x<y,z<y
				{
					eidxz = x < z ? G.vedge_set[x][z] : G.vedge_set[z][x];
					if (trian_vis[eidxz].find(y) != trian_vis[eidxz].end())
						continue;
					else
					{
						trian_vis[eidxz].insert(y);
						if (count_edge_sup(G, eidxz, delta) < midk)
						{
							cur_que.insert(eidxz);
							vis[eidxz] = -1;
							continue;
						}
						eidyz = G.vedge_set[z][y];
						if (count_edge_sup(G, eidyz, delta) < midk)
						{
							cur_que.insert(eidyz);
							vis[eidyz] = -1;
							continue;
						}
					}
				}

				if (vis[eidxz]==0)			
				{
					que.push(eidxz);
					vis[eidxz] = 1;
				}

				if (vis[eidyz]==0)
				{
					que.push(eidyz); 
					vis[eidyz] = 1;
				}
			}
		}
		else
			cur_que.insert(cur);

	}

	for (auto& eg1 : cur_que)
		que.push(eg1);
}


vector<vector<sedge>> local_search(tgraph& G, int q, int delta,string file)
{
	int exp_time = 0, sc_time = 0, shrink_time = 0, check_time = 0;
	//timeval s_t, e_t;

	G.eedge_sup.clear();
	G.etriangle.clear();

	G.etriangle.resize(G.edge_num);
	G.eedge_sup.resize(G.edge_num,-1);			

	//cout << "q: " << q << "  ";

	int mink = INT16_MAX, maxk = 0;
	queue<int> que;
	vector<vector<sedge>> solution;

	int round=0,expand_size=0;

	for (const auto& v_ifo : G.nodes[q].TE)				
	{
		int u = q, v = v_ifo.first;
		if (u > v)
			swap(u, v);
		int eid = G.vedge_set[u][v];
		
		int esup = count_edge_sup(G, eid, delta);
		if (esup != 0)
			mink = min(mink, esup);
		maxk = max(maxk, esup);
	}

	//cout <<"q: "<< q<<" ori_mink: " << mink << "  ori_maxk: " << maxk << endl;

	if (maxk == 0)
	{
		string file_x = "./ans/expand_size/" + file + "_expand_size.txt";
		ofstream exp;
		exp.open(file_x, ios::app);
		assert(exp);

		//exp<<q<<":"<<expand_size<<" "<<0<<" "<<round<<endl;
	

		return solution;
	}
	int midk;

	
	vector<int> expand_comu;

	vector<unordered_set<int>> non_temptri(G.edge_num);			
	int maxnode = q, maxsup = 0;								
	tgraph Gs;
	vector<int8_t> vis(G.edge_num, 0);
	bool fir_round = true;

	timeval s_t, e_t;
	

	while (mink <= maxk)
	{
		round++;
		vector<int> new_edge;

		midk = (mink + maxk) / 2;

		if (fir_round)								
		{
			fir_round = false;
			for (const auto& v_ifo : G.nodes[q].TE)						
			{
				int u = q, v = v_ifo.first;
				if (u > v)
					swap(u, v);
				int eid = G.vedge_set[u][v];
				
				if (G.eedge_sup[eid] >= midk)					
				{
					que.push(eid);
					vis[eid] = 1;
				}
			}
			
			gettimeofday(&s_t, 0);
			expand(G, expand_comu,new_edge, non_temptri, que,vis, midk, delta, maxnode, 0);
			gettimeofday(&e_t, 0);
			exp_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;

			gettimeofday(&s_t, 0);
			scan_subgraph(G, Gs, new_edge, maxnode);	   
			maxsup=count_global_sup(Gs, delta);			
			gettimeofday(&e_t, 0);
			sc_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;
		}
		else
		{
			gettimeofday(&s_t, 0);
			expand(G,expand_comu, new_edge, non_temptri, que,vis, midk, delta, maxnode, que.size());
			gettimeofday(&e_t, 0);
			exp_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;

			gettimeofday(&s_t, 0);
			unordered_set<int> ngra_edge = scan_subgraph(G, Gs, new_edge, maxnode);	
			update_sup(Gs, ngra_edge, delta, maxsup);
			gettimeofday(&e_t, 0);
			sc_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;
		}

		expand_size=max(expand_size,(int)expand_comu.size());

		if (expand_comu.size() < 3)
		{
			maxk = midk;
			continue;
		}

		int ansk = 0;

		gettimeofday(&s_t, 0);
		vector<int> ans1 = shrink_prune(Gs, q, maxsup, delta, ansk,midk);		
		gettimeofday(&e_t, 0);
		shrink_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;

		// cout << "min-mid-max: " << mink << " " << midk << " " << maxk;
		// cout << "   ansk: " << ansk;
		// cout << "   expand size: " << expand_comu.size();
		// cout << "   expan time: " << exp_time << "ms";
		// cout << "   scan&comp time: " << sc_time << "ms";
		// cout << "   shrink time: " << shrink_time << "ms";

		if (ansk == 0)							
			maxk = midk;
		else if (ansk < midk)
		{
			mink = ansk; maxk = midk;
		}
		else if (ansk >= midk)
		{
			gettimeofday(&s_t, 0);
			check_connect(G, Gs, solution, ans1, q, delta, ansk, non_temptri);
			gettimeofday(&e_t, 0);
			check_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;
			// cout << "   check time: " << check_time << "ms" << endl;
			break;
		}		
	}

	string file_x = "./ans/expand_size/" + file + "_expand_size.txt";

	ofstream exp;
	exp.open(file_x, ios::app);
	assert(exp);

	exp<<q<<":"<<expand_size<<" "<<solution[0].size()<<" "<<round<<endl;
	exp.close();
	return solution;
}




vector<vector<sedge>> local_search(tgraph& G, int q, int delta)
{
	int exp_time = 0, sc_time = 0, shrink_time = 0, check_time = 0;
	//timeval s_t, e_t;

	G.eedge_sup.clear();
	G.etriangle.clear();

	G.etriangle.resize(G.edge_num);
	G.eedge_sup.resize(G.edge_num,-1);			

	// cout << "q: " << q << "  ";

	int mink = INT32_MAX, maxk = 0;
	queue<int> que;
	vector<vector<sedge>> solution;


	for (const auto& v_ifo : G.nodes[q].TE)				
	{
		int u = q, v = v_ifo.first;
		if (u > v)
			swap(u, v);
		int eid = G.vedge_set[u][v];
		
		int esup = count_edge_sup(G, eid, delta);
		if (esup != 0)
			mink = min(mink, esup);
		maxk = max(maxk, esup);
	}

	// cout << "ori_mink: " << mink << "  ori_maxk: " << maxk << endl;


	if (maxk == 0)
		return solution;
	int midk;

	
	vector<int> expand_comu;

	vector<unordered_set<int>> non_temptri(G.edge_num);			
	int maxnode = q, maxsup = 0;								
	tgraph Gs;
	vector<int8_t> vis(G.edge_num, 0);
	bool fir_round = true;

	timeval s_t, e_t;


	while (mink <= maxk)
	{
		vector<int> new_edge;

		midk = (mink + maxk) / 2;

		if (fir_round)								
		{
			fir_round = false;
			for (const auto& v_ifo : G.nodes[q].TE)							
			{
				int u = q, v = v_ifo.first;
				if (u > v)
					swap(u, v);
				int eid = G.vedge_set[u][v];
				
				if (G.eedge_sup[eid] >= midk)				
				{
					que.push(eid);
					vis[eid] = 1;
				}
			}
			
			gettimeofday(&s_t, 0);
			expand(G, expand_comu,new_edge, non_temptri, que,vis, midk, delta, maxnode, 0);
			gettimeofday(&e_t, 0);
			exp_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;

			gettimeofday(&s_t, 0);
			scan_subgraph(G, Gs, new_edge, maxnode);	   
			maxsup=count_global_sup(Gs, delta);			
			gettimeofday(&e_t, 0);
			sc_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;
		}
		else
		{
			gettimeofday(&s_t, 0);
			expand(G,expand_comu, new_edge, non_temptri, que,vis, midk, delta, maxnode, que.size());
			gettimeofday(&e_t, 0);
			exp_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;

			gettimeofday(&s_t, 0);
			unordered_set<int> ngra_edge = scan_subgraph(G, Gs, new_edge, maxnode);	   
			update_sup(Gs, ngra_edge, delta, maxsup);
			gettimeofday(&e_t, 0);
			sc_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;
		}

		if (expand_comu.size() < 3)
		{
			maxk = midk;
			continue;
		}

		int ansk = 0;

		gettimeofday(&s_t, 0);
		vector<int> ans1 = shrink_prune(Gs, q, maxsup, delta, ansk,midk);		
		gettimeofday(&e_t, 0);
		shrink_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;

		// cout << "min-mid-max: " << mink << " " << midk << " " << maxk;
		// cout << "   ansk: " << ansk;
		// cout << "   expand size: " << expand_comu.size();
		// cout << "   expan time: " << exp_time << "ms";
		// cout << "   scan&comp time: " << sc_time << "ms";
		// cout << "   shrink time: " << shrink_time << "ms";

		if (ansk == 0)						
			maxk = midk;
		else if (ansk < midk)
		{
			mink = ansk; maxk = midk;
		}
		else if (ansk >= midk)
		{
			gettimeofday(&s_t, 0);
			check_connect(G, Gs, solution, ans1, q, delta, ansk, non_temptri);
			gettimeofday(&e_t, 0);
			check_time = (e_t.tv_sec - s_t.tv_sec) * 1000 + (e_t.tv_usec - s_t.tv_usec) / 1000;
			// cout << "   check time: " << check_time << "ms" << endl;
			break;
		}		
	}
	return solution;
}