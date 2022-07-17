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

#define DEBUG 1 // Allows debugging by skipping VCBPtr->sig == signature check

directoryEntry * entries[NUM_ENTRIES];

void initRootDirectory(VCB* VCBPtr);

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
		
	return 0; 			// return value unsure on 0
	}

void initRootDirectory(VCB* VCBPtr){
	// Deciding on 50 Directory Entries for now (based on MS1 steps)
	// DE Struct size is 40
	// Math works out to 4 blocks, 51 Directory Entries
	// 50 * 40 = 2000, 512 / 2000 = 3.9 -> approx. 4
	// 4 * 512 = 2048
	// 51*40 = 2040 -> 51 DEs

	for(int i = 0; i < NUM_ENTRIES ; i++){
		entries[i] = NULL; // Null is free state	
	}
	
	// Asking for starting block of allocated space from freespace.c
	int startingBlock = freeSpaceRequest(DIR_ENTRY_BLOCKS);
	time_t rawTime;					// rawtime for time part of entry
	entries[0] = malloc(sizeof(directoryEntry));	// malloc space for entries
	
	// Initializing member variables of the struct for "." and ".." -> root directories
	strcpy(entries[0]->name, ".");	
	entries[0]->size = ENTRY_MEMORY;
	entries[0]->location = startingBlock;
	entries[0]->time = time( &rawTime );
	printf("address: %llx\n", (unsigned long long int)entries[0]); 
	entries[1] = malloc(sizeof(directoryEntry));
	strcpy(entries[1]->name, "..");
	entries[1]->size = ENTRY_MEMORY;
	entries[1]->location = startingBlock;
	entries[1]->time = time( &rawTime );

	VCBPtr->rootBlockNum = startingBlock; // setting rootBlockNum to beginning or root dir
}

	
void exitFileSystem (){
	printf ("System exiting\n");
	free(VCBPtr); 
	VCBPtr = NULL;
	for(int i = 0; i < NUM_ENTRIES; i++){
		if(entries[i] != NULL){
			free(entries[i]); // freeing allocated space
			entries[i] = NULL; // setting null -> known free state
		}

	}
	}
