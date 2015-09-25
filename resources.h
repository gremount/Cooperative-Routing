#pragma once
#include "common.h"
using namespace std;

class CStatus{
public:
	int pointID;
	CStatus* father;
	int source;
	int d;
	float util;
	CStatus(int ID){ pointID = ID; source = 1; d = 0;util=0.0; };
	CStatus(int ID, int l,CStatus* f){ pointID = ID; father = f; source = 0; d = l; };
	CStatus(int ID, float l,CStatus* f){ pointID = ID; father = f; source = 0; util = l; };
};

bool pStatusComp(CStatus* x, CStatus* y)
{
	if (x->d < y->d)
		return 1;
	return 0;
};
bool StatusComp(CStatus* x, CStatus* y)
{
	if (x->util < y->util)
		return 1;
	return 0;
};

class CVertex{
public:
	float d;
	int p;
	int ID;
	CVertex(){ d = INF; p = NULL; };
	CVertex(int id){ ID = id; d = INF; p = NULL; };
	~CVertex(){ ; };
};

bool pVertexComp(CVertex* x, CVertex* y)
{
	if (x->d < y->d) 
		return 1;
	return 0;
};


class CEdge{
public:
	int tail, head;
	int weight, capacity;
	float bw_util;
public:
	CEdge(int a, int b, int c, int d);
	CEdge(int a, int b, int c);
	CEdge(CEdge &x);
	int getHead(){return head;}
	int getTail(){return tail;}
	int getWeight(){return weight;}
	int getCap(){return capacity;}
	float getUtil() {return bw_util;}
	bool operator<(CEdge& x){
		if(weight<x.weight)
			return 1;
		else
			return 0;
	}
};

CEdge::CEdge(int a, int b, int c, int d){
	tail = a;
	head = b;
	weight = c;
	capacity = d;
	bw_util=0;
}

CEdge::CEdge(int a, int b, int c){
	head = b;
	tail = a;
	weight = c;
	bw_util=0;
}

CEdge::CEdge(CEdge & x){
	tail = x.getTail();
	head = x.getHead();
	weight = x.getWeight();
	capacity = x.getCap();
	bw_util=x.getUtil();
}



class CGraph{
private:
	int numVertex;
	int numEdge;
	list<CEdge*> IncidentList;//图的所有边组成的链表
	void KSP_Update(int VID,CStatus* f,int b);

public:
	set<int> S[N+10];
	set<int> V[N+10];
	int d[N+10][N+10];
	int p[N+10][N+10];
	//matrix  邻接矩阵
	int weight[N][N];
	int adjmatrix[N+10][N+10];
	map<int, CVertex*> mapVID_Vertex;
	// 暂时标记的顶点集合	
	list<CVertex*> listTempMark;
	// 记录与顶点关联的出度边	
	map<int, list<CEdge*> > mapVID_listEdge;
	////******KSP*****////////
	// KSP用的排序list
	list<CStatus*> listStatus;
	// KSP用的存储序列
	list<list<int>> KSPlist;
	void KSP(int s, int t,int k,int band);

	int all_flow_num[N][N];
	int all_link_bw[N][N];
	void lbr_Update(int v,int b);
	void lbr_DijkstraAlg(int s,int t,int band);

	/********** MCR *************/
	float mcr_record;
	int mcr_link_bw[N][N]; 
	list<int> mcr_path;
	void mcr_propose(pair<pair<int,int>,int > req);
	float mcr_evaluate(list<int> path);
	void mcr_implement(list<int> path,int band);

	/********** LBR **********/
   float lbr_record;  /////没找到路
   int lbr_all_links_bw[N][N];
	int lbr_link_bw[N][N];//
	float lbr_bw_util[N][N];//边的带宽利用率
	list<int> lbr_path;//当次所求路径
	void find_paths(CGraph *g,int s,int t);
	void lbr_propose( pair<pair<int,int>,int > req);
	float lbr_evaluate(list<int> path,int band);
	void lbr_implement(list<int> path,int band);

	/*********** EER **********/
	//统计链路带宽
	int EER_link_bw[100][100];
//	int all_flow_num[100][100];
	list<int> EER_solution;//用于记录EER的解

	void EER_update(int v, int w);
	void EER_propose(pair<pair<int,int>,int > req);
	float EER_evaluate(list<int> solution,int w);
	void EER_implement(list<int> solution,int w);

	/********** QSR *************/
	float qsr_record;
	int qsr_link_bw[N][N]; 
	float qsr_bw_util[N][N];
	list<int> qsr_path;
	void qsr_propose(pair<pair<int,int>,int > req);
	float qsr_evaluate(list<int> path,int band);
	void qsr_implement(list<int> path,int band);

	CGraph(){;};
	CGraph(char* inputFile);
	~CGraph(){ ; };

	int getNumVertex(){
		return numVertex;
	}
	int getNumEdge(){
		return numEdge;
	}
	int cmp(const pair<int,int> &x, const pair<int, int> &y){
		return x.second > y.second;
	}


	void mcr_Update(int s,int i,int b){
		list<CEdge*>::iterator it,iend;
		it=mapVID_listEdge[i].begin();
		iend=mapVID_listEdge[i].end();
		for(;it!=iend;it++)
		{
			if((*it)->capacity-all_link_bw[i][(*it)->getHead()]<b)
				continue;
			if((*it)->getWeight()+d[s][i]<d[s][(*it)->getHead()])
			{
				d[s][(*it)->getHead()]=(*it)->getWeight()+d[s][i];
				p[s][(*it)->getHead()]=i;
			}
		}
	}

	int mcr_FindMin(int s){
		set<int>::iterator vi,vend;
		vend=V[s].end();
		int mini=10000000;
		int loc=0;
		for(vi=V[s].begin();vi!=vend;vi++)
			if(mini>=d[s][*vi])
				{mini=d[s][*vi];loc=*vi;}
		return loc;
	}

	void mcr_DijkstraAlg(int s,int des,int band){
		int i,j;
		V[s].clear();
		S[s].clear();
		for(i=1;i<=this->numVertex;i++)
			V[s].insert(i);
		for(i=1;i<=this->numVertex;i++)
			{d[s][i]=INF;p[s][i]=-2;}
		S[s].insert(s);
		V[s].erase(s);
		d[s][s]=0;
		p[s][s]=-1;
		mcr_Update(s,s,band);
		set<int>::iterator it;
		while (V[s].size()!=0){
			j=mcr_FindMin(s);
			S[s].insert(j);
			mcr_Update(s,j,band);
			if(j==des) break;
			V[s].erase(j);
		}
	}

	void qsr_Update(int s,int i,int b){
		list<CEdge*>::iterator it,iend;
		it=mapVID_listEdge[i].begin();
		iend=mapVID_listEdge[i].end();
		for(;it!=iend;it++)
		{
			if((*it)->capacity-all_link_bw[i][(*it)->getHead()]<b)
				continue;
			if((*it)->getWeight()+d[s][i]<d[s][(*it)->getHead()])
			{
				d[s][(*it)->getHead()]=all_link_bw[i][(*it)->getHead()]*10/(float)((*it)->getCap())+1+d[s][i];
				p[s][(*it)->getHead()]=i;
			}
		}
	}

	int qsr_FindMin(int s){
		set<int>::iterator vi,vend;
		vend=V[s].end();
		int mini=10000000;
		int loc=0;
		for(vi=V[s].begin();vi!=vend;vi++)
			if(mini>=d[s][*vi])
				{mini=d[s][*vi];loc=*vi;}
		return loc;
	}

	void qsr_DijkstraAlg(int s,int des,int band){
		int i,j;
		V[s].clear();
		S[s].clear();
		for(i=1;i<=this->numVertex;i++)
			V[s].insert(i);
		for(i=1;i<=this->numVertex;i++)
			{d[s][i]=INF;p[s][i]=-2;}
		S[s].insert(s);
		V[s].erase(s);
		d[s][s]=0;
		p[s][s]=-1;
		qsr_Update(s,s,band);
		set<int>::iterator it;
		while (V[s].size()!=0){
			j=qsr_FindMin(s);
			S[s].insert(j);
			qsr_Update(s,j,band);
			if(j==des) break;
			V[s].erase(j);
		}
	}
	//test output
	/*	for(int j=1;j<=this->numVertex;j++)
			  cout<<p[s][j]<<"   ";
			cout<<endl;
			for(int j=1;j<=this->numVertex;j++)
			  cout<<this->d[s][j]<<" ";
			cout<<endl;	
	*/
};

void CGraph::KSP(int s, int t,int k,int band)
{
	KSPlist.clear();
	listStatus.clear();
	int count = 0;
	listStatus.push_back(new CStatus(s));

	while (count != k && !listStatus.empty())
	{
		listStatus.sort(StatusComp); /////util comp
		int j = (*listStatus.begin())->pointID;
		//cout<<count<<endl;
		if (j != t)
			KSP_Update(j, (*listStatus.begin()),band);
		else
		{
			list<int> KSP_Solution;
			count++;
			//cout << "path" << count << ":" << endl;
			CStatus* status = (*listStatus.begin());
			while (status->source != 1)
			{
				//cout << status->pointID << " ";
				KSP_Solution.push_front(status->pointID);
				status = status->father;
			}
			//cout << status->pointID << endl;
			KSP_Solution.push_front(status->pointID);
			KSPlist.push_back(KSP_Solution);
		   //cout<<KSPlist.size()<<endl;
		}
		listStatus.pop_front();

	}
	// 以下是输出KSP的结果
/*
list<list<int>>::iterator llit;
	list<int>::iterator lit;
	for (llit = KSPlist.begin(); llit != KSPlist.end(); llit++)
	{
		cout << "path:" << endl;
		for (lit = (*llit).begin(); lit != (*llit).end(); lit++)
			cout << *lit << " ";
		cout << endl;
	}
*/

}

void CGraph::KSP_Update(int v,CStatus* f,int b)
{
	list<CEdge*> lEdge = mapVID_listEdge[v];
	list<CEdge*>::iterator it;
	// int d = f->d;
	float u1=f->util;
	/*list<CStatus*>::iterator i;
	for(i=listStatus.begin();i!=listStatus.end();i++)
	{
	   if(lEdge.empty())
	   { if((*i)->pointID==v)
		this->listStatus.remove(*i);
	   }
	}*/
	if(!lEdge.empty()){
	for (it = lEdge.begin(); it != lEdge.end(); it++)
	{
		if((*it)->capacity-all_link_bw[v][(*it)->getHead()]<b)
				continue;
		(*it)->bw_util=((float)(b)+(float)(all_link_bw[v][(*it)->getHead()]))/(float)(adjmatrix[v][(*it)->getHead()]);
		/*
		int w = (*it)->getWeight();
		int head_ID = (*it)->getHead();
		CVertex* h = mapVID_Vertex[head_ID];
		CVertex* t = mapVID_Vertex[v];
		listStatus.push_back(new CStatus(head_ID,d+w,f));
		*/
		
		int head_ID = (*it)->getHead();
		float u2=(*it)->getUtil();
		float u=max(u1,u2);
		listStatus.push_back(new CStatus(head_ID,u+(float)(0.001),f));	//!!!!!!!!!!!!!!死循环
	}

	}

}


CGraph::CGraph(char* inputFile)
{
    
	////初始化
	    //MCR
	    mcr_record=0;
		//LBR
		lbr_record=0;
		memset(d,INF,sizeof(d));
		memset(p,-2,sizeof(p));
		memset(adjmatrix,0,sizeof(adjmatrix));
	////读入graph
	ifstream fin(inputFile);
	int nhead,ntail,nweight,ncapacity;
	//while(!fin.eof())  ////多读入问题？？？

		fin>>numVertex>>numEdge;
		list<CEdge*> emptylist;
		for (int i = 0; i < numVertex; i++)
	{
		mapVID_Vertex.insert(pair<int, CVertex*>(i + 1, new CVertex(i + 1)));
		mapVID_listEdge.insert(pair<int, list<CEdge*>>(i + 1, emptylist));
	}
		for (int i = 0; i < numEdge; i++)
		{
		fin>>ntail>>nhead>>nweight>>ncapacity;
		CEdge* tmpedge=new CEdge(ntail,nhead,nweight,ncapacity);
		IncidentList.push_back(tmpedge);
		adjmatrix[ntail][nhead]=ncapacity;
		weight[ntail][nhead]=nweight;
		}
		
   list<CEdge*>::iterator it;
	for (it = IncidentList.begin(); it != IncidentList.end(); it++)
	{
		mapVID_listEdge[(*it)->getTail()].push_back(*it);
	}

	for(int i=1;i<=numVertex;i++)
		for(int j=1;j<=numVertex;j++)
		{
			all_flow_num[i][j]=0;
			all_link_bw[i][j]=0;
			mcr_link_bw[i][j]=0;

			lbr_link_bw[i][j]=0;
			lbr_bw_util[i][j]=0;

			all_link_bw[i][j] = 0;
			EER_link_bw[i][j] = 0;
			all_flow_num[i][j] = 0;
			qsr_link_bw[i][j] = 0;
			qsr_bw_util[i][j] = 0;
		}

    //////LBR线性规划用到文件
		ofstream fout,f;
		fout.open("cap.txt");
		 f.open("V.txt");
     for(int i=1;i<=numVertex;i++)
	 {
		for(int j=1;j<=numVertex;j++)
	      {
			fout<<adjmatrix[i][j]<<" ";
			if(adjmatrix[i][j]>0)
				f<<1<<" ";
			else f<<0<<" ";
		  }
				fout<<"\n";
				f<<"\n";
	 }
	 fout.close();
	 f.close();

	
}

