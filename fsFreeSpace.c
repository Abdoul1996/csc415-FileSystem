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
#include "fsFreeSpace.h"
#define CHARSIZE 8 		// Number for the 8 bits in an unsigned char
#define TOTAL_FREE_SPACE 2560 	// Free space bytes as denoted by MS1 steps
#define FREE_SPACE_LIMIT CHARSIZE*TOTAL_FREE_SPACE
#define FREE_SPACE_DEBUG 0
int freeSpaceLocation = 0;	// Number that signifies beginning of free space
int numSpace = 0;
unsigned char freeSpace[TOTAL_FREE_SPACE];

int initFreeSpace(){
	for(int i = 0; i < TOTAL_FREE_SPACE; i++)
		freeSpace[i] = 0;	// Setting whole bitmap to 0
	// Setting 0th bit to 1 for VCB
	int requestedSpace = freeSpaceRequest(6);	// Returns start location of space requested. Need to increment for return val
	requestedSpace+=6;
	// Incrementing freeSpaceLocation since bit 0 is allocated
		
	LBAwrite(freeSpace, 5, 1);	// Writing freespace map to disk with LBAwrite
	return requestedSpace;	// Beginning of free space 
}

int freeSpaceRequest(int numBlocks){
#if FREE_SPACE_DEBUG
	printf("REQUESTING FREE SPACE . . .\n");
#endif
	int byteNum = 0;
	int bitNum = freeSpaceSearch(); // returns number from 0 - 2560*8
#if FREE_SPACE_DEBUG
	printf("FOUND FREE SPACE AT . . . %d\n", bitNum);
#endif
	if(bitNum == -1)
		return -1; // error, no more space
	int returnVal = bitNum;
	for(int i = 1; i<= numBlocks; i++){
		byteNum = bitNum/CHARSIZE;
#if FREE_SPACE_DEBUG
		printf("byteNum: %d, bitNum: %d, setting bit: %x\n", byteNum, bitNum, (1 << (CHARSIZE - ((bitNum % CHARSIZE) + 1))) );
#endif
		freeSpace[byteNum] = freeSpace[byteNum] | ( 1 << (CHARSIZE - ((bitNum % CHARSIZE) + 1)));
		bitNum++;
	}

	return returnVal;
}

int freeSpaceSearch(){
#if FREE_SPACE_DEBUG
	printf("IN FREE SPACE SEARCH . . .\n");
#endif
	int bitLocation = 0;
	for(int byteNum = 0; byteNum < TOTAL_FREE_SPACE; byteNum++){
		for(int bitNum = 0; bitNum < CHARSIZE; bitNum++){
#if FREE_SPACE_DEBUG
			printf("SEARCH . . . BYTENUM: %d, SEARCHING AT: %x \n", byteNum, (freeSpace[byteNum] & (1 << (CHARSIZE - (bitNum%8 + 1)))));
#endif
			if(!(freeSpace[byteNum] & (1 << (CHARSIZE - (bitNum%8 + 1)))))
				return bitLocation;
			bitLocation++;
		}
	}	
	return -1;
}

int releaseSpace(int entryLocation, int numBlocks){
// Goal: To release space, the bits of the map have to be set to 0
// Using entry, we can find the bit location (bitNum) of the space to clear
// byteNum = bitNum / CHARSIZE 
// byteNum can specify the chunk of this and bitNum&CHARSIZE can specify the 

#if FREE_SPACE_DEBUG
	printf("FREEING SPACE AT LOCATION %d . . .\n", entryLocation);
#endif
	int byteNum = 0;
	int bitNum = entryLocation;
	for(int blocksToDel = 1; blocksToDel <= numBlocks; blocksToDel++){
		byteNum = bitNum/CHARSIZE;
#if FREE_SPACE_DEBUG
		printf("byteNum: %d, bitNum: %d, negating bit: %x\n", byteNum, bitNum, (~(1 << (CHARSIZE - ((bitNum % CHARSIZE) + 1))) & 0xff) );
#endif
		freeSpace[byteNum] = freeSpace[byteNum] & (~(1 << (CHARSIZE - ((bitNum % CHARSIZE) + 1))));
		bitNum++;
	}
}
