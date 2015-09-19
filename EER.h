#include<iostream>
#include"resources.h"
#include<math.h>
#include<cmath>

void CGraph::EER_propose(pair<pair<int,int>,int > req)
{
	int s=req.first.first;
	int t=req.first.second;
	int w=req.second;

	listTempMark.clear();
	map<int, CVertex*>::iterator i, iend;
	iend = mapVID_Vertex.end();
	for (i = mapVID_Vertex.begin(); i != iend; i++)
	{
		if (i->second->ID == s)
			i->second->d = 0;
		else
			i->second->d = 10000;
		i->second->p = 0;
		listTempMark.push_back(i->second);
	}
	
	while (!listTempMark.empty())
	{
		listTempMark.sort(pVertexComp);
		int j = (*listTempMark.begin())->ID;
		listTempMark.pop_front();
		EER_update(j,w);
		if (j == t)
			break;
	}
	EER_solution.clear();
	int j = t;
	while (mapVID_Vertex[j]->ID != s)
	{
		if (mapVID_Vertex[j]->p == 0)
			break;
		EER_solution.push_front(mapVID_Vertex[j]->ID);
		j = mapVID_Vertex[j]->p;
	}
	EER_solution.push_front(s);
	cout << "path is:" << endl;
	list<int>::iterator it;
	for (it = EER_solution.begin(); it != EER_solution.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;


	//EER_link_bwµÄÍ³¼Æ
	for (int i = 1; i <= numVertex; i++)
	{
		for (int j = 1; j <= numVertex; j++)
			EER_link_bw[i][j] =0;
	}
	int solution_size = EER_solution.size() - 1;
	it = EER_solution.begin();
	int tt = *(it);
	int h;
	for (int i = 0; i < solution_size; i++)
	{
		h = *(++it);
		EER_link_bw[t][h] = w;
		tt = h;
	}
	for (int i = 1; i <= numVertex; i++)
	{
		for (int j = 1; j <= numVertex; j++)
			EER_link_bw[i][j] += all_link_bw[i][j];
	}

}

void CGraph::EER_update(int v, int w)
{
	list<CEdge*> lEdge = mapVID_listEdge[v];
	list<CEdge*>::iterator i, iend;
	iend = lEdge.end();
	for (i = lEdge.begin(); i != iend; i++)
	{
		int cap = (*i)->getCap();

		
		if (all_link_bw[v][(*i)->getHead()] + w > cap)
			continue;
		CVertex* h = mapVID_Vertex[(*i)->getHead()];
		CVertex* t = mapVID_Vertex[v];


		float weight = pow(((float)all_link_bw[v][(*i)->getHead()] + (float)w) / 10, (float)(1.5)) - pow(((float)all_link_bw[v][(*i)->getHead()]) / 10,  (float)(1.5));
		
		if (all_link_bw[v][(*i)->getHead()] == 0)
			weight += 1;

		//cout << weight << endl;


		if (t->d + weight < h->d)
		{
			h->d = t->d + weight;
			h->p = v;
		}
	}
}

float CGraph::EER_evaluate(list<int> solution, int w)
{
	float score = 0;
	//cout << "solution size is " << solution.size() - 1 << endl;
	int solution_size = solution.size() - 1;
	list<int>::iterator it = solution.begin();
	int t = *(it);
	int h;
	for (int i = 0; i < solution_size; i++)
	{
		h = *(++it);
		//cout << !all_flow_num[t][h] << " ";
		score += pow(((float)all_link_bw[t][h] + (float)w) / 10,(float) (1.5)) - pow(((float)all_link_bw[t][h]) / 10, (float)(1.5));
		if (all_link_bw[t][h] == 0)
			score += 1;
		t = h;
	}

	return score;

}


void CGraph::EER_implement(list<int> solution,int w)
{
	int solution_size = solution.size()-1;
	list<int>::iterator it = solution.begin();
	int t = *(it);
	int h;
	for (int i = 0; i < solution_size; i++)
	{
		h = *(++it);
		all_link_bw[t][h] += w;
		all_flow_num[t][h] += 1;
		t = h;
	}
/*	cout<<"1 2 3 4 5 6 7 8 9 10 11 12 13 14"<<endl;
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