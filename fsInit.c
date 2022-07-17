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


directoryEntry entries[NUM_ENTRIES];

void initRootDirectory(VCB* VCBPtr);

int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize)
	{
	printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */

	// Malloc a block of memory as VCB pointer (MS1)
	VCBPtr = malloc(blockSize);
	printf("Size of Directory Entry Struct: %ld\n", sizeof(struct directoryEntry));


	// LBAread block 0 (MS1)
	LBAread(VCBPtr, 1, 0);
	
	// "Magic Number" check (MS1)
	//if((VCBPtr->sig == SIGNATURE))
	//	return 0;	
	VCBPtr->sig = SIGNATURE;
	VCBPtr->blockNumber = numberOfBlocks;
	VCBPtr->blockSize = blockSize;
	// TODO: VCBPtr->freeSpace = 
	// TODO: VCBPtr->rootBlockNum = 
	
	initRootDirectory(VCBPtr);
	LBAwrite(VCBPtr, 1, 0);	
		
	return 0;
	}

void initRootDirectory(VCB* VCBPtr){
	// Decide how many DEs I want
	// 50 for now I guess
	// size of struct DE is 40, 40*50 is 2000, 2000/512 = 3.9 = 4, 4*512 = 2048
	// 51*40 = 2040 -> 51 entries	
	for(int i = 0; i < NUM_ENTRIES ; i++){
		//TODO: initialize each structure to be in a known free state	
	}

	//TODO: ask free space system for 6 blocks, returns starting block
	int startingBlock;
	time_t rawTime;
	
	strcpy(entries[0].name, ".");
	entries[0].size = ENTRY_MEMORY;
	entries[0].location = startingBlock;
	entries[0].time = time( &rawTime );

	strcpy(entries[1].name, "..");
	entries[1].size = ENTRY_MEMORY;
	entries[1].location = startingBlock;
	entries[1].time = time( &rawTime );

	VCBPtr->rootBlockNum = startingBlock;
}

	
void exitFileSystem ()
	{
	printf ("System exiting\n");
	free(VCBPtr);
	VCBPtr = NULL;
	}
