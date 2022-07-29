/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Abdoulfatah Abdillahi, Brandon Butler, Jacob Cross, Kevin Manago
* Student IDs: 921623492, 920819091, 916168167, 917784912
* GitHub Name: Abdoul1996, BrandonButler08, JacobCross, Kmanago95
* Group Name: Team Shield
* Project: Basic File System
*
* File: fsFreeSpace.c
*
* Description: bitmap file system.
*
*
**************************************************************/

// TODO: FreeSpaceRequest needs to be fixed per MS1 comments from Bierman
// TODO: API to release space and make it free


#include <stdio.h>
#include <stdint.h>
#include <mfs.h>

#define CHARSIZE 8 		// Number for the 8 bits in an unsigned char
#define TOTAL_FREE_SPACE 2560 	// Free space bytes as denoted by MS1 steps

int freeSpaceLocation = 0;	// Number that signifies beginning of free space

unsigned char freeSpace[TOTAL_FREE_SPACE];

int initFreeSpace(){
	for(int i = 0; i < TOTAL_FREE_SPACE; i++)
		freeSpace[i] = 0;	// Setting whole bitmap to 0
	// Setting 0th bit to 1 for VCB
	freeSpace[0] = freeSpace[0] | (1 << (CHARSIZE - 0 + 1));
	// Incrementing freeSpaceLocation since bit 0 is allocated
	freeSpaceLocation++;
	for(int i = 1; i <= 5; i++){ 	// Setting the next 5 blocks to 1 for free space map
		int block = i/CHARSIZE;
		freeSpace[block] = freeSpace[block] | (1 << (CHARSIZE - i%8));
		freeSpaceLocation++;
	}

	LBAwrite(freeSpace, 5, 1);	// Writing freespace map to disk with LBAwrite
	return freeSpaceLocation;	// Beginning of free space 
}

int freeSpaceRequest(int numBlocks){
	// return starting position of the blocks
	// TODO: Edge case when freeSpaceLocation == TOTAL_FREE_SPACE
	int startingPosition = freeSpaceLocation;
	
	int fieldNum = 0;
	for(int i = 1; i <= numBlocks; i++){
		fieldNum = freeSpaceLocation/CHARSIZE;	//Specifies specific byte of map
		// Sets specific bit to 1 based on map location 
		freeSpace[fieldNum] = freeSpace[fieldNum] | ( 1 << (CHARSIZE - freeSpaceLocation % 8 + 1));
		freeSpaceLocation++;
	}
	
	//TODO: Change free space request to _maybe_ search map and then allocate closest to the request
	// There might be an issue when data is being freed 
	// and some spaces are going to be released

	return startingPosition;

}

