#include "mfs.h"
//#include "fsInit.c"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//#include "parsePath.h"


typedef struct parsedInfo{
    int isPathValid;  // 1 for valid / 0 for invalid
    int isFile;      // 1 for file  / 0 for directory / -1 if invalid ?
    int lastElementIndex;
    char newEntryName[NAME_LIMIT];
    directoryEntry* parent;
    // add more fields here
} parsedInfo;
// TODO: separate into header files

void parsePath(directoryEntry * cwd, directoryEntry* root, char* pathToParse, parsedInfo * info ){

    char prev[NAME_LIMIT] = "";
    char curr[NAME_LIMIT] = "";
    directoryEntry * myCwd;
    char * token = strtok(pathToParse, "/");

    printf("first: %s \n", token);
    
    info->isFile = -1;
    info->lastElementIndex = -1; 

    if(pathToParse[0] == '/') // loading root directory first
	    myCwd = root;
    else
	    myCwd = cwd;

    int i = 0;
    while(token != NULL){
	strcpy(prev, curr);
	strcpy(curr, token);
	token = strtok(NULL, "/");	    
	printf("itr: %d, prev: %s, curr: %s, token: %s\n", i++, prev, curr, token);
	    // if token is NOT Null
	    // for each curr word, search in the myCwd's directoryEntries to see if it exists. 
	    // If it does exist, change myCwd = myCwd->entries[index]. If it doesn't, error
	    // if token IS Null -> special case
	    // check if curr exists 
	    // if curr exists, find the index of curr from the parent or use .. ? 
	    // if curr doesn't exist, return the parent memory location
		if(token != NULL){
	    		int index = 1; 
			while(myCwd->entries[++index] != NULL){
				if(!(strcmp(curr, myCwd->entries[index]->name))){
					myCwd = myCwd->entries[index];
					break;
				}

				if(myCwd->entries[index] == NULL){
					info->isPathValid = 0;
					return;
				}
			}
		}
		else if(token == NULL){ // NOTE: MyCwd == prev or the parent of "curr"
			info->parent = myCwd;
			strcpy(info->newEntryName, curr);
			info->isPathValid = 1; // at this process point, path is valid
			// next step: loop thru cwd->entries and see if curr exists
			// if it does, get index, filetype(?) and return
			// if it doesn't, lastElementIndex stays -1 to denote file not existing 
			printf("cwd name: %s\n", myCwd->name);
			
			int itr = 1; // 0 = ., 1 = .., in loop will be entries[2]
			while(myCwd->entries[++itr] != NULL){ // Finds curr in prev entries
				if(!(strcmp(myCwd->entries[itr]->name, curr))){
					info->lastElementIndex = itr;
					info->isFile = myCwd->entries[itr]->isFile;
					return;
				}
			} 
			// at end of while, means curr was not found in myCwd->entries
			return;
		}
	}
		    
    
}
