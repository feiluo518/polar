#include<iostream>
#include<cstdio>
#include<fstream>
#include<sstream>
#include<cstring>
#include<cmath>
#include<set>
#include"analysis.h"
#include"channelPolarization.h"
#include"SC.h"
#include"generator.h"
#include"listdecoding.h"
#include"rewritting.h"

using namespace std;
extern int trial;
extern int N;
extern int K;
extern int q;
extern double distribution;


void rewritter(int *current, int *dither, int *updated, int *frozenBit, set<int>FrozenIndex, double **W){
   int datavector[N];
   int tempUpdated[N];

   for(int i = 0; i < N; i++){
	datavector[i] = rand()%2;
	dither[i] = (current[i] + datavector[i])%2;
   }
   SCEncoding(datavector, tempUpdated, FrozenIndex, frozenBit, W);
   for(int i = 0; i < N; i++)
	updated[i] = (tempUpdated[i] + dither[i])%2;
}

void findFrozenBits(int *codeword, int *result, set<int>FrozenIndex){
	int FIndex = 0;
	for(int i = 0; i < N; i++){
		if(FrozenIndex.find(i) != FrozenIndex.end())
			result[FIndex++] = codeword[i];
	}
}

int decoder(int *received, int *dither, set<int>FrozenIndex){
	int data[N];
	for(int i = 0; i < N; i++)
		data[i] = (received[i] + dither[i])%2;
	
	int message[N], result[N - K];
	encode(message, data);
	findFrozenBits(message, result, FrozenIndex);
	return bin2int(result, N - K);
	
}

void flipToBalanced(int *codeword){
  int flipNumber = N*fabs(0.5-measureDistribution(codeword));
  for(int i = 0; i < flipNumber; i++){
    codeword[i] = (1+codeword[i])%2;
  }
}

double  measureDistribution(int *codeword){
  int count = 0;
  for(int i = 0; i < N; i++)
    if(codeword[i] == 0)
      count++;
  return (double)count/N;
}
void experiment_for_rewriting(double **W, double **M, set<int>frozenIndex){
  int current[N], updated[N];
  distribution = 0;
  int data[N - K];
  long double rewritingCost = 0;
  for(int i = 0; i < trial; i++){
    //cout<<"Now i is "<<i<<endl;
    for(int j = 0; j < N - K; j++)
      data[j] = rand()%2; 
  
      int DataCopy[N];
      for(int i = 0; i < K; i++)
	DataCopy[i] = data[i];
      for(int i = K; i < N; i++)
	DataCopy[i] = rand()%2;
      encode(current, DataCopy);
      flipToBalanced(current);

      SCEncoding(data, updated, frozenIndex, data, W);
      //rewritter(current, dither, updated, data, frozenIndex, W);
      distribution += log2(fabs(measureDistribution(updated)-0.5)*N);
      //rewritingCost +=  measureDistortion(current, updated, M);
      if(i%1000 == 0){
	cout<<"now trial is "<<i<<endl;
	cout<<"average length is "<<(double)distribution/(i+1)<<endl;
	//cout<<"so far rewriting cost is "<<rewritingCost/(i + 1)<<endl;
      }					
  }
	
  distribution /= trial;
  rewritingCost /= trial;
  stringstream ss;
  ss<<N;
  string nstr = ss.str(); 
  string name = nstr + " PolarWEM result.txt";
  char *filename = new char[1000];
  strcpy(filename, name.c_str());
  ofstream out(filename);
  out<<"-------------------------\n";
  out<<"Polar Code for Write-Efficient Memories Experiment information\n";
  out<<"q is :"<<q<<endl;
  out<<"N is :"<<N<<endl;	
  out<<"K is :"<<K<<endl;
  out<<"rewriting capacity:" <<1 - 1*compute_I(W)<<endl;
  out<<"Rate (N - K/N):"<< 1 - 1*(double) K/ (double)N<<endl;
  out<<"Probability of one is "<<distribution<<endl;
  out<<"we have flip "<<abs(0.5-distribution)<<" fraction of cells"<<endl;
  out<<"Average rewriting cost: "<<rewritingCost<<endl;
  out<<"Theoretical bound for rewriting cost: "<<compute_D(W,M)<<endl;
  out<<"Trial number: "<<trial<<endl;
  out<<"\n\n\n\n                             Qing Li(qingli@cse.tamu.edu)";
  /*for(int i = 0; i < q; i++)
    free (L1[i]);
  free(L1);*/
  delete []filename;
}


