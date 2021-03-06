#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<time.h>
#include<vector>
#include<sys/time.h>
#include<sys/resource.h>
using namespace std;
int PPartition(vector<long double>&W, int start, int end){
	
	int i = start - 1;
	long double elem = W[end];
	//exchange W[start + index] with W[end]
	for (int j = start; j <= end - 1; j++){
		if(W[j] <= elem){
			//exchange W[j] and W[i]
		        i++;
			swap(W[j],W[i]);
		}	
	}
	//exchange W[i + 1] and W[end] and exchange W[ i + 1 + W.size()/2] and W[end + W.size()/2]
	swap(W[i + 1], W[end]);	
	return i + 1;
}

void QQuickSort(vector<long double>&W, int start, int end){
	if (start < end){
	  int q = PPartition(W, start, end);
		QQuickSort(W, start, q - 1);
		QQuickSort(W, q + 1, end);		
	}
}


/*-----------------------------------------------------------
FUNCTION:
         long double selection(int m, vector<long double>seq)
DESCRIPTION:
	Implementation of section 9.3 selection in worst-case linear time 
        of the book introduction to aglorithm
	
	step 1, divide the n elements of the input array into [n/5] groups of 5 elements each and at most one group made up of the remaining n mod5 elements
	
	step 2, find the median of each of the [n/5] groups by first insertion sorting the elements
         of each group (of which there are at most 5) and then picking the median from the sorted list of group elements.
	
	 step 3: use SELECT recursively to find the median x of the [n/5] medians found in step 2

	 step 4: partition the input array around the median-of-medians x using the modified version of PARTITION.

	 step 5: if m = k, the return x. Otherwise, use SELECT recursively to find the ith smallest element on the low side if m < k, or the (m - k)th smallest element on the high side if m > k.

PARAMETERS:
        INPUT:
             m-- we want to select the m-th largest element
             sequence -- the input sequence
	OUTPUT:
	     the m-th largest element
RETURN VALUES:
       the m-th largest element
------------------------------------------------------------*/
long double selection(int m, vector<long double>sequence){
  //step 1 and step 2: 
  vector<long double>median, seq(sequence);
  int i;
  for(i = 0; i < (int)seq.size()/5; i++){
    QQuickSort(seq, i*5, (i+1)*5 -1);
    median.push_back(seq[i*5 + 2]);
  }
  if(i*5 < (int)seq.size()){
    QQuickSort(seq, i*5, seq.size() - 1);
    median.push_back(seq[(i*5 + seq.size())/2]);
  }
  if (median.size() == 1) return seq[m-1];
  //step 3
  long double medianOfMedians = selection(median.size()/2 , median);
  vector<long double>Left, Right;
  for(int i = 0; i < (int)sequence.size(); i++){
    if(sequence[i]<medianOfMedians)
      Left.push_back(sequence[i]);
    else
      Right.push_back(sequence[i]);
  }
  int k = Left.size() + 1;
  if (m == k) {return medianOfMedians;}
  else if(m < k) {return selection(m, Left);}
    else { return selection(m - k + 1, Right);}
}



