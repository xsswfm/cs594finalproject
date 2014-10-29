#include "../RandomGenerator.h"
#include "../ParseData.h"
#include "../PBFStats.h"
#include "Bloom.h"
#include <time.h>
#include <math.h>

/**
*
*/
int main(int argc,char** argv){
	
	//Seed the timer.
	srand(time(0));
	//printf("Begin GPU PBF..\n");

	//Does the user need help?
	if(wasArgSpecified("--help",argv,argc)!=0){
		printHelp();
		return 0;
	}
	
	//Initialize with default configuration.
	BloomOptions_t bloomOptions_t;
	setDefault(&bloomOptions_t);	
	//Parse the user's configuration.
	getConfiguration(&bloomOptions_t,argv,argc);
	bloomOptions_t.prob = calculateProb(bloomOptions_t.freq,bloomOptions_t.numKeys);
	bloomOptions_t.size = calculateSize(bloomOptions_t.numHashes,bloomOptions_t.numKeys,bloomOptions_t.prob);

	//showDetails(&bloomOptions_t);

	//Create the bloom filter being used, and initialize it with all 0's.
	char* bloom = (char*)malloc(sizeof(char)*bloomOptions_t.size);
	memset(bloom,0,bloomOptions_t.size);

	//Allocate the GPU bloom filter.
	char* dev_bloom = allocateAndCopyChar(bloom,bloomOptions_t.size); 
	if(dev_bloom==0){
		printf("Could not allocate the bloom filter \n");
		return -1;	
	}

	//Read input keys and insert to PBF
	char* fileName = (char*)malloc(sizeof(char)*50);
	sprintf(fileName,"./data/total_keys.txt");
	WordAttributes* allKeys = loadFileByName(fileName);
	int total_keys = allKeys->numWords;	
	int randOffset = rand()%2432+10;			
	insertWordsPBF(dev_bloom,bloomOptions_t.size,allKeys->currentWords,allKeys->positions,allKeys->numWords,allKeys->numBytes,bloomOptions_t.numHashes,bloomOptions_t.device,bloomOptions_t.prob,randOffset);
	freeWordAttributes(allKeys);
			
	//Query PBF
	sprintf(fileName,"./data/distinct_keys.txt");
	WordAttributes* distinctKeys = loadFileByName(fileName);
	int distinct_keys = distinctKeys->numWords;
	//printf("distinct keys = %d, total keys = %d\n",distinct_keys,total_keys);
	int* results = (int*)calloc(sizeof(int)*distinct_keys,sizeof(int));
	queryWordsPBF(dev_bloom,bloomOptions_t.size,distinctKeys->currentWords,distinctKeys->positions,distinctKeys->numWords,
distinctKeys->numBytes,bloomOptions_t.numHashes,bloomOptions_t.device,results);
	freeWordAttributes(distinctKeys);

	//Read the actual frequency of keys
	int* actual = (int*)calloc(sizeof(int)*distinct_keys,sizeof(int));
	FILE* actualFreqFile = fopen("./data/freq.txt","r");
	if(!actualFreqFile){
		printf("Can not open actual frequency file!\n");
		return -1;
	}
	for(int i = 0; i < distinct_keys; i++){
		char* temp = (char*)malloc(sizeof(char)*15);
		if(fgets(temp,15,actualFreqFile)!=NULL){
			int len = strlen(temp);
			temp[len-1]='\0'; 		
			actual[i]=atoi(temp);
			//printf("%d: actual = %d\n",i,actual[i]);
		}
		free(temp);
	}
	
	//Copy the bloom filter to main memory.
	//copyCharsToHost(bloom,dev_bloom,bloomOptions_t.size);

	//Write the result to output file
	//data format: index, number of 1s, calculated frequency, actual frequency, relative error
	if(bloomOptions_t.pbfOutput){
		FILE* outputFile = fopen(bloomOptions_t.pbfOutput,"w");
		writeStats(outputFile,actual,results,distinct_keys, bloomOptions_t.numHashes,bloomOptions_t.prob,total_keys,bloomOptions_t.size);
		fclose(outputFile);
	}
	
	free(fileName);
	free(actual);
	free(results);
	freeChars(dev_bloom);	
	free(bloom);
	//printf("\n");
	return 0;
}
