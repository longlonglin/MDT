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

	for (const auto& eid : G.etuv[e])			//遍历该时刻的加入的边
	{		
		int id1 = G.eid_touv[eid].first, id2 = G.eid_touv[eid].second;

		if (VG[s].nbor[id1].find(id2) == VG[s].nbor[id1].end())			//id1和id2不是邻居，这条静态边是第一次出现
		{
			VG[s].subifo[eid].eweg = 1;										//第一次出现就将权重置为1
			//int idx = VG[s].nbor[id1].size() < VG[s].nbor[id2].size() ? id1 : id2;
			if (VG[s].nbor[id1].size() > VG[s].nbor[id2].size())	    //从id1,id2中邻节点少的点开始遍历寻找其公共顶点
				swap(id1, id2);
			for (const auto& id3 : VG[s].nbor[id1])						//id1取到邻节点
			{
				if (id3 == id1 || id3 == id2)
					continue;
				if (VG[s].nbor[id2].find(id3) != VG[s].nbor[id2].end())	//(id2,id3)存在，形成了新的三角形，将其trian_sup设为0，下面更新就好一点
				{
					
					int eid13 = G.vedge_set[min(id1, id3)][max(id1, id3)], eid23 = G.vedge_set[min(id2, id3)][max(id2, id3)];

					//sub_trian已经保证了是最小两边的id->,w	
					if (id1 < id3 && id2 < id3)						//因为前面swap了所以id和id2的大小还需要判断，可以用个flag
						sub_trian[eid].insert(id3);
					else if (id2 < id1 && id3 < id1)
						sub_trian[eid23].insert(id1);
					else if(id1<id2 && id3<id2)
						sub_trian[eid13].insert(id2);								
					
					VG[s].subifo[eid].trifo[id3]=0;					//设置三角形的sup为0，同时也更新了三角形
					VG[s].subifo[eid13].trifo[id2]=0;
					VG[s].subifo[eid23].trifo[id1]=0;
				}
			}
			VG[s].nbor[id1].insert(id2); VG[s].nbor[id2].insert(id1);	//更新邻节点
		}
		else
		{																//这条静态边已经存在了，权重+1
			VG[s].subifo[eid].eweg++;
			for (auto&wtri : VG[s].subifo[eid].trifo)					//权重改变了，其参与的所有三角形的sup值必然变化
			{
				int id3 = wtri.first;


				if(id2<id3)												//因为id1一定是小于id2的，只用判断id2和id3的关系就行
					sub_trian[eid].insert(id3);
				else
					sub_trian[G.vedge_set[min(id1, id3)][max(id1, id3)]].insert(id2);
			}
		}
	}
}


void update_sup_vec(tgraph& G, vector<vgraph>& VG, vector< unordered_map<int, int>>& fir_trian_up, int s, int e, vector<unordered_set<int>>& sub_trian)
{
	//sub_trian是在更新权重阶段确定的sup值会发生改变的所有三角形, 这里的三角形已经是按(（u，v)->eid,w)的形式存储的
	for (int i = 0; i < sub_trian.size(); i++)
	{
		if (sub_trian[i].size() == 0)
			continue;
		int u = G.eid_touv[i].first, v = G.eid_touv[i].second;
		int wguv = VG[s].subifo[i].eweg;
		for (auto& w : sub_trian[i])
		{
			int wguw = VG[s].subifo[G.vedge_set[u][w]].eweg, wgvw = VG[s].subifo[G.vedge_set[v][w]].eweg;			//取出三边权重
			int count = wguv * wguw * wgvw, incre = 0;			//三边权重积即为当前子区间内的sup值

			incre = count - VG[s].subifo[i].trifo[w];		//当前sup-上一层sup即为sup增量

			VG[s].subifo[i].trifo[w] = count;				//更新子区间三角形的sup值

			G.vtrian_up[i][w] += incre;						//更新全局增量
			if (s == G.tmin)									//第一个区间特殊处理
				fir_trian_up[i][w] = incre;
		}
	}
}

//或者不用index_train，搜索的时候判断这个三角形是不是时态三角形就好
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

				if (G.index_train[i].count(w) == 0)			//值为0，表明这个三角形第一次出现，给其另外两条边添加三角形信息
				{
					G.etriangle[eiduw][v] = 0;
					G.etriangle[eidvw][u] = 0;
					G.index_train[i][w] = delta;			//存储成为三角形时的delta信息
				}

				G.etriangle[i][w] += sup;
				//G.etriangle[v][w][u] += sup;
				//G.vtriangle[u][w][v] += sup;			//这里目前这样写是因为需要知道三边参与了哪些三角形,（多设置一个变量，判断这个三角形是不是已经存在了,）
				G.eedge_sup[i] += sup;
				G.eedge_sup[eiduw] += sup;				//这样访问边太耗时，或许可以将边的sup用数组来表示？？？但是不是，耗时的不是这
				G.eedge_sup[eidvw] += sup;
				
				G.msup = max(max(G.msup, G.eedge_sup[i]), max(G.eedge_sup[eiduw], G.eedge_sup[eidvw]));
				G.vtrian_up[i][w] -= fir_trian_up[i][w];			//更新G的trian_up,为下一轮做准备
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

				int sup = seg.second - cpy_trian_up[i][w];			//与delta等于0的时候不一样的地方

				if (G.index_train[i].count(w) == 0)			//判断是否已经成为时态三角形
				{
					G.etriangle[eiduw][v] += sup;
					G.etriangle[eidvw][u] += sup;
					G.index_train[i][w] = delta;					
				}

				G.etriangle[i][w] += sup;
				G.eedge_sup[i] += sup;				//这样访问边太耗时，或许可以将边的sup用数组来表示？？？但是不是，耗时的不是这
				G.eedge_sup[eiduw] += sup;
				G.eedge_sup[eidvw] += sup;
				G.msup = max(max(G.msup, G.eedge_sup[i]), max(G.eedge_sup[eiduw], G.eedge_sup[eidvw]));
				G.vtrian_up[i][w] -= fir_trian_up[i][w];			//更新G的trian_up,为下一轮做准备
			}
		}
	}
}


void transfor_layer_vec(tgraph& G, vector<vgraph>& VG, int delta)
{

	vector<unordered_map<int, int>> cpy_trian_up = G.vtrian_up;
	G.vtrian_up.clear();						//清零，为从第一个区间计算做准备
	G.vtrian_up.resize(G.edge_num);
	vector<unordered_map<int, int>> fir_trian_up(G.edge_num);		//用来存第一个区间的增量


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
		auto itor = G.skytruss[eid].back();        //这是一种简化吗？可以保证delta越大的时候边的truss值不递减吗吗？-->是的，因为就算delta不变时，其truss值也会和原来保持相同
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
	vector<int> edge_del(G.edge_num,0);					//将所有边标记为未处理
	ListLinearHeap heap(G.edge_num, G.msup);			//第一个参数是最大的边id值(用来开辟数组空间），第二个参数是最大的value值
	int* Ids = new int[G.edge_num];
	int* Keys = new int[G.edge_num];
	int i_ids = 0;

	for (int i = 0; i < G.edge_num; i++)
	{
		Ids[i_ids] = i;
		Keys[i_ids] = G.eedge_sup[i];		//记录eid对应的sup值
		i_ids++;
		int sup = G.eedge_sup[i];
		if (sup < 0)
			cout << "sup-i: " << sup << " " << i << endl;
	}
	heap.init(G.edge_num, G.msup, Ids, Keys);			//初始化堆

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

				find_skytruss(G, delta, sup_min, eid);		//合并具有包含关系的(k,truss)对

				int id1 = G.eid_touv[eid].first, id2 = G.eid_touv[eid].second;
				if (id1 > id2)
					swap(id1, id2);
				for (auto& id3_ifo : G.etriangle[eid])		//通过(3,4)找不到三角形（1,3,4）
				{
					int id3 = id3_ifo.first;

					int eid13 = id1 < id3 ? G.vedge_set[id1][id3] : G.vedge_set[id3][id1];
					int eid23 = id2 < id3 ? G.vedge_set[id2][id3] : G.vedge_set[id3][id2];

					if (!edge_del[eid13] && !edge_del[eid23])
					{
						int dec;		//异常是走这儿产生的，因为可能[edge]不存在，就会将其设为0,w1可能已经和u,v交换了
						
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
						//这里只用更新sup<k的
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
		
		
		transfor_layer_vec(G, VG, i);			//这里的i是代表delta
		

		//G.vtuv.pop_back(G.tmin + i);			//删除反而更耗时

		cout << "delta i:" << i << endl;

		
		decompose_vec(G, i);
	
	}

	print_to_file(G, file);

}