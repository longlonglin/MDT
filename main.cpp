#include"Graph_IO.h"
#include<unordered_map>
#include"triangle_count.h"
#include"local_search.h"
#include"index_search.h"
#include"index_build.h"
#include"print.h"
#include"query.h"
#include<sys/time.h>
#include"helper.h"

using namespace std;



int main(int argc, char* argv[])
{

	string file;
	int q;
	int delta;

	cout<<"Welcome to the q-MDT model: "<<endl;
	tgraph G;

	char flag='Y';
	char change_file='Y';
	set<string> name={"rmin","primary","lyon","thiers","twitter","facebook","enron","lkml","dblp"};
	while(flag=='y' || flag=='Y')
	{
		if(change_file=='Y' || change_file=='y')
		{
			cout<<"Please choose the dataset that you want to query from the following:"<<endl;
			cout<<"Rmin Primary Lyon Thiers Twitter Facebook Enron Lkml DBLP"<<endl;
			cin>>file;
			transform(file.begin(),file.end(),file.begin(),::tolower);
			// Readin(strfile, G);

			if(name.find(file)==name.end())
				continue;
		
			tgraph Gn;
			G=Gn;
			// transform(file.begin(),file.end(),file.begin(),::tolower);
			string strfile = "./data/" + file + ".txt";
			Readin(strfile, G);
		}

		cout<<"Please input the query node:";
		while(true)
		{
			cin>>q;
			if(G.nodes[q].TE.size()==0)
				cout<<"Sorry, please input the vaild query node: ";
			else
				break;

		}
		cout<<"Please input the δ：";
		cin>>delta;
		
		query_try_q(G, file, delta, q);

		cout<<"Whether to continue the query? please choose Y or N: ";
		cin>>flag;

		if(flag)
		{
			cout<<"Would you like to change the query dataset? Y or N: ";
			cin>>change_file;
		}
				
	}

	cout<<"Looking forward to seeing you next time!"<<endl;

	// string file = argv[1];			//CollegeMsg    sort_graph2
	//int delta = atoi(argv[2]);
	//int q = atoi(argv[3]);
	/*int deg_num = atoi(argv[3]);
	int round_num = atoi(argv[4]);*/
	//int per = atoi(argv[2]);
	// int delta = atoi(argv[2]);

	// string strfile = "./data/" + file + ".txt";

	// /*cerr << strfile << endl;
	// cerr << "d-p-a: " << delta << " " << argv[3] << endl;*/
	// cerr << strfile << endl;

	// tgraph G;

	// timeval et, st;
	// gettimeofday(&st, 0);
	// Readin(strfile, G);
	// gettimeofday(&et, 0);
	// cout << "reading time:" << (et.tv_sec - st.tv_sec) * 1000 + (et.tv_usec - st.tv_usec) / 1000 << "ms" << endl;

	// //query_ans_varydelta(G,file,delta);

	// query_expand_ans_size(G,file,delta);

	// query_case(G, file);

	/*gettimeofday(&st, 0);
	build_index_vec(G, file);
	gettimeofday(&et, 0);
	cout << "index time:" << (et.tv_sec - st.tv_sec) * 1000 + (et.tv_usec - st.tv_usec) / 1000 << "ms" << endl;*/

	//select_seed(G, file);

	//query_try_q(G, file, delta, q);
	//local_search(G, q, delta);

	/*query_time(G, file, delta);*/

	//select_seed_varydeg(G, file);
	//select_seed_withtruss(G, file);

	//query_ans(G, file, delta);

	//query_time_varydeg(G, file, per);


	/*gettimeofday(&st, 0);
	unordered_map<int, unordered_map<int, unordered_set<int>>> non_temptri;
	G.vtriangle.resize(G.maxid + 1);
	G.vedge_sup.resize(G.maxid + 1);
	int ansk = 0, maxsup = 0;
	count_global_sup(G, delta, non_temptri, maxsup);
	gettimeofday(&et, 0);
	cout << "count triangle time:" << (et.tv_sec - st.tv_sec) * 1000 + (et.tv_usec - st.tv_usec) / 1000 << "ms" << endl;*/


	//select_target_seed(G, file);
	//select_seed(G, file);

	/*if (((string)argv[3]) == "seed")
		select_seed(G, file);
	else if (((string)argv[3]) == "query")
		query(G, file, delta);
	else if (((string)argv[3]) == "query_time")
		query_time(G, file, delta);
	else if (((string)argv[3]) == "index")
	{
		cout << "1";
		gettimeofday(&st, 0);
		build_index_vec(G, file);
		gettimeofday(&et, 0);
		cout << "index time:" << (et.tv_sec - st.tv_sec) * 1000 + (et.tv_usec - st.tv_usec) / 1000 << "ms" << endl;
	}*/
}
