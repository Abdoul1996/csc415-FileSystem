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

#define CHAR 8
#define TOTAL_FREE_SPACE 2560



// 19,531 bits for that many blocks
// or 2,442 bytes -> 5 blocks*size = 2560

unsigned char freeSpace[TOTAL_FREE_SPACE];

void initFreeSpace(){
	for(int i = 0; i < TOTAL_FREE_SPACE)
		freeSpace[i] = 0;

	freeSpace[0] = freeSpace[0] | (1 << ());
}
