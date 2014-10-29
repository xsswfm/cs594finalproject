#include "../ParseArgs.h"
#include "../RandomGenerator.h"
#include "../ParseData.h"
#include "../PBFStats.h"
#include "../Hash.h"
#include <time.h>
#include <math.h>

/**
* Inserts words into the pbf.
*/
void insertWords(char* bloom,BloomOptions_t* bloomOptions_t,
	WordAttributes* wordAttributes,float prob){
	int numWordsProcessed = 0;			
	for(;numWordsProcessed<wordAttributes->numWords;numWordsProcessed++){
		int y = 0; 
		for(y=0;y<bloomOptions_t->numHashes;y++){
			unsigned long firstValue = djb2HashOffset(wordAttributes->currentWords,
				wordAttributes->positions[numWordsProcessed])%bloomOptions_t->size;
			unsigned long secondValue = sdbmHashOffset(wordAttributes->currentWords,
				wordAttributes->positions[numWordsProcessed])%bloomOptions_t->size;
			int value = (firstValue+(y*y*secondValue)%bloomOptions_t->size)%
				bloomOptions_t->size;
			float temp = (float)rand()/RAND_MAX;					
			if(temp<prob)	
				bloom[value] = 1;
		}
	}
}

/**
* Responsible for querying the bloom filter.
*/
void queryWords(char* bloom,BloomOptions_t* bloomOptions_t,
	WordAttributes* wordAttributes,int* results){	
	int numWordsProcessed = 0;
	for(;numWordsProcessed<wordAttributes->numWords;numWordsProcessed++){
		int y = 0;
		int count = 0;
		for(y=0;y<bloomOptions_t->numHashes;y++){
			unsigned long firstValue = djb2HashOffset(wordAttributes->currentWords,
				wordAttributes->positions[numWordsProcessed])%bloomOptions_t->size;
			unsigned long secondValue = sdbmHashOffset(wordAttributes->currentWords,
				wordAttributes->positions[numWordsProcessed])%bloomOptions_t->size;
			int value = (firstValue+(y*y*secondValue)%bloomOptions_t->size)%
				bloomOptions_t->size;
			count+=bloom[value];
		}				
		results[numWordsProcessed] = count;		
	}
}

/**
* Main function
*/
int main(int argc,char** argv){
	//Seed the timer...
	srand(time(0));
	//printf("Begin Regular PBF..\n");

	//Does the user need help?
	if(wasArgSpecified("--help",argv,argc)!=0){
		printHelp();
		return 0;
	}

	//Initialize with default configuration.
	BloomOptions_t bloomOptions_t;
	setDefault(&bloomOptions_t);

	//Parse the user's configuration
	getConfiguration(&bloomOptions_t,argv,argc);
	bloomOptions_t.prob = calculateProb(bloomOptions_t.freq,bloomOptions_t.numKeys);
	bloomOptions_t.size = calculateSize(bloomOptions_t.numHashes,bloomOptions_t.numKeys,bloomOptions_t.prob);

	//Show the user the configuration.
	//showDetails(&bloomOptions_t);

	//Create the bloom filter being used, and initailize with all 0's.
	char* bloom = (char*)calloc(sizeof(char)*bloomOptions_t.size,sizeof(char));


	//Read input keys and insert to PBF
	char* fileName = (char*)malloc(sizeof(char)*50);
	sprintf(fileName,"./data/total_keys.txt");
	WordAttributes* allKeys = loadFileByName(fileName);
	int total_keys = allKeys->numWords;
	insertWords(bloom,&bloomOptions_t,allKeys,bloomOptions_t.prob);
	freeWordAttributes(allKeys);
	
	//Query PBF
	sprintf(fileName,"./data/distinct_keys.txt");
	WordAttributes* distinctKeys = loadFileByName(fileName);
	int distinct_keys = distinctKeys->numWords;
	//printf("distinct keys = %d, total keys = %d\n",distinct_keys,total_keys);
	int* results = (int*)calloc(sizeof(int)*distinct_keys,sizeof(int));
	queryWords(bloom,&bloomOptions_t,distinctKeys,results);
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
	free(bloom);
	//printf("\n");
	return 0;
}
