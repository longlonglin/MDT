#pragma once
#include"temp_truss.h"
#include"LinearHeap.h"
#include"print.h"
//#include<sys/time.h>

using namespace std;

//if (id1 < id2 )				
					//{
					//	if (id2 < id3)			//id1,id2,id3
					//	{
					//		eid13 = G.vedge_set[id1][id3]; eid23 = G.vedge_set[id2][id3];	sub_trian[eid].insert(id3);
					//	}
					//	else if(id1 < id3)		//id1,id3,id2
					//	{
					//		eid13 = G.vedge_set[id1][id3]; eid23 = G.vedge_set[id3][id2]; sub_trian[eid13].insert(id2);
					//	}
					//	else                   //id3,id1,id2
					//	{
					//		eid13 = G.vedge_set[id3][id1]; eid23 = G.vedge_set[id3][id2];	sub_trian[eid13].insert(id2);
					//	}
					//}
					//else
					//{
					//	if (id1<id3)			//id2,id1,id3
					//	{
					//		eid23 = G.vedge_set[id2][id3]; eid13 = G.vedge_set[id1][id3];	sub_trian[eid].insert(id3);
					//	}
					//	else if (id2 < id3)		//id2,id3,id1
					//	{
					//		eid23 = G.vedge_set[id2][id3]; eid13 = G.vedge_set[id3][id1];	sub_trian[eid23].insert(id1);
					//	}
					//	else
					//	{
					//		eid23 = G.vedge_set[id3][id2]; eid13 = G.vedge_set[id1][id3];	sub_trian[eid23].insert(id1);
					//	}
					//}

void update_weight_vec(vector<vgraph>& VG, tgraph& G, int s, int e, vector<unordered_set<int>> & sub_trian)
{

	for (const auto& eid : G.etuv[e])			//������ʱ�̵ļ���ı�
	{		
		int id1 = G.eid_touv[eid].first, id2 = G.eid_touv[eid].second;

		if (VG[s].nbor[id1].find(id2) == VG[s].nbor[id1].end())			//id1��id2�����ھӣ�������̬���ǵ�һ�γ���
		{
			VG[s].subifo[eid].eweg = 1;										//��һ�γ��־ͽ�Ȩ����Ϊ1
			//int idx = VG[s].nbor[id1].size() < VG[s].nbor[id2].size() ? id1 : id2;
			if (VG[s].nbor[id1].size() > VG[s].nbor[id2].size())	    //��id1,id2���ڽڵ��ٵĵ㿪ʼ����Ѱ���乫������
				swap(id1, id2);
			for (const auto& id3 : VG[s].nbor[id1])						//id1ȡ���ڽڵ�
			{
				if (id3 == id1 || id3 == id2)
					continue;
				if (VG[s].nbor[id2].find(id3) != VG[s].nbor[id2].end())	//(id2,id3)���ڣ��γ����µ������Σ�����trian_sup��Ϊ0��������¾ͺ�һ��
				{
					
					int eid13 = G.vedge_set[min(id1, id3)][max(id1, id3)], eid23 = G.vedge_set[min(id2, id3)][max(id2, id3)];

					//sub_trian�Ѿ���֤������С���ߵ�id->,w	
					if (id1 < id3 && id2 < id3)						//��Ϊǰ��swap������id��id2�Ĵ�С����Ҫ�жϣ������ø�flag
						sub_trian[eid].insert(id3);
					else if (id2 < id1 && id3 < id1)
						sub_trian[eid23].insert(id1);
					else if(id1<id2 && id3<id2)
						sub_trian[eid13].insert(id2);								
					
					VG[s].subifo[eid].trifo[id3]=0;					//���������ε�supΪ0��ͬʱҲ������������
					VG[s].subifo[eid13].trifo[id2]=0;
					VG[s].subifo[eid23].trifo[id1]=0;
				}
			}
			VG[s].nbor[id1].insert(id2); VG[s].nbor[id2].insert(id1);	//�����ڽڵ�
		}
		else
		{																//������̬���Ѿ������ˣ�Ȩ��+1
			VG[s].subifo[eid].eweg++;
			for (auto&wtri : VG[s].subifo[eid].trifo)					//Ȩ�ظı��ˣ����������������ε�supֵ��Ȼ�仯
			{
				int id3 = wtri.first;


				if(id2<id3)												//��Ϊid1һ����С��id2�ģ�ֻ���ж�id2��id3�Ĺ�ϵ����
					sub_trian[eid].insert(id3);
				else
					sub_trian[G.vedge_set[min(id1, id3)][max(id1, id3)]].insert(id2);
			}
		}
	}
}


void update_sup_vec(tgraph& G, vector<vgraph>& VG, vector< unordered_map<int, int>>& fir_trian_up, int s, int e, vector<unordered_set<int>>& sub_trian)
{
	//sub_trian���ڸ���Ȩ�ؽ׶�ȷ����supֵ�ᷢ���ı������������, ������������Ѿ��ǰ�(��u��v)->eid,w)����ʽ�洢��
	for (int i = 0; i < sub_trian.size(); i++)
	{
		if (sub_trian[i].size() == 0)
			continue;
		int u = G.eid_touv[i].first, v = G.eid_touv[i].second;
		int wguv = VG[s].subifo[i].eweg;
		for (auto& w : sub_trian[i])
		{
			int wguw = VG[s].subifo[G.vedge_set[u][w]].eweg, wgvw = VG[s].subifo[G.vedge_set[v][w]].eweg;			//ȡ������Ȩ��
			int count = wguv * wguw * wgvw, incre = 0;			//����Ȩ�ػ���Ϊ��ǰ�������ڵ�supֵ

			incre = count - VG[s].subifo[i].trifo[w];		//��ǰsup-��һ��sup��Ϊsup����

			VG[s].subifo[i].trifo[w] = count;				//���������������ε�supֵ

			G.vtrian_up[i][w] += incre;						//����ȫ������
			if (s == G.tmin)									//��һ���������⴦��
				fir_trian_up[i][w] = incre;
		}
	}
}

//���߲���index_train��������ʱ���ж�����������ǲ���ʱ̬�����ξͺ�
void com_laysup_vec(tgraph& G, vector< unordered_map<int, int>>& cpy_trian_up, vector<unordered_map<int, int>>& fir_trian_up, int delta)
{
	if (delta == 0)
	{
		for (int i = 0; i < G.vtrian_up.size(); i++)
		{
			if (G.vtrian_up[i].size() == 0)
				continue;
			int u = G.eid_touv[i].first, v = G.eid_touv[i].second;
			for (auto& seg : G.vtrian_up[i])
			{
				int w = seg.first, sup = seg.second;
				int eiduw = G.vedge_set[u][w], eidvw = G.vedge_set[v][w];

				if (G.index_train[i].count(w) == 0)			//ֵΪ0��������������ε�һ�γ��֣��������������������������Ϣ
				{
					G.etriangle[eiduw][v] = 0;
					G.etriangle[eidvw][u] = 0;
					G.index_train[i][w] = delta;			//�洢��Ϊ������ʱ��delta��Ϣ
				}

				G.etriangle[i][w] += sup;
				//G.etriangle[v][w][u] += sup;
				//G.vtriangle[u][w][v] += sup;			//����Ŀǰ����д����Ϊ��Ҫ֪�����߲�������Щ������,��������һ���������ж�����������ǲ����Ѿ�������,��
				G.eedge_sup[i] += sup;
				G.eedge_sup[eiduw] += sup;				//�������ʱ�̫��ʱ��������Խ��ߵ�sup����������ʾ���������ǲ��ǣ���ʱ�Ĳ�����
				G.eedge_sup[eidvw] += sup;
				
				G.msup = max(max(G.msup, G.eedge_sup[i]), max(G.eedge_sup[eiduw], G.eedge_sup[eidvw]));
				G.vtrian_up[i][w] -= fir_trian_up[i][w];			//����G��trian_up,Ϊ��һ����׼��
			}
		}
	}
	else
	{
		for (int i = 0; i < G.vtrian_up.size(); i++)
		{
			if (G.vtrian_up[i].size() == 0)
				continue;
			int u = G.eid_touv[i].first, v = G.eid_touv[i].second;
			for (auto& seg : G.vtrian_up[i])
			{
				int w = seg.first;
				int eiduw = G.vedge_set[u][w], eidvw = G.vedge_set[v][w];

				int sup = seg.second - cpy_trian_up[i][w];			//��delta����0��ʱ��һ���ĵط�

				if (G.index_train[i].count(w) == 0)			//�ж��Ƿ��Ѿ���Ϊʱ̬������
				{
					G.etriangle[eiduw][v] += sup;
					G.etriangle[eidvw][u] += sup;
					G.index_train[i][w] = delta;					
				}

				G.etriangle[i][w] += sup;
				G.eedge_sup[i] += sup;				//�������ʱ�̫��ʱ��������Խ��ߵ�sup����������ʾ���������ǲ��ǣ���ʱ�Ĳ�����
				G.eedge_sup[eiduw] += sup;
				G.eedge_sup[eidvw] += sup;
				G.msup = max(max(G.msup, G.eedge_sup[i]), max(G.eedge_sup[eiduw], G.eedge_sup[eidvw]));
				G.vtrian_up[i][w] -= fir_trian_up[i][w];			//����G��trian_up,Ϊ��һ����׼��
			}
		}
	}
}


void transfor_layer_vec(tgraph& G, vector<vgraph>& VG, int delta)
{

	vector<unordered_map<int, int>> cpy_trian_up = G.vtrian_up;
	G.vtrian_up.clear();						//���㣬Ϊ�ӵ�һ�����������׼��
	G.vtrian_up.resize(G.edge_num);
	vector<unordered_map<int, int>> fir_trian_up(G.edge_num);		//�������һ�����������


	for (int i = G.tmin; i <= G.tmax - delta; i++)
	{
		vector<unordered_set<int>> sub_trian(G.edge_num);
		
		update_weight_vec(VG, G, i, i + delta, sub_trian);
		
		update_sup_vec(G, VG, fir_trian_up, i, i + delta, sub_trian);
		
	}

	com_laysup_vec(G, cpy_trian_up, fir_trian_up, delta);

	if (delta != 0)
		VG.pop_back();
}



void find_skytruss(tgraph& G, int delta, int k, int eid)
{
	if (delta == 0)
		return;
	if (G.skytruss[eid].size() == 0)
		G.skytruss[eid].push_back(pair<int, int>(delta, k));
	else
	{
		auto itor = G.skytruss[eid].back();        //����һ�ּ��𣿿��Ա�֤deltaԽ���ʱ��ߵ�trussֵ���ݼ�����-->�ǵģ���Ϊ����delta����ʱ����trussֵҲ���ԭ��������ͬ
		int delt1 = itor.first;
		int k1 = itor.second;
		if (k > k1)
		{
			G.skytruss[eid].push_back(pair<int, int>(delta, k));
		}
	}
}



void decompose_vec(tgraph& G, int delta)
{
	vector<int> edge_del(G.edge_num,0);					//�����б߱��Ϊδ����
	ListLinearHeap heap(G.edge_num, G.msup);			//��һ�����������ı�idֵ(������������ռ䣩���ڶ�������������valueֵ
	int* Ids = new int[G.edge_num];
	int* Keys = new int[G.edge_num];
	int i_ids = 0;

	for (int i = 0; i < G.edge_num; i++)
	{
		Ids[i_ids] = i;
		Keys[i_ids] = G.eedge_sup[i];		//��¼eid��Ӧ��supֵ
		i_ids++;
		int sup = G.eedge_sup[i];
		if (sup < 0)
			cout << "sup-i: " << sup << " " << i << endl;
	}
	heap.init(G.edge_num, G.msup, Ids, Keys);			//��ʼ����

	while (!heap.empty())
	{
		int eid_min, sup_min;
		heap.get_min(eid_min, sup_min);

		while (!heap.empty())
		{
			int eid, supl;
			heap.get_min(eid, supl);
			if (supl <= sup_min)
			{
				heap.pop_min(eid, supl);
				edge_del[eid]=1;
				if (sup_min == 0)
					continue;

				find_skytruss(G, delta, sup_min, eid);		//�ϲ����а�����ϵ��(k,truss)��

				int id1 = G.eid_touv[eid].first, id2 = G.eid_touv[eid].second;
				if (id1 > id2)
					swap(id1, id2);
				for (auto& id3_ifo : G.etriangle[eid])		//ͨ��(3,4)�Ҳ��������Σ�1,3,4��
				{
					int id3 = id3_ifo.first;

					int eid13 = id1 < id3 ? G.vedge_set[id1][id3] : G.vedge_set[id3][id1];
					int eid23 = id2 < id3 ? G.vedge_set[id2][id3] : G.vedge_set[id3][id2];

					if (!edge_del[eid13] && !edge_del[eid23])
					{
						int dec;		//�쳣������������ģ���Ϊ����[edge]�����ڣ��ͻὫ����Ϊ0,w1�����Ѿ���u,v������
						
						if (id1 < id3 && id2 < id3)
							dec=G.etriangle[eid][id3];
						else if (id1 < id2 && id3 < id2)
							dec = G.etriangle[eid13][id2];
						else if (id3 < id1 && id2 < id1)
							dec = G.etriangle[eid23][id1];

						if (heap.get_key(eid13) > sup_min)
							heap.decrement(eid13, dec);
						if (heap.get_key(eid23) > sup_min)
							heap.decrement(eid23, dec);
						//����ֻ�ø���sup<k��
					}
				}
			}
			else
				break;
		}
	}
	delete[] Ids;
	delete[] Keys;
	heap.~ListLinearHeap();
}



void build_index_vec(tgraph& G, string file)
{
	//enum_static_triangle(G);

	vector<vgraph> VG(G.tmax + 1);
	for (int i = 0; i <= G.tmax; i++)
	{
		VG[i].nbor.resize(G.maxid + 1);
		VG[i].subifo.resize(G.edge_num );
	}

	G.etriangle.resize(G.edge_num );
	G.eedge_sup.resize(G.edge_num);
	G.skytruss.resize(G.edge_num );
	G.index_train.resize(G.edge_num);

	//timeval st, et;
	for (int i = 0; i <= G.tmax - G.tmin; i++)
	{
		
		
		transfor_layer_vec(G, VG, i);			//�����i�Ǵ���delta
		

		//G.vtuv.pop_back(G.tmin + i);			//ɾ����������ʱ

		cout << "delta i:" << i << endl;

		
		decompose_vec(G, i);
	
	}

	print_to_file(G, file);

}