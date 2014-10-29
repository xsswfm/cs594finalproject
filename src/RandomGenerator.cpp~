#include "./RandomGenerator.h"

/**
* Responsible for creating random data of specified sizes.
* @param numBytes The length of the String in bytes.
*/
char* generateRandomString(int numBytes){
	char* randomString = (char*)malloc(numBytes);
	int i =0;
	for(i =0; i<numBytes-1;i++){
		randomString[i] = random()%49+48;
	}
	randomString[numBytes-1]=',';
	return randomString;
}

/**
* Generate file with random strings.
* @param fileName the name of the file being generated
* @param numBytes the number of bytes in the file.
* @return Returns the number of words written.
*/
int generateFile(char* fileName,int numKeys){
	/************
	int totalBytesAdded = 0;
	int numWords = 0;
	char* fileBuffer = (char*)malloc(sizeof(char)*numBytes); 
	while(1){
		int numBytesString = random()%50+2;
		char* randomString = generateRandomString(numBytesString);
		if(totalBytesAdded+numBytesString>=numBytes){
			free(randomString);
			break;
		}
		memcpy(fileBuffer+totalBytesAdded,randomString,numBytesString);
		free(randomString);
		totalBytesAdded+=numBytesString;
		numWords++;
	}
	
	fwrite(fileBuffer,sizeof(char),totalBytesAdded,generatedFile);
	****************/
	int i=0;
	FILE* generatedFile = fopen(fileName,"w");
	for(i=0;i<numKeys;i++)
	{
		//random length and generate random string		
		int numBytesString = random()%50+2;
		char* randomString = generateRandomString(numBytesString);
		fwrite(randomString,sizeof(char),numBytesString,generatedFile);	
		free(randomString);
	}	
	fclose(generatedFile);	
	//free(fileBuffer);
	return numKeys;
}

/**
* Generate files with random Strings. 
* @param bloomOptions_t The bloom options used to create the filter.
*/
void generateFiles(int numFiles, int numKeys){
	//Seed the timer
	srand(time(0));
	int i = 0;
	for(i = 0; i<numFiles; i++){
		char* newFile = (char*)malloc(sizeof(char)*FILENAME_MAX);
		sprintf(newFile,"%i",i);
		strcat(newFile,".txt");
		generateFile(newFile,numKeys);
		free(newFile);	
	}
}

/**
* Generates files with a prefix filename.
*/
void generateFilesPrefix(int numFiles,int numKeys,char* prefix){
	//Seed the timer
	int i = 0;
	for(i = 0; i<numFiles; i++){
		char* newFile = (char*)malloc(sizeof(char)*FILENAME_MAX);
		sprintf(newFile,"%s%i",prefix,i);
		strcat(newFile,".txt");
		generateFile(newFile,numKeys);
		free(newFile);	
	}
}
