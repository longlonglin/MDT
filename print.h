#pragma once
#include"temp_truss.h"
#include<iostream>

using namespace std;


void print_edge(tgraph& G)
{
	set<tuple<int,int,int>> vec;
	for (int i = 0; i < G.edge_num; i++)
	{
		int u = G.eid_touv[i].first, v = G.eid_touv[i].second;
		//cout << "u-v-sup: " << u << " " << v << " " << G.eedge_sup[i] << endl;
		vec.insert(make_tuple(u, v, G.eedge_sup[i]));
	}

	for(auto&tu:vec)
		cout << "eid-¡·u-v-sup: " <<G.vedge_set[get<0>(tu)][get<1>(tu)]<<" "<<get<0>(tu) << " " << get<1>(tu) << " " << get<2>(tu) << endl;
}


void print_edge_unvalid(tgraph& G,int k)
{
	int flag = 0;
	for (int i = 0; i < G.edge_num; i++)
	{
		if (G.eedge_sup[i] < k)
		{
			cout << "unquliad_edge: (" << G.eid_touv[i].first << " " << G.eid_touv[i].second << ")-->"<< G.eedge_sup[i] << endl;
			flag = 1;
		}
	}

	if (flag == 0)
		cout << "ans all edge greater than k!" << endl;
}


void print_triangle_unvalid(tgraph& G)
{
	int flag = 0, num = 0;
	for (int i = 0; i < G.maxid + 1; i++)
		for (auto& trian : G.vtriangle[i])
		{
			for (auto& mp : trian.second)
			{				
				if (i < trian.first && trian.first < mp.first && mp.second == 0)
				{
					flag = 1;
					num++;
					cout << "(" << i << "," << trian.first << "," << mp.first << ") " << mp.second << endl;
				}
			}
		}
	if (flag == 0)
		cout << "ans all triangle is temporal!" << endl;
	else
		cout << "di_temporal triangle nums is: " << num << endl;
}



void print_ans(vector<vector<sedge>>& ans)
{
	if (ans.size() == 0)
		cout << "There is no qualified community!" << endl;
	
	for (auto& vec : ans)
	{
		cout << "edge_size: " << vec.size();
		set<int> node;
		for (auto& eg : vec)
		{
			//cout << "(" << eg.first << "," << eg.second << ") ";
			node.insert(eg.first);
			node.insert(eg.second);
		}
		cout <<" node_size: "<< node.size() << endl;
		cout<<"node: ";
		for (auto& id : node)
			cout << id << " ";
		cout<<endl<<"edge: ";
		for (auto& eg : vec)
		{
			cout << "(" << eg.first << "," << eg.second << ") ";
		}
		cout << endl;
	}
}


void print_index_for_record(tgraph& G)
{
	for(int i=0;i<G.edge_num;i++)
	{
		int eid = i;
		if (G.skytruss[i].size() == 0)
			continue;
		cout << eid << " ";
		for (auto& kd : G.skytruss[i])
		{
			cout <<kd.first << " " << kd.second<< " ";
		}
		cout << endl;
	}
}


void print_index(tgraph& G)
{
	for (int i = 0; i < G.edge_num; i++)
	{
		int eid = i;
		if (G.skytruss[i].size() == 0)
			continue;
		cout << eid << "--> "<<G.eid_touv[eid].first<<" "<<G.eid_touv[eid].second<<" " ;
		for (auto& kd : G.skytruss[i])
		{
			cout << "("<<kd.first << "," << kd.second << ") ";
		}
		cout << endl;
	}
}


void print_triangle(tgraph G)
{
	set<tuple<int, int, int, int>> tri;
	for (int i = 0; i < G.edge_num; i++)
	{
		if (G.etriangle[i].size() == 0)
			continue;
		int u = G.eid_touv[i].first, v = G.eid_touv[i].second;
		for (auto& trian : G.etriangle[i])
		{			
			int w = trian.first;

			//if (G.vedge_set[u].count(w) == 0 && G.vedge_set[w].count(u) == 0)
			//	cout << "id1-id3" << u << " " << w << endl;
			//if (G.vedge_set[v].count(w) == 0 && G.vedge_set[w].count(v) == 0)
			//	cout << "id1-id3" << v << " " << w << endl;;

			if(trian.first>v)
			{
				//tri.insert(make_tuple(u, v, trian.first, trian.second));
				int dec1 = trian.second, dec2 = G.etriangle[G.vedge_set[min(u, w)][max(u, w)]][v], dec3 = G.etriangle[G.vedge_set[min(v, w)][max(v, w)]][u];
				if(dec1!=dec2 || dec1!=dec3 || dec2!=dec3)
					cout <<i<< "-->(" << u << "," << v << "," << trian.first << ") " << trian.second<<" "<<dec2<<" "<<dec3 << endl;
			}
		}
	}

	/*cout << "the number of  non-triangles£º " << tri.size() << endl;
	for (auto& tu : tri)
	{
		cout << "(" << get<0>(tu) << "," << get<1>(tu) << "," << get<2>(tu) << ") " << get<3>(tu) << endl;
	}*/
}



void print_trian(map<int, map<int, map<int, int>>>& mtrian)
{
	
	for (auto& eg : mtrian)
	{
		for (auto& seg : eg.second)
		{
			for (const auto& tr : seg.second)
			{
				int u = eg.first, v = seg.first, w = tr.first;
				cout << "(" << u << "," << v << "," << w << ") " << tr.second << endl;
			}
		}
	}
	cout << endl;
}



void print_time()
{
	cout << "trans_t: " << (double)trans_t << "ms" << endl;
	cout << "weight_t " << (double)weight_t << "ms" << endl;
	cout << "upsup_t: " << (double)upsup_t << "ms" << endl;
	cout << "laysup: " << (double)laysup_t << "ms" << endl;
	
	cout << endl;

	cout << "decom_t: " << (double)decom_t << "ms" << endl;
	cout << "truss_t: " << (double)truss_t << "ms" << endl;

	cout << endl;
	
}


void print_to_file(tgraph& G,string file)
{
	string efile = "./index/" + file + "_edge.txt";
	string tfile = "./index/" + file + "_tri.txt";

	ofstream wedge, wtri;
	wedge.open(efile, ios::out | ios::trunc);
	wtri.open(tfile, ios::out | ios::trunc);
	for (int i = 0; i < G.edge_num; i++)
	{
		int eid = i;
		if (G.skytruss[i].size() == 0)
			continue;
		wedge << eid << " ";
		for (auto& kd : G.skytruss[i])
		{
			wedge << kd.first << " " << kd.second << " ";
		}
		wedge << endl;
	}
	wedge.close();


	for (int i = 0; i < G.edge_num ; i++)
	{
		if (G.index_train[i].size() == 0)
			continue;
		int u = G.eid_touv[i].first, v = G.eid_touv[i].second;
		wtri << u << " " << v << " ";
		for (auto& id3 : G.index_train[i])
		{
			if(id3.first> u && id3.first>v)
				wtri << id3.first << " " << id3.second << " ";
		}
		wtri << endl;
	}
	wtri.close();
}



void print_tdtri(tgraph& G)
{
	for (int i = 0; i < G.maxid + 1; i++)
	{
		if (G.tdtri[i].size() == 0)
			continue;
		int u = G.eid_touv[i].first, v = G.eid_touv[i].second;
		for (auto& id2_ifo : G.tdtri[i])
		{	
			int w = id2_ifo.second, delta = id2_ifo.first;
			if (w > u && w > v)
				cout << u << " " << v << " " << w << " " << delta << endl;
						
		}
		cout << endl;
	}
}

