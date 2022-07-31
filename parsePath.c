/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Abdoulfatah Abdillahi, Brandon Butler, Jacob Cross, Kevin Manago
* Student IDs: 921623492, 920819091, 916168167, 917784912
* GitHub Name: Abdoul1996, BrandonButler08, JacobCross, Kmanago95
* Group Name: Team Shield
* Project: Basic File System
*
* File: parsePath.c
*
* Description: Parse path function to parse the path of a given string
*
**************************************************************/

#include "mfs.h"
//#include "fsInit.c"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "parsePath.h"


void parsePath(directoryEntry * cwd, directoryEntry* root, char* pathToParse, parsedInfo * info ){
	// This function will traverse the path by using / as a delimiter
	// We will keep track of everything by pointing to the previous, current, and the next strings in the path
	// For example, /foo/bar/this/that 
	// in the case where curr is bar, prev is foo and next (token in this case) is this
	// This is used to take care of the special case where the next or token is null
	
    char prev[NAME_LIMIT] = ""; 
    char curr[NAME_LIMIT] = "";
    directoryEntry * myCwd;						// Used to store the current working directory / root
    char * token = strtok(pathToParse, "/");	// Initial string split 
    
    info->isFile = -1;							// Setting isFile identifier to -1 initially, will always be set to 0/1 unless error
    info->lastElementIndex = -1; 				// Setting lastElIndex to -1 for error checking, will assign later
	printf("IN PARSE PATH . . . \n");
	printf("pathToParse: %s\n", pathToParse);
    if(pathToParse[0] == '/') 					// Loading root dir if first char is a / otherwise, using CWD.
	    myCwd = root;
    else
	    myCwd = cwd;
	
    int i = 0;
    printf("initial token: %s", token);
    while(token != NULL){	
	strcpy(prev, curr);						// This logic will make it so that prev, curr, and token have the
	strcpy(curr, token);						// previous, current, and next entries.
	token = strtok(NULL, "/");	    
	    // if token is NOT Null -> case 1
	    // for each curr word, search in the myCwd's directoryEntries to see if it exists. 
	    // If it does exist, change myCwd = myCwd->entries[index]. If it doesn't, error
	    // if token IS Null -> case 2
	    // check if curr exists 
	    // if curr exists, find the index of curr from the parent or use .. ? 
	    // if curr doesn't exist, return the parent memory location
		if(token != NULL){ 						// case 1
	    	int index = 1; 
			// Looping through CWD entries to see if any match "curr".
			// If any match, set CWD to that entry and continue 
			while(myCwd->entries[++index] != NULL){
				if(!(strcmp(curr, myCwd->entries[index]->name))){
					myCwd = myCwd->entries[index];
					break;
				}
				// Error condition, no entries match the current token, therefore invalid path
				if(myCwd->entries[index] == NULL){
					info->isPathValid = 0;
					return;
				}
			}
		}
		// Case 2
		else if(token == NULL){ // NOTE: MyCwd == prev or the parent of "curr"
			if(myCwd == NULL)	// if prev is null, that means curr did not move, assuming parent is the current directory
				info->parent = root;
			else
				info->parent = myCwd;			
			strcpy(info->newEntryName, curr);	// Setting newEntryName to current, possibly not necessary to do

			info->isPathValid = 1; // at this process point, path is valid
			// next step: loop thru cwd->entries and see if curr exists
			// if it does, get index, filetype(?) and return
			// if it doesn't, lastElementIndex stays -1 to denote file not existing 
			
			int itr = 1; // 0 = ., 1 = .., in loop will be entries[2]
			while(myCwd->entries[++itr] != NULL){ // Finds last entry in parent entries
				if(!(strcmp(myCwd->entries[itr]->name, curr))){
					info->lastElementIndex = itr;
					info->isFile = myCwd->entries[itr]->isFile;
					return;
				}
			} 
			// at end of while, means curr was not found in myCwd->entries
			// This means that the path exists, but the file/directory does not. 
			return;
		}
	}
		    
    
}
