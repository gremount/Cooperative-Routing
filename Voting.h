#pragma once
#include"common.h"
#define P 4   // P is the number of proposals
#define M 4   // M is the number of modules
#define None 0;

using namespace std;

class Vote{
public:
	//Table tb;
	float finalVotes[P];//p1,p2,p3
	float TotalVotes[M];//MCR,LCR,EER,QSR
	int flag;//
	int wins[P + 1][P + 1];  //scenario 3
	int finalwin[P + 1];
	int winner;
	vector<int> winner_vector;
	Vote(){ ; }

	int Voting(float table[P][M],int k){
	if(k==2)
		return Cumulative_Voting(table);//Scenario 2
	else if(k==3)
		return Condorcet_Voting(table);//Scenario 3
	else
	{
		cout<<"the k is the wrong number,make sure k is 2 or 3"<<endl;
		return 0;
	}
	}

	//Scenario 2
	int Cumulative_Voting(float table2[P][M]){
		//initialization
		memset(finalVotes, 0, sizeof(finalVotes));
		memset(TotalVotes, 0, sizeof(TotalVotes));
		winner = 0;
		float table[4][4];
		for (int i = 0; i <= 3; i++)
			for (int j = 0; j <= 3; j++)
				table[i][j] = table2[i][j];
		
		/////convert 0 to 0.1  for EER

		for (int k = 0; k<P; k++)
			for (int m = 0; m<M; m++)
			{
				if (table[k][m] == 0)
					table[k][m] = 0.1;
			}

		///// 将MCR  EER 取倒数  因为评价是越小越好，且归一化为小数
		for (int k = 0; k<P; k++)
			{
				table[k][0] = 1 / table[k][0];
				table[k][1] = 1 / table[k][1];
				table[k][2] = 1 / table[k][2];
				table[k][3] = 1 / table[k][3];
			}

		// count totalVotes
		int i = 0;
		while (i<M){
			for (int j = 0; j<M; j++)
			{
				TotalVotes[i] = table[0][j] + table[1][j] + table[2][j] + table[3][j];
				i++;
			}
		}


		// count votes for every proposal
		for (int k = 0; k<P; k++)
			for (int m = 0; m<M; m++)
			{
				finalVotes[k] += table[k][m] / TotalVotes[m];
			}


		//select winner
		float sum = 0;
		flag = 0;
		for (int i = 0; i<P; i++)
		{
			if (finalVotes[i]>sum)
			{
				sum = finalVotes[i];
				//winner = i + 1;
				winner_vector.clear();
				winner_vector.push_back(i + 1);
			}
			else if (finalVotes[i] == sum)
			{
				flag += 1;//there are two or more proposal having same votes
				winner_vector.push_back(i + 1);
			}
				
		}

	for (int i = 0; i<P; i++)
		cout<<"proposal "<<i+1<<"votes is:"<<finalVotes[i]<<endl;

		/*
		if(flag>=1)
		{ cout<<"there is not only one winner,try another Scenario or vote again"<<endl;
		// return None;
		return winner;// the first one winner
		}
		else  */


		//strat to random the winner
	srand(time(0));
	if (winner_vector.size()==1)
		return winner_vector[0];
	else
		return winner_vector[rand() % winner_vector.size()];
	}


	//Scenario 3
	int Condorcet_Voting(float test[P][M]){
		//initialization
		winner = 0;
		memset(wins, 0, sizeof(wins));
		int table[4][4];
	/*	for (int i = 0; i <= 2; i++)
			for (int j = 0; j <= 3; j++)
				table[i][j] = test[i][j];
	*/
		multimap<float,int > tmp;
		int j=0;
		while(j<M)
		{
         tmp.clear();
		 for(int i=0;i<P;i++)
		 {
		pair<float,int> p(test[i][j],i+1);
		tmp.insert(p);
		 }
		 multimap<float,int>::iterator it;
		 int m=0;
		 for(it=tmp.begin();it!=tmp.end();it++)
		 {
			 table[m][j]=(*it).second;
			 m++;
		 }
		 j++;
		}
		//几个proposal的排序输出
		for(int i=0;i<P;i++)
		{	for(int j=0;j<M;j++)
				cout<<table[i][j]<<" ";
		         cout<<endl;
		}

		// count pairwise Votes
		for (int k = 0; k<P - 1; k++)
			for (int m = 0; m<M; m++)
				for (int j = 1; j + k<P; j++)
					wins[table[k][m]][table[k + j][m]] += 1;

		// count votes for every proposal
		memset(finalwin, 0, sizeof(finalwin));
		for (int k = 0; k<P + 1; k++)
			for (int i = 0; i<P + 1; i++)
			{
				finalwin[k] += wins[k][i];
			}

		//select winner
		float sum = 0;
		flag = 0;
		for (int i = 1; i<P+1; i++)
		{
			if (finalwin[i]>sum)
			{
				sum = finalwin[i];
				//winner = i + 1;
				winner_vector.clear();
				winner_vector.push_back(i);
			}
			else if (finalwin[i] == sum)
			{
				flag += 1;//there are two or more proposal having same votes
				winner_vector.push_back(i);
			}
				
		}

	for (int i = 1; i<P+1; i++)
		cout<<"proposal "<<i<<"votes is:"<<finalwin[i]<<endl;

		/*
		if(flag>=1)
		{ cout<<"there is not only one winner,try another Scenario or vote again"<<endl;
		// return None;
		return winner;// the first one winner
		}
		else  */

	//strat to random the winner
	srand(time(0));
	if (winner_vector.size()==1)
		return winner_vector[0];
	else
		return winner_vector[rand() % winner_vector.size()];

	}

};