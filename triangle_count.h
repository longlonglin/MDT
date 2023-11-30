#pragma once
#include"temp_truss.h"
#include"print.h"

using namespace std;



void sort_triangle_node(int& u, int& v, int& w)
{
	vector<int> sort_num;
	sort_num.push_back(u);
	sort_num.push_back(v);
	sort_num.push_back(w);
	sort(sort_num.begin(), sort_num.end());
	u = sort_num[0];
	v = sort_num[1];
	w = sort_num[2];
}



void sort_3vector(vector<int>& vec1, vector<int>& vec2, vector<int>& vec3)	
{

	int n1 = vec1.size();
	int n2 = vec2.size();
	int n3 = vec3.size();
	if (n1 < n2)    //vec1 vec2
	{
		if (n2 < n3)//vec1,vec2,vec3
		{
			return;
		}
		else   //  vec1 vec2, vec3 vec2
		{
			if (n1 < n3)   
			{
				vector<int> vec4 = vec2;
				vec2 = vec3;
				vec3 = vec4;
			}			
			else   //vec1 vec2, vec3 vec2 , vec3 vec1   --->vec3 vec1 vec2
			{
				vector<int> vec4 = vec3;				
				vec3 = vec2;
				vec2 = vec1;
				vec1 = vec4;
			}
		}
	}
	else         //vec2 vec1
	{
		if (n2 > n3)     
		{
			vector<int> vec4 = vec1;
			vec1 = vec3;
			vec3 = vec4;
		}
		else
		{
			if (n1 < n3)   
			{
				vector<int> vec4 = vec1;
				vec1 = vec2;
				vec2 = vec4;
			}
			else         
			{
				vector<int> vec4 = vec3;
				vec3 = vec1;
				vec1 = vec4;
			}
		}
	}
}



bool cmp_2dime_vec(vector<int>& vec1, vector<int>& vec2)
{
	if (vec1[2] != vec2[2])
		return vec1[2] < vec2[2];
	else
	{
		if (vec1[0] != vec2[0])
			return vec1[0] < vec2[0];
		else
			return vec1[1] < vec2[1];
	}
}


int find_max_interval(vector<int> vec1, vector<int> vec2, vector<int> vec3)
{
	return max(vec1.back() - vec2[0], max(vec1.back() - vec3[0],
		max(vec2.back() - vec1[0], max(vec2.back() - vec3[0],
			max(vec3.back() - vec1[0], vec3.back() - vec2[0])))));
}


void enum_static_triangle(tgraph& G)   
{
	int max_interval = 0;
	for (int i = 0; i < G.nodes.size(); i++)
	{
		for (auto& u2_ifo : G.nodes[i].TE)
		{
			int u = i;
			int v = u2_ifo.first;
			if (u < v)
			{
				if (G.nodes[i].TE.size() > G.nodes[v].TE.size())    
				{
					u = v; v = i;
				}

				for (auto& u3_ifo : G.nodes[u].TE)   
				{
					int w = u3_ifo.first;
					if (w == u || w == v)
						continue;
					int w1 = w, u1 = u, v1 = v;
					sort_triangle_node(u1, v1, w1);  
					if (G.nodes[v].TE.find(w) != G.nodes[v].TE.end())  
					{
						if (static_triangle[pair<int, int>(u1, v1)].find(w1) == static_triangle[pair<int, int>(u1, v1)].end())     
						{
							static_triangle[pair<int, int>(u1, v1)].insert(w1);  
							static_triangle[pair<int, int>(u1, w1)].insert(v1);
							static_triangle[pair<int, int>(v1, w1)].insert(u1);
							max_interval = max(find_max_interval(G.nodes[u1].TE[v1].TS, G.nodes[u1].TE[w1].TS, G.nodes[v1].TE[w1].TS), max_interval);
						}
					}
				}	
			}
		}
	}
	G.max_interval = max_interval;
}


int count_temp_triangle(vector<int> &vec1,vector<int>&vec2, vector<int>& vec3,int dealt)		
{
	int s1 = 0;
	int num = 0;
	for (int i = 0; i < vec1.size(); i++)
	{
		int flag1 = 1;
		int s2 = 0;
		for (int j = s1; j < vec2.size(); j++)
		{
			int flag2 = 1;           
			if (abs(vec2[j] - vec1[i]) <= dealt)
			{
				if (flag1)
				{
					s1 = j;        
					flag1 = 0;
				}
				for (int k = s2; k < vec3.size(); k++)    
				{
					if (abs(vec3[k] - vec2[j]) <= dealt)
					{
						if (flag2)
						{
							s2 = k;	flag2 = 0;
						}
						if (abs(vec3[k] - vec1[i]) <= dealt)
							num++;
					}
					if (vec3[k] - vec2[j] > dealt)
						break;
				}
				if (vec2[j] - vec1[i] > dealt)
					break;
			}

		}
	}
	return num;
}



int count_edge_sup(tgraph& G,int eid, int dealt)
{
	if (G.eedge_sup[eid] != -1)		
		return G.eedge_sup[eid];
	int esup = 0;
	int u = G.eid_touv[eid].first, v = G.eid_touv[eid].second;
	if (G.nodes[u].TE.size() > G.nodes[v].TE.size())    
		swap(u, v);
	for (auto& u3_ifo : G.nodes[u].TE)		
	{
		int w = u3_ifo.first;
		if (w == u || w == v)
			continue;

		if (G.nodes[v].TE.find(w) != G.nodes[v].TE.end())		
		{
			if (G.etriangle[eid].count(w) == 0)		
			{
				vector<int> vec1 = G.nodes[u].TE[v].TS;
				vector<int> vec2 = G.nodes[u].TE[w].TS;
				vector<int> vec3 = G.nodes[v].TE[w].TS;
				sort_3vector(vec1, vec2, vec3);		
				int trian_num= count_temp_triangle(vec1, vec2, vec3, dealt);
				
				G.etriangle[eid][w] = trian_num;
				G.etriangle[G.vedge_set[min(u, w)][max(u, w)]][v] = trian_num;
				G.etriangle[G.vedge_set[min(v, w)][max(v, w)]][u] = trian_num;
			}
			esup += G.etriangle[eid][w];
		}		
	}	
	G.eedge_sup[eid] = esup;
	return esup;
}



int count_global_sup(tgraph& G, int dealt)
{
	int maxsup = 0;
	for (int i = 0; i < G.edge_num; i++)
		maxsup = max(maxsup, count_edge_sup(G, i, dealt));
	return maxsup;
}


int count_edge_sup(tgraph& G, int eid, int dealt, vector<unordered_set<int>>& non_temptri)
{
	if (G.eedge_sup[eid] != -1)		
		return G.eedge_sup[eid];

	int esup = 0;
	int u = G.eid_touv[eid].first, v = G.eid_touv[eid].second;
	if (G.nodes[u].TE.size() > G.nodes[v].TE.size())    
		swap(u, v);
	for (auto& u3_ifo : G.nodes[u].TE)		
	{
		int w = u3_ifo.first;
		if (w == u || w == v)
			continue;
		if (G.nodes[v].TE.find(w) != G.nodes[v].TE.end())		
		{
			if (G.etriangle[eid].count(w) == 0)		
			{
				vector<int> vec1 = G.nodes[u].TE[v].TS;
				vector<int> vec2 = G.nodes[u].TE[w].TS;
				vector<int> vec3 = G.nodes[v].TE[w].TS;
				sort_3vector(vec1, vec2, vec3);		
				int trian_num = count_temp_triangle(vec1, vec2, vec3, dealt);


				int eiduw = G.vedge_set[min(u, w)][max(u,w)], eidvw = G.vedge_set[min(v,w)][max(v,w)];

				
				if (trian_num == 0)
				{
					if (u < w && v < w)						
						non_temptri[eid].insert(w);
					else if (v < u && w < u)
						non_temptri[eidvw].insert(u);
					else if (u < v && w < v)
						non_temptri[eiduw].insert(v);
				}

				G.etriangle[eid][w] = trian_num;
				G.etriangle[eiduw][v] = trian_num;
				G.etriangle[eidvw][u] = trian_num;
			}
		}
		esup += G.etriangle[eid][w];
	}
	G.eedge_sup[eid] = esup;
	return esup;
}


int count_global_sup(tgraph& G, int dealt, vector<unordered_set<int>>& non_temptri)
{
	int maxsup = 0;
	for (int i = 0; i < G.edge_num; i++)
		maxsup = max(maxsup, count_edge_sup(G,i, dealt,non_temptri));
	return maxsup;
}






void PopPre(pair<int, int> p1, int id1, int id2)
{
	int nbr, u_or_v, ex_uv;
	if (p1.first != id1 && p1.first != id2)
	{
		nbr = p1.first;
		u_or_v = p1.second;
	}
	else
	{
		nbr = p1.second;
		u_or_v = p1.first;
	}
	if (u_or_v == id1)
		ex_uv = id2;
	else
		ex_uv = id1;
	if (nbr != id1 && nbr != id2) {
		pre_node[pair<int, int>(nbr, u_or_v)]--;
		pre = pre - pre_node[pair<int, int>(nbr, ex_uv)];
	}
}

void PopPos(pair<int, int> p1, int id1, int id2)
{
	int nbr, u_or_v, ex_uv;
	if (p1.first != id1 && p1.first != id2)
	{
		nbr = p1.first;
		u_or_v = p1.second;
	}
	else
	{
		nbr = p1.second;
		u_or_v = p1.first;
	}
	if (u_or_v == id1)
		ex_uv = id2;
	else
		ex_uv = id1;
	if (nbr != id1 && nbr != id2) {
		pos_node[pair<int, int>(nbr, u_or_v)]--;
		pos = pos - pos_node[pair<int, int>(nbr, ex_uv)];
	}
}

void PushPre(pair<int, int> p1, int id1, int id2)
{
	int nbr, u_or_v, ex_uv;
	if (p1.first != id1 && p1.first != id2)
	{
		nbr = p1.first;
		u_or_v = p1.second;
	}
	else
	{
		nbr = p1.second;
		u_or_v = p1.first;
	}
	if (u_or_v == id1)
		ex_uv = id2;
	else
		ex_uv = id1;
	if (nbr != id1 && nbr != id2) {
		pre = pre + pre_node[pair<int, int>(nbr, ex_uv)];
		pre_node[pair<int, int>(nbr, u_or_v)]++;
	}
}

void PushPos(pair<int, int> p1, int id1, int id2)
{
	int nbr, u_or_v, ex_uv;
	if (p1.first != id1 && p1.first != id2)
	{
		nbr = p1.first;
		u_or_v = p1.second;
	}
	else
	{
		nbr = p1.second;
		u_or_v = p1.first;
	}
	if (u_or_v == id1)
		ex_uv = id2;
	else
		ex_uv = id1;
	if (nbr != id1 && nbr != id2) {
		pos = pos + pos_node[pair<int, int>(nbr, ex_uv)];
		pos_node[pair<int, int>(nbr, u_or_v)]++;
	}
}

void ProcessCurrent(pair<int, int> p1, int id1, int id2)
{
	int nbr, u_or_v, ex_uv;
	if (p1.first != id1 && p1.first != id2)
	{
		nbr = p1.first;
		u_or_v = p1.second;
	}
	else
	{
		nbr = p1.second;
		u_or_v = p1.first;
	}
	if (u_or_v == id1)
		ex_uv = id2;
	else
		ex_uv = id1;
	if (nbr != id1 && nbr != id2)
		mid = mid + pos_node[pair<int, int>(nbr, ex_uv)] - pre_node[pair<int, int>(nbr, ex_uv)];

	else
		counts = counts + pre + pos + mid;
}



void merge_together(tgraph &G,vector<vector<int>>& vec, int id1, int id2)
{
	if (id2 < id1)
		swap(id1, id2);
	vector<int> timelist = G.nodes[id1].TE[id2].TS;
	for (int i = 0; i < timelist.size(); i++)
	{
		vector<int> vec1(3);
		vec1[0] = id1;
		vec1[1] = id2;
		vec1[2] = timelist[i];
		vec.push_back(vec1);
	}
}

