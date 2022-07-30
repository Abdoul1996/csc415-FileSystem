/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Abdoulfatah Abdillahi, Brandon Butler, Jacob Cross, Kevin Manago
* Student IDs: 921623492, 920819091, 916168167, 917784912
* GitHub Name: Abdoul1996, BrandonButler08, JacobCross, Kmanago95
* Group Name: Team Shield
* Project: Basic File System
*
* File: fsFreeSpace.h
*
* Description: bitmap file system header
*
*
**************************************************************/

int initFreeSpace();
int increaseSpace();
int freeSpaceRequest(int numBlocks);
int freeSpaceSearch();
int releaseSpace(int entryLocation, int numBlocks);
