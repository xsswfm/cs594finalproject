#include <math.h>
#include <stdio.h>

/**
* Responsible for calculating the stats relating to a pbf.
*/
void writeStats(FILE* pbfOutput,int* actual,int* counts,int numCounts,
	int k,float p,int n,int m);

float calculateProb(int freq, int n);

int calculateSize(int k, int n, float p);
