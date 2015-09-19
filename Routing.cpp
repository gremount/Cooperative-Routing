#pragma once
#include"resources.h"
#include"MCR.h"
#include"LBR.h"
#include"EER.h"
#include"QSR.h"
#include"Voting.h"
#include<fstream>
#include <time.h>
#include <stdlib.h>
#include<string>


float test_record[4][4];

void test(pair<pair<int, int>,int > req,CGraph &g)
{
	//the initialization of the test_record[][]
	memset(test_record,0,sizeof(test_record));
	//evaluate the MCR
	test_record[0][0] = g.mcr_evaluate(g.mcr_path);
	test_record[0][1] = g.lbr_evaluate(g.mcr_path,req.second);
	test_record[0][2] = g.EER_evaluate(g.mcr_path,req.second);
	test_record[0][3] = g.qsr_evaluate(g.mcr_path);
	//evaluate the LBR
	test_record[1][0] = g.mcr_evaluate(g.lbr_path);
	test_record[1][1] = g.lbr_evaluate(g.lbr_path,req.second);
	test_record[1][2] = g.EER_evaluate(g.lbr_path,req.second);
	test_record[1][3] = g.qsr_evaluate(g.lbr_path);
	//evaluate the EER
	test_record[2][0] = g.mcr_evaluate(g.EER_solution);
	test_record[2][1] = g.lbr_evaluate(g.EER_solution,req.second);
	test_record[2][2] = g.EER_evaluate(g.EER_solution,req.second);
	test_record[2][3] = g.qsr_evaluate(g.EER_solution);
	//evaluate the QSR
	test_record[3][0] = g.mcr_evaluate(g.qsr_path);
	test_record[3][1] = g.lbr_evaluate(g.qsr_path,req.second);
	test_record[3][2] = g.EER_evaluate(g.qsr_path,req.second);
	test_record[3][3] = g.qsr_evaluate(g.qsr_path);
}
/*  
//LB测试需要
void genReq(int n,int ver,int bw,char name[])
{
	 FILE *out=fopen(name, "w");
	 fprintf(out,"%d \n",n);
	 int count=0;
	 while(count<n)
	 {
     //for(int i = 1; i <ver; i++){
      int s = rand()%ver+1, t = rand()%ver+1, w = rand()%bw+1;
      while(t == s)
      t = rand()%ver+1;
	 //if(t==s) continue;
      fprintf(out, "%d %d %d \n", s, t, w);
	  count++;
      //fprintf(out, "%d %d %d 0\n", t, s, w);
     
	 }
    fclose(out);
}
void genReq2(int n,int ver,int bw,char name[])
{
	ofstream f2;
	f2.open(name);
	f2<<n<<endl;
	int s=rand()%ver+1,t=rand()%ver+1;
	while(t==s)
		t=rand()%ver+1;
	int count=0;
	while(count<n)
	{
	  int w=rand()%bw+1;
	  f2<<s<<" "<<t<<" "<<w<<endl;
	  count++;
	}
	f2.close();
}
*/
int main()
{

    CGraph G("graph6.txt");
	ifstream testfile("req6.txt");//ifstream默认以输入方式打开文件
	pair<pair<int,int>,int >  req;
	Vote vv;
	//*****************  Standard Test   *****************

	printf("  min_cost   bw_util     link_util\n");

	//*****************the first request*****************
	int req_num;
	testfile >> req_num;
	for (int k = 0; k < req_num; k++)
	{
		cout << "-------------------------------------------------" << endl;
		cout << "req" << k + 1 << ":" << endl;
		testfile >> req.first.first;
		testfile >> req.first.second;
		testfile >> req.second;
		cout << "src = " << req.first.first << " des = " << req.first.second <<" BW= "<<req.second<< endl;
		G.mcr_propose(req);
		G.lbr_propose(req);
		G.EER_propose(req);
		G.qsr_propose(req);
		test(req, G);
		for (int i = 0; i <= 3; i++)
		{
			for (int j = 0; j <= 3; j++)
			printf("%10.3f ", test_record[i][j]);
			printf("\n");
		}
		//voting methods
		int winner;//the winner choosed by the voting methods
		winner = vv.Voting(test_record, 2);
		cout << "After voting, the winner is: " << winner << endl;

		if (winner == 1)
		{
			G.mcr_implement(G.mcr_path,req.second);
		}
		else if (winner == 2)
		{
			G.lbr_implement(G.lbr_path,req.second);
		}
		else if (winner == 3)
		{
			G.EER_implement(G.EER_solution,req.second);
		}
		else if (winner == 4)
		{
			G.EER_implement(G.qsr_path,req.second);
		}
		else
			cout << "error" << endl;
    }


//**********************LBR单模块测试
/*	int dem[N][N];
	memset(dem,0,sizeof(dem));
	int req_num;
	testfile >> req_num;
	float lbr_bw_util;
	cout<<"LBR testing:"<<"the max bw_util"<<endl;
	for(int i=0;i<req_num;i++)
	{
		cout << "-----------------------------------" << endl;
		cout << "req" << i + 1 << ":" << endl;
		testfile >> req.first.first;
		testfile >> req.first.second;
		testfile >> req.second;
		cout << "src = " << req.first.first << " des = " << req.first.second <<" BW= "<<req.second<< endl;
		dem[req.first.first][req.first.second]+=req.second;
		G.lbr_propose(req);
		lbr_bw_util=G.lbr_evaluate(G.lbr_path,req.second);
		cout<<"bw_util:"<<lbr_bw_util<<endl;
		G.lbr_implement(G.lbr_path,req.second);
	}
	////output demand to file
	ofstream f1;
	f1.open("demand.txt");
	int num=G.getNumVertex();
	for(int i=1;i<=num;i++)
	{	for(int j=1;j<=num;j++)
		{	f1<<dem[i][j]<<" ";
	    }
	    f1<<"\n";
	 }
	f1.close(); //!!!!!!!!!!!!!!!!!!!!!!!
	//输出带宽分布情况
	cout<<"link(i,j) used:"<<endl;
	for(int i=1;i<N;i++)
		for(int j=1;j<N;j++)
		{
			if(G.all_link_bw[i][j]>0)
				cout<<"  ("<<i<<","<<j<<")"<<":"<<G.all_link_bw[i][j]<<endl;

		}
		cout<<endl;
  */

   //****************req随机产生，多对节点对之间的需求
	/*	srand ((unsigned)time(0));
		int num_case=10,num_req=15,max_bw=50;
		//for(int id_case=1;id_case<num_case;id_case++)
		//{
		
			CGraph G("graph3.txt");
			int ver_num=G.getNumVertex();
			 genReq2(num_req,ver_num, max_bw, "ReqSinglePair.txt");
			 cout << "************************" << endl;
			// cout << "case " << id_case << ":" << endl;
    
    ifstream testfile("ReqSinglePair.txt");
    int dem[N][N];

	memset(dem,0,sizeof(dem));
	int req_num;
	testfile >> req_num;
	float lbr_bw_util;
	cout<<"LBR testing:"<<"the max bw_util"<<endl;
	for(int i=0;i<req_num;i++)
	{
		cout << "-----------------------------------" << endl;
		cout << "req" << i + 1 << ":" << endl;
		testfile >> req.first.first;
		testfile >> req.first.second;
		testfile >> req.second;
		cout << "src = " << req.first.first << " des = " << req.first.second <<" BW= "<<req.second<< endl;
		dem[req.first.first][req.first.second]+=req.second;
		G.lbr_propose(req);
		lbr_bw_util=G.lbr_evaluate(G.lbr_path,req.second);
		cout<<"bw_util:"<<lbr_bw_util<<endl;
		G.lbr_implement(G.lbr_path,req.second);
	}
	//	}
	ofstream f1;
	f1.open("demand.txt");
	int num=G.getNumVertex();
	for(int i=1;i<=num;i++)
	{	for(int j=1;j<=num;j++)
		{	f1<<dem[i][j]<<" ";
	    }
	    f1<<"\n";
	 }
	f1.close(); //!!!!!!!!!!!!!!!!!!!!!!!
	cout<<endl;cout<<"  bw_util:"<<G.lbr_record<<endl;
	//输出带宽分布情况
	cout<<"  link(i,j) used:"<<endl;
	for(int i=1;i<N;i++)
		for(int j=1;j<N;j++)
		{
			if(G.all_link_bw[i][j]>0)
				cout<<"  ("<<i<<","<<j<<")"<<":"<<G.all_link_bw[i][j]<<endl;

		}
		cout<<endl;
*/
/*
   CGraph G("graph1.txt");
   ifstream testfile("reqKSP.txt");
	int req_num;
	testfile >> req_num;
	float lbr_bw_util;
	cout<<"LBR testing:"<<"the max bw_util"<<endl;
	for(int i=0;i<req_num;i++)
	{
		cout << "-----------------------------------" << endl;
		cout << "req" << i + 1 << ":" << endl;
		testfile >> req.first.first;
		testfile >> req.first.second;
		testfile >> req.second;
		cout << "src = " << req.first.first << " des = " << req.first.second <<" BW= "<<req.second<< endl;
		G.lbr_propose(req);
		lbr_bw_util=G.lbr_evaluate(G.lbr_path,req.second);
		cout<<"bw_util:"<<lbr_bw_util<<endl;
		G.lbr_implement(G.lbr_path,req.second);
	}
	cout<<endl;
	cout<<"bw_util:"<<G.lbr_record<<endl;

	cout<<"  link(i,j) used:"<<endl;
	for(int i=1;i<N;i++)
		for(int j=1;j<N;j++)
		{
			if(G.all_link_bw[i][j]>0)
				cout<<"  ("<<i<<","<<j<<")"<<":"<<G.all_link_bw[i][j]<<endl;

		}
		cout<<endl;
*/
	system("pause");
	return 0;

}