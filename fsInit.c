/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Abdoulfatah Abdillahi, Brandon Butler, Jacob Cross, Kevin Manago
* Student IDs: 921623492, 920819091, 916168167, 917784912
* GitHub Name: Abdoul1996, BrandonButler08, JacobCross, Kmanago95
* Group Name: Team Shield
* Project: Basic File System
*
* File: fsInit.c
*
* Description: Main driver for file system assignment.
*
* This file is where you will start and initialize your system
*
**************************************************************/


#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fsLow.h"
#include "mfs.h"
#include "fsFreeSpace.c"

#include "parsePath.c"

#define DEBUG 1 // Allows debugging by skipping VCBPtr->sig == signature check

directoryEntry * root;
directoryEntry * cwd;
void initRootDirectory(VCB* VCBPtr);

directoryEntry * createDir(char* name, int isFile, directoryEntry* parent); 

int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize)
	{
	printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */

	VCBPtr = malloc(blockSize);	// malloc block of memory
	LBAread(VCBPtr, 1, 0);		// grab VCBPtr to check if initialized correctly	
	
#if !DEBUG
	if((VCBPtr->sig == SIGNATURE))	// if signature is correct, return 0
		return 0;	
#endif
	// initializing Volume control block info if signature doesnt match
	// signature doesn't match = not initialized
	VCBPtr->sig = SIGNATURE;
	VCBPtr->blockNumber = numberOfBlocks;
	VCBPtr->blockSize = blockSize;
	VCBPtr->freeSpace = initFreeSpace(); 	
	initRootDirectory(VCBPtr);	// init function sets the rootBlockNumber
	LBAwrite(VCBPtr, 1, 0);		// write data to hard disk / memory

	// initParsePath(entires)?
	
		
	return 0; 			// return value unsure on 0
	}

void initRootDirectory(VCB* VCBPtr){
	// Deciding on 50 Directory Entries for now (based on MS1 steps)
	// DE Struct size is 40
	// Math works out to 4 blocks, 51 Directory Entries
	// 50 * 40 = 2000, 512 / 2000 = 3.9 -> approx. 4
	// 4 * 512 = 2048
	// 51*40 = 2040 -> 51 DEs

	// TODO: Change entries to be root->entries
//	for(int i = 0; i < NUM_ENTRIES ; i++){
//		root->entries[i] = NULL; // Null is free state	
//	}
	
	// Asking for starting block of allocated space from freespace.c
	int startingBlock = freeSpaceRequest(DIR_ENTRY_BLOCKS);
	time_t rawTime;					// rawtime for time part of entry
	root = malloc(sizeof(directoryEntry));	// malloc space for entries
	root->entries = malloc(sizeof(directoryEntry*) * NUM_ENTRIES);
	for(int i = 0; i < NUM_ENTRIES ; i++){
                root->entries[i] = NULL; // Null is free state  
        }


	// Initializing member variables of the struct for "." and ".." -> root directories
	root->entries[0] = malloc(sizeof(directoryEntry));
	strcpy(root->entries[0]->name, ".");	
	root->entries[0]->size = ENTRY_MEMORY;
	root->entries[0]->location = startingBlock;
	root->entries[0]->time = time( &rawTime );
	root->entries[0]->isFile = 0;
	root->entries[0]->entries = root->entries;
	
	
	// printf("address: %llx\n", (unsigned long long int)entries[0]); 
	root->entries[1] = malloc(sizeof(directoryEntry));
	strcpy(root->entries[1]->name, "..");
	root->entries[1]->size = ENTRY_MEMORY;
	root->entries[1]->location = startingBlock;
	root->entries[1]->time = time( &rawTime );
	root->entries[1]->isFile = 0;
	root->entries[1]->entries = root->entries;

	cwd = root;
	parsedInfo * info = malloc(sizeof(parsedInfo));
	char pathToParse[20] = "/home/var/this/that";
	directoryEntry* home = createDir("home", 0, root);
	directoryEntry* var = createDir("var", 0, home);
	directoryEntry* this = createDir("this", 0, var);
	directoryEntry* that = createDir("that", 0, this);
	parsePath(cwd, root, pathToParse, info);
	
	VCBPtr->rootBlockNum = startingBlock; // setting rootBlockNum to beginning or root dir
}

directoryEntry * createDir(char* name, int isFile, directoryEntry* parent){
	int index = -1;

	// Create a directoryEntry in the parent's entries
	// Checking if there is any space
	for(int i = 2; i < NUM_ENTRIES; i++){
		if(parent->entries[i] == NULL){
			index = i;
			break;
		}
	}

	if(index == -1)
		return NULL;
	
	// allocating memory for the new directory to be added
	directoryEntry* currentDir = malloc(sizeof(directoryEntry));
	int freeSpaceBlock = freeSpaceRequest(DIR_ENTRY_BLOCKS); 
	time_t rawTime;
	strcpy(currentDir->name, name);
	currentDir->size = ENTRY_MEMORY;
	currentDir->location = freeSpaceBlock;
	currentDir->time = time( &rawTime );
	currentDir->isFile = isFile;
	
	// allocating memory for the entries of new directory to be added
	currentDir->entries = malloc(sizeof(directoryEntry*) * NUM_ENTRIES);

	// free state
	for(int i = 0; i < NUM_ENTRIES; i++){
		currentDir->entries[i] = NULL;
	}
	// initializing . (self) and .. (parent) directories
	currentDir->entries[0] = malloc(sizeof(directoryEntry));
	strcpy(currentDir->entries[0]->name, ".");
	currentDir->entries[0]->size = currentDir->size;
	currentDir->entries[0]->location = freeSpaceBlock;
	currentDir->entries[0]->time = currentDir->time;
	currentDir->entries[0]->isFile = currentDir->isFile;
	
	currentDir->entries[1] = malloc(sizeof(directoryEntry));
	strcpy(currentDir->entries[1]->name, "..");
	currentDir->entries[1]->size = parent->size;
	currentDir->entries[1]->location = parent->location;
	currentDir->entries[1]->time = parent->time;
	currentDir->entries[1]->isFile = parent->isFile;
	currentDir->entries[1]->entries = parent->entries;

	// setting . to go back to self
	currentDir->entries[0]->entries = currentDir->entries;
	// setting the new directory to the index of parent 
	parent->entries[index] = currentDir;
	
	// return directoryEntry memory location of the child 
	return currentDir;
}


	
void exitFileSystem (){
	printf ("System exiting\n");
	free(VCBPtr); 
	VCBPtr = NULL;
	for(int i = 0; i < NUM_ENTRIES; i++){
		if(root->entries[i] != NULL){
			free(root->entries[i]); // freeing allocated space
			root->entries[i] = NULL; // setting null -> known free state
		}

	}
	}
