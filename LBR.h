#pragma once
#include"resources.h"

//Load Balance Routing

void CGraph::lbr_propose( pair<pair<int,int>,int > req){
	int src,des,bw;
	src=req.first.first;
	des=req.first.second;
	bw=req.second;

   lbr_path.clear(); //当次路径
   
   this->lbr_DijkstraAlg(src,des,bw);
   int NO = des;
   while(mapVID_Vertex[NO]->p!=0)
   {
	   lbr_path.push_front(NO);
	   NO = mapVID_Vertex[NO]->p;
   }
   lbr_path.push_front(src);
   
  
   /*
	////caculate  path  ,choose lbr_path
    ///// KSP
	this->KSP(src,des,5,bw);
	if(!KSPlist.empty())
	{
	list<list<int> >::iterator it1,end1,fag,flag[N];
	end1=this->KSPlist.end();	
	list<int>::iterator it3,end3;
	float maxmin[N];
	int re_bw[N];//residual bw
    memset(maxmin,(float)(0),sizeof(maxmin));
	memset(re_bw,0,sizeof(re_bw));
	int k=1; //// K th path
	for(it1=KSPlist.begin();it1!=end1;it1++)
	{
      for(it3=(*it1).begin();it3!=(*it1).end();)
	  {
		  int v1=(*it3);
		  it3++;
		  if(it3!=(*it1).end())
		  {
			  int v2=(*it3);
			  float util=((float)(all_link_bw[v1][v2])+(float)(bw))/(float)(this->adjmatrix[v1][v2]);
			  int res=adjmatrix[v1][v2]-all_link_bw[v1][v2];
              if(util>=maxmin[k])
		       {
                     maxmin[k]=util;	
					 re_bw[k]=res;
		        }
			  
		    }
		  else break;
	  }
	  flag[k]=it1;
	  k++; //kth path
	}
	//select path
	float min=(float)INF;
	int  max_bw=0;//剩余带宽最大值
	for(unsigned int i=1;i<=KSPlist.size();i++)
	{
		if(min>=maxmin[i] && max_bw<re_bw[i])
			{
				min=maxmin[i];
		        fag=flag[i];
				max_bw=re_bw[i];
		    }
	}

	lbr_path=(*fag);/////选出了带宽利用率最大最小那条路

	}

	else cout<<"NO path to allocate the flow"<<endl;
  
	if(!lbr_path.empty()){
	//output test
	cout << "path is:" << endl;
	list<int>::iterator it,end;
	end=lbr_path.end();
	for (it = lbr_path.begin(); it!= end; it++)
	{
		cout << *it<< " ";
	}
	cout << endl;
   */
	
   ///PATH
    cout<<"path is:"<<endl;
	list<int>::iterator it,end;
	end=lbr_path.end();
	if(lbr_path.size()-1==0) cout<<"No path"<<endl;
	else{
	for(it=lbr_path.begin();it!=end;it++)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
	}

	//LBR bw
	for(it=lbr_path.begin();it!=end;)
	{	
		int v1=(*it);
		it++;
		if(it!=end){
		int v2=(*it);
		mcr_link_bw[v1][v2]=bw;
		}
		else break;
	}

	for(int i=1;i<=numVertex;i++)
		for(int j=1;j<=numVertex;j++)
		{
			lbr_link_bw[i][j]+=all_link_bw[i][j];
		}
	}

	



float CGraph::lbr_evaluate(list<int> path,int band){

if(path.size()-1==0) return lbr_record;//没找到路，返回历史最大值
else{
float max=0.0;
list<int>::iterator it,end;
end=path.end();
for(it=path.begin();it!=end;)
{
	int v1=(*it);
	it++;
	if(it!=end)
	{
		int v2=(*it);
		float util=((float)band+(float)(all_link_bw[v1][v2]))/(float)(adjmatrix[v1][v2]);
		if(max<util)
			max=util;
	}
	else break;
}
if(lbr_record<max) lbr_record=max;
return max;
}
}

void CGraph::lbr_implement(list<int>path,int band)
{
	list<int>::iterator it,end;
	end=path.end();
	for(it=path.begin();it!=end;)
	{	
		int v1=(*it);
		it++;
		if(it!=end)
		{
		int v2=(*it);
		all_link_bw[v1][v2]+=band;
		all_flow_num[v1][v2]+=1;//统计流的种类
		}
		else break;
	}

// cout<<"1 2 3 4 5 6 7 8 9 10 11 12 13 14"<<endl;
/*		for (int i = 1; i <= numVertex; i++)
	{
		for (int j = 1; j <= numVertex; j++)
		{
			cout << all_link_bw[i][j] << " ";

		}
		cout << endl;
	}
*/

}
void CGraph::lbr_DijkstraAlg(int s,int t,int band)
{
	listTempMark.clear();
	map<int, CVertex*>::iterator i, iend;
	iend = mapVID_Vertex.end();
	for (i = mapVID_Vertex.begin(); i != iend; i++)
	{
		if (i->second->ID == s)
			i->second->d = 0;
		else
			i->second->d=10000;
		i->second->p=0;
		listTempMark.push_back(i->second);
	}
	//Update(s);
	while (!listTempMark.empty())
	{
		listTempMark.sort(pVertexComp);
		int j = (*listTempMark.begin())->ID;
		listTempMark.pop_front();
		lbr_Update(j,band);
		//cout<<"j "<<j<<endl;
		if(j==t) break;
		//cout << mapVID_Vertex[j]->ID << "  d:" << mapVID_Vertex[j]->d << endl;
	}
}

void CGraph::lbr_Update(int v,int b)
{
	list<CEdge*> lEdge = mapVID_listEdge[v];
	list<CEdge*>::iterator i, iend;
	iend = lEdge.end();
	for (i = lEdge.begin(); i != lEdge.end(); i++)
	{
		if((*i)->capacity-all_link_bw[v][(*i)->getHead()]<b)
				continue;
		
		float util =(float)(b+all_link_bw[v][(*i)->getHead()])/(float)((*i)->getCap());
		CVertex* h = mapVID_Vertex[(*i)->getHead()];//head
		CVertex* t = mapVID_Vertex[v];//tail
		float m=max(t->d,util);
		if( m < h->d)
		{
			h->d = m;
			h->p = v;
		}
	}
}