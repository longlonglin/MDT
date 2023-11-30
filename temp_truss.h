#pragma once
#include <time.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <vector>
#include <time.h>
#include <queue> 
#include <set>
#include <list>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <assert.h>
 
using namespace std;


typedef pair<int, int> sedge;
typedef tuple<int, int, int> striangle;


struct sedge_hash
{
    size_t operator()(const sedge& se) const
    {
        return hash<int>()(se.first) ^ hash<int>()(se.second);
    }
};


struct sedge_equal
{
    bool operator()(const sedge& se1, const sedge& se2) const noexcept
    {
        return se1.first == se2.first && se1.second == se2.second;
    }
};

struct striangle_hash
{
    size_t operator()(const striangle& st) const
    {
        return hash<int>()(get<0>(st)) ^ hash<int>()(get<1>(st)) ^ hash<int>()(get<2>(st));
    }
};


struct striangle_equal
{
    bool operator()(const striangle& st1, const striangle& st2) const noexcept
    {
        return get<0>(st1) == get<0>(st2) && get<1>(st1) == get<1>(st2) && get<2>(st1) == get<2>(st2);
    }
};



class TimeInfo {
public:              
    vector<int> TS;            
};

class NodeInfo {
public:
    map<int, TimeInfo> TE;      //temporal neighbors
    vector<int> neighbors;      //static neighbors
    bool isin;                  //whether the node is in the graph
};

class tgraph {
public:
    vector<NodeInfo> nodes;     //the ADJ_TAB OF TEMPORAL GRAPH 
    int tmin, tmax, max_interval, edge_num, msup, maxid;    //The maximum time differences among the three edges of the triangle. To calculate, enum_tringale needs to be called

    //readfile 
    //vector <unordered_map<int, unordered_set<int>>> vtuv;

    vector<unordered_map<int, int>> vedge_set;
    vector<sedge> eid_touv;
    //map<int, sedge> eid_touv;

    //build_index
    vector<unordered_map<int, int>> vedge_sup;
    vector<unordered_map<int, unordered_map<int, int>>> vtriangle;

    vector<int> eedge_sup;           
    vector<unordered_map<int, int>> etriangle;
    vector <unordered_set<int>> etuv;
   

    vector<vector<int>> vskytruss;                                             //eid->t->(
    vector<vector<pair<int, int>>> skytruss;
    //vector<unordered_map<int, unordered_map<int, int>>> index_train;        
    vector<unordered_map<int, int>> index_train;
    //vector<unordered_map<int,set<pair<int,int>>>> tdtri;                    
    vector<set<pair<int, int>>> tdtri;                                    //eid<delta,w>


    
    vector<unordered_map<int, int>> vtrian_up;      //eid->(w,sup)



    //map<sedge, int> edge_set;      
    //map<int,set<sedge>> tuv;        
    //map<striangle, int> trian_up;
    //map<int, map<int, set<int>>> stuv;
    //map<sedge, map<int, int>> triangle;     
    //map<sedge, int> edge_sup;               
    //map<int, map<int, map<int, int>>> mtrian_up;
};


class sgraph
{
public:
    map<int, unordered_set<int>> nbor;                
    map<sedge,set<int>> trian_in;          
    map<sedge, int> weight;                 
    map<sedge, int> edge_up;                
    map<striangle,int> trian_up;                                               
    map<striangle, int> trian_sup;          

};

map<pair<int, int>, set<int>> static_triangle;


int pre, pos, mid, counts;
map<pair<int, int>, int> pre_node;
map<pair<int, int>, int> pos_node;


int trans_t = 0;
int decom_t = 0;
int truss_t = 0;     

int scan_t = 0;
int compsup_t = 0;
int weight_t = 0;
int upsup_t = 0;
int laysup_t = 0;
int cpy_triup_t = 0, erase_tuv_t = 0, erase_SG_t = 0;

class nodeifo
{
public:
    int eweg;
    unordered_map<int,int> trifo;      
};

class vgraph
{
public:
    vector<nodeifo> subifo;         
    vector<unordered_set<int>> nbor;                     
};


//vector<vgraph> VG;

class viseg
{
public:
    int eid;
    bool vis;
};
