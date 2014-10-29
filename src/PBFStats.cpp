#include "./PBFStats.h"

/**
* Responsible for calculating the stats relating to a pbf.
*/
void writeStats(FILE* pbfOutput,int* actual,int* counts,int numCounts,
	int k,float p,int n,int m){
	int i  = 0;
	for(;i<numCounts;i++){
		float a = 0.0f;	
		float b = 0.0f;
		a = (float)k*p*n+m*logf(1-(float)counts[i]/k);
		b = (float)(k-m)*p;
		float f = a/b;
		a = ((float)k*p*n+m*logf(1-(float)counts[i]/k+(float)1.96*sqrt((1-(float)(k-counts[i])/k)*(k-counts[i])/k/k)));
		b = (float)(k-m)*p;
		float fMin = a/b;
		a = ((float)k*p*n+m*logf(1-(float)counts[i]/k-(float)1.96*sqrt((1-(float)(k-counts[i])/k)*(k-counts[i])/k/k)));
		float fMax = a/b;
		float err = (f-actual[i])/(float)actual[i];		
		fprintf(pbfOutput,"%d,%d,%.2f,%d,%.4f\n",i,counts[i],f,actual[i],err);
	}

}

float calculateProb(int freq, int n)
{
	float e = 0.1;
	float ratio = 0.7;
	float a = (float)(n - freq)*logf(1-e) - (float)n*logf(e);
	float b = (float)n*freq;
	float p = ratio*a/b;
	//printf("p = %.6f\n",p);
	return p;
}

int calculateSize(int k, int n, float p)
{
	float e = 0.1;
	int m = 0;
	m = -k*n*p/logf(1-e);
	return m;
}

