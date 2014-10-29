#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
* Responsible for creating random data of specified sizes.
* @param numBytes The length of the String in bytes.
*/
char* generateRandomString(int numBytes);

/**
* Generate file
*/
int generateFile(char* fileName,int numKeys);

/**
* Generates files
*/
void generateFiles(int numFiles,int numKeys); 

/**
* Generates files with a prefix filename.
*/
void generateFilesPrefix(int numFiles,int numKeys,char* prefix);
