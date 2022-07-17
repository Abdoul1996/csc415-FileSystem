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

#include "fsLow.h"
#include "mfs.h"

#define SIGNATURE 0x414142424A434B4D

int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize)
	{
	printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */

	// Malloc a block of memory as VCB pointer (MS1)
	VCBPtr = malloc(blockSize);
	
	// LBAread block 0 (MS1)
	LBAread(VCBPtr, 1, 0);

	// "Magic Number" check (MS1)
	if((VCBPtr->sig == SIGNATURE))
		return 0;	
	VCBPtr->sig = SIGNATURE;
	VCBPtr->blockNumber = numberOfBlocks;
	VCBPtr->blockSize = blockSize;
	// TODO: VCBPtr->freeSpace = 
	// TODO: VCBPtr->rootBlockNum = 
	LBAwrite(VCBPtr, 1, 0);	
	
	return 0;
	}

void initRootDirectory(VCB* VCBPtr){

}
	
void exitFileSystem ()
	{
	printf ("System exiting\n");
	free(VCBPtr);
	VCBPtr = NULL;
	}
