/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Abdoulfatah Abdillahi, Brandon Butler, Jacob Cross, Kevin Manago
* Student IDs: 921623492, 920819091, 916168167, 917784912
* GitHub Name: Abdoul1996, BrandonButler08, JacobCross, Kmanago95
* Group Name: Team Shield
* Project: Basic File System
*
* File: parsePath.h
*
* Description: Header file containing struct and function to parse the path
* of a given input
*
**************************************************************/


#include <stdlib.h>
typedef struct parsedInfo{
    int isPathValid;                // 1 for valid / 0 for invalid
    int isFile;                     // 1 for file  / 0 for directory / -1 if invalid 
    int lastElementIndex;           // holds the index of the last element in the path
    char newEntryName[NAME_LIMIT];  // used to hold the name of the last element, optional
    directoryEntry* parent;         // directoryEntry of the parent
    // add more fields here
} parsedInfo;

// Parse path function
void parsePath(directoryEntry * cwd, directoryEntry* root, char* pathToParse, parsedInfo * info );
