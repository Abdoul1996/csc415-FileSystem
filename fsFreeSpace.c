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
#include <stdio.h>
#include <stdint.h>
#include <mfs.h>

#define CHARSIZE 8
#define TOTAL_FREE_SPACE 2560

int freeSpaceLocation = 0;

// 19,531 bits for that many blocks
// or 2,442 bytes -> 5 blocks*size = 2560

unsigned char freeSpace[TOTAL_FREE_SPACE];

int initFreeSpace(){
	for(int i = 0; i < TOTAL_FREE_SPACE; i++)
		freeSpace[i] = 0;

	freeSpace[0] = freeSpace[0] | (1 << (CHARSIZE - 0 + 1)); //VCB
	freeSpaceLocation++;
	for(int i = 1; i <= 5; i++){
		int block = i/CHARSIZE;
		freeSpace[block] = freeSpace[block] | (1 << (CHARSIZE - i%8 + 1));
		freeSpaceLocation++;
	}

	LBAwrite(freeSpace, 5, 1);

	return freeSpaceLocation; // VCB location
}

int freeSpaceRequest(int numBlocks){
	//return starting position of the blocks
	int startingPosition = freeSpaceLocation;
	
	int fieldNum = 0;
	for(int i = 1; i <= numBlocks; i++){
		fieldNum = freeSpaceLocation/CHARSIZE;
		freeSpace[fieldNum] = freeSpace[fieldNum] | ( 1 << (CHARSIZE - freeSpaceLocation % 8 + 1));
		freeSpaceLocation++;
	}

	return startingPosition;

}

