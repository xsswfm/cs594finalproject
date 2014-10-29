#include "../RandomGenerator.h"
#include "../ParseArgs.h"
#include <time.h>

int main(int argc, char** argv)
{
	srand((int)time(NULL));
	int distinct_keys = 5;
	int total_keys = 100;

	char* arg_value = getArgValue("--distinct",argv,argc);
	if(arg_value!=0){
		distinct_keys = atoi(arg_value);
	}
	arg_value = getArgValue("--total",argv,argc);
	if(arg_value!=0){
		total_keys = atoi(arg_value);
	}
	
	//three output files
	//@keyFile: all distinct keys
	//@allKeysFile: input file for PBF
	//@freqFile: frequency of each distinct key
	FILE* keyFile;
	FILE* allKeysFile;
	FILE* freqFile;

	//generate distinct keys
	keyFile = fopen("./data/distinct_keys.txt","w");
	char** key = (char**)malloc(sizeof(char*)*distinct_keys);
	for(int i = 0;i < distinct_keys; i++){
		//key[i] = (char*)malloc(sizeof(char)*100);
		int len = rand()%50 + 2;
		//printf("%d: len = %d\n",i,len);
		key[i] = generateRandomString(len);
		fwrite(key[i],sizeof(char),len,keyFile);		
	}
	fclose(keyFile);

	//keep record of frequency of each distinct key
	int* freq = (int*)malloc(sizeof(int)*distinct_keys);
	for(int i = 0; i < distinct_keys; i++){
		freq[i] = 0;
	}

	//generate the input file of the PBF
	allKeysFile = fopen("./data/total_keys.txt","w");
	for(int i = 0; i < total_keys; i++){
		int temp = rand()%distinct_keys;
		freq[temp]++;
		//printf("%d: idx = %d\n",i,temp);
		int len = strlen(key[temp]); 
		fwrite(key[temp],sizeof(char),len,allKeysFile);
	}
	fclose(allKeysFile);

	freqFile = fopen("./data/freq.txt","w");
	for(int i = 0; i < distinct_keys; i++){
		free(key[i]);
		//printf("%d: freq = %d\n",i,freq[i]);
		fprintf(freqFile,"%d\n",freq[i]);
	}
	fclose(freqFile);
	free(key);
	free(freq);
	return 0;
}


