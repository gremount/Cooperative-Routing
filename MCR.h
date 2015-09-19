#pragma once
#include"resources.h"


//Minimum Cost Routing

void CGraph::mcr_propose(  pair<pair<int,int>,int > req){

	int src,des,bw;
	src=req.first.first;
	des=req.first.second;
	bw=req.second;

	mcr_path.clear(); /////////

  ////caculate shortest path  ,get mcr_path
    this->mcr_DijkstraAlg(src,des,bw);
	int tmp=p[src][des];
	if(tmp>0)  //前继大于0表示找到路
	{
	mcr_path.push_back(des);
	while(tmp!=src)
	{
	this->mcr_path.push_back(tmp);
	tmp=p[src][tmp];
	}
	}
	mcr_path.push_back(src);
	mcr_path.reverse();

	cout << "path is:" << endl;
	list<int>::iterator it1;
	for (it1 = mcr_path.begin(); it1 != mcr_path.end(); it1++)
	{
		cout << *it1 << " ";
	}
	cout << endl;


	//MCR bw
	list<int>::iterator it,end;
	end=mcr_path.end();
	for(it=mcr_path.begin();it!=end;)
	{	
		int v1=(*it);
		it++;
		if(it!=end){
		int v2=(*it);
		mcr_link_bw[v1][v2]=bw;
		}
		else break;
	}
   for(int i=1;i<=this->numVertex;i++)
		for(int j=1;j<=this->numVertex;j++)
	{
		mcr_link_bw[i][j]+= all_link_bw[i][j];
	}

}


float CGraph::mcr_evaluate(list<int> path){

 /*  if(path.size()-1==0) 
   {
	cout<<"No path to allocate the flow"<<endl;
	return 0;
   }
  else
	{
		mcr_record=(float)(path.size())-(float)(1);
	   // cout<<"mcr_record:"<<mcr_record<<endl;
	   return mcr_record;	  
	}  
	*/
	int max=0;
	list<int>::iterator it,end;
	end=path.end();
	for(it=path.begin();it!=end;)
	{	
		int v1=(*it);
		it++;
		if(it!=end){
		int v2=(*it);
		max+=weight[v1][v2];
		}
		else break;
	}
	return (float)(max);
    }

void CGraph::mcr_implement(list<int> path,int band)
{
	// allocate bw  //
	list<int>::iterator it2,end2;
	end2=path.end();
	for(it2=path.begin();it2!=end2;)
	{	
		int v1=(*it2);
		it2++;
		if(it2!=end2){
		int v2=(*it2);
	//	mcr_link_bw[v1][v2]=band;  /////如果获胜是MCR，记录当次分配带宽
		all_link_bw[v1][v2]+=band;  /////记录所有带宽
		all_flow_num[v1][v2]+=1;//统计流的种类
		}
		else break;
	}
/*	
cout<<"1 2 3 4 5 6 7 8 9 10 11 12 13 14"<<endl;
	for (int i = 1; i <= numVertex; i++)
	{
		for (int j = 1; j <= numVertex; j++)
		{
			cout << all_link_bw[i][j] << " ";

		}
		cout << endl;
	}
*/

}



