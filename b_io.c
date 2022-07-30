/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Abdoulfatah Abdillahi, Brandon Butler, Jacob Cross, Kevin Manago
* Student IDs: 921623492, 920819091, 916168167, 917784912
* GitHub Name: Abdoul1996, BrandonButler08, JacobCross, Kmanago95
* Group Name: Team Shield
* Project: Basic File System
*
* File: b_io.c
*
* Description: Basic File System - Key File I/O Operations
*
**************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>			// for malloc
#include <string.h>			// for memcpy
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "b_io.h"
#include "mfs.h"

#define MAXFCBS 20
#define B_CHUNK_SIZE 512

typedef struct b_fcb
	{
	VCB * fi;
	int currLocation;
	int blockRemainder;
	int sizeRemainder;
	int bytesRead;
	/** TODO add al the information you need in the file control block **/
	char * buf;		//holds the open file buffer
	int index;		//holds the current position in the buffer
	int buflen;		//holds how many valid bytes are in the buffer
	} b_fcb;
	
b_fcb fcbArray[MAXFCBS];
char * bufferArray[MAXFCBS];

int startup = 0;	//Indicates that this has not been initialized

//Method to initialize our file system
void b_init ()
	{
	//init fcbArray to all free
	for (int i = 0; i < MAXFCBS; i++)
		{
		fcbArray[i].buf = NULL; //indicates a free fcbArray
		}
		
	startup = 1;
	}

//Method to get a free FCB element
b_io_fd b_getFCB ()
	{
	for (int i = 0; i < MAXFCBS; i++)
		{
		if (fcbArray[i].buf == NULL)
			{
			return i;		//Not thread safe (But do not worry about it for this assignment)
			}
		}
	return (-1);  //all in use
	}

// CreateFile()
//	- Find free DirectoryEntry
//	- ptr->DirectoryEntry
//	- DirectoryEntry (size, loc, etc) = 0
//	- return directoryEntry

// Interface to open a buffered file
// Modification of interface for this assignment, flags match the Linux flags for open
// O_RDONLY, O_WRONLY, or O_RDWR
b_io_fd b_open (char * filename, int flags)
	{
	b_io_fd returnFd;
	directoryEntry * fi;
	char * buf;
	// Parse path
	// if last element is not found Use flags to check O_WRONLY, O_RDONLY, O_RDWR, TRUNC, CRBATE
	
	// Check flags
	// Check Validity
	// Check existence
	// Create if needed
	//	- ptr DirectEntry = CreateFile(parent Dir)

	//*** TODO ***:  Modify to save or set any information needed
	//
	//
		
	if (startup == 0) b_init();  //Initialize our system
	fi = 

	
	returnFd = b_getFCB();				// get our own file descriptor
										// check for error - all used FCB's
	fcbArray[returnFd].buf = buf;

	
	return (returnFd);						// all set
	}


// Interface to seek function	
int b_seek (b_io_fd fd, off_t offset, int whence)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
		
		
	return (0); //Change this
	}



// Interface to write function	
int b_write (b_io_fd fd, char * buffer, int count)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
	
	
		
		
	return (0); //Change this
	}



// Interface to read a buffer

// Filling the callers request is broken into three parts
// Part 1 is what can be filled from the current buffer, which may or may not be enough
// Part 2 is after using what was left in our buffer there is still 1 or more block
//        size chunks needed to fill the callers request.  This represents the number of
//        bytes in multiples of the blocksize.
// Part 3 is a value less than blocksize which is what remains to copy to the callers buffer
//        after fulfilling part 1 and part 2.  This would always be filled from a refill 
//        of our buffer.
//  +-------------+------------------------------------------------+--------+
//  |             |                                                |        |
//  | filled from |  filled direct in multiples of the block size  | filled |
//  | existing    |                                                | from   |
//  | buffer      |                                                |refilled|
//  |             |                                                | buffer |
//  |             |                                                |        |
//  | Part1       |  Part 2                                        | Part3  |
//  +-------------+------------------------------------------------+--------+
int b_read (b_io_fd fd, char * buffer, int count)
	{

	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}

	if(fcbArray[fd].fi == NULL){
		return -1;
	}

	// Return value for function call, total bytes transferred
	int bytesRead = 0;

	// Offset used for when block is less than count input parameter, therefore needing to refill block
	// and buffer needs a specific offset to write to correct part
	int offset = 0;

	// Processing data while count is > 0 -> more data to be read
	while(count != 0){
		// used to track position of block by giving offset
		int blockRemainderOffset = B_CHUNK_SIZE;

		// Exit condition, after reaching EOF
		if(fcbArray[fd].sizeRemainder == 0){
			return 0;
		}

		// Last proocessing when EOF is reached before exit condition, have to return
		// the bytes read from the final call
		if(fcbArray[fd].sizeRemainder <= 0 && fcbArray[fd].blockRemainder <= 0){
			// Set to 0 for exit condition in subsequent calls
			fcbArray[fd].sizeRemainder = 0;
			break;
		}
		// empty block with data remaining in file to read, fill up bufferArray with block of data
		if( fcbArray[fd].blockRemainder == 0 ){
			LBAread(bufferArray[fd], 1, fcbArray[fd].currLocation++); // incrementing currLocation for next LBAread call
			fcbArray[fd].sizeRemainder-=B_CHUNK_SIZE; 		  // tracking remaining size of file after LBAread, <=0 when empty
			// blockRemainder condition below to account for EOF: ex. if sizeRemainder above is 27:
			// 27 - 512 = -485. -----> blockRemainder = -485 + 512 = 27
			// when not in EOF, blockRemainder is B_CHUNK_SIZE
			fcbArray[fd].blockRemainder = (fcbArray[fd].sizeRemainder >= 0) ? B_CHUNK_SIZE : fcbArray[fd].sizeRemainder + B_CHUNK_SIZE;
			// offset used to track current location in block for duration of function call
			blockRemainderOffset = fcbArray[fd].blockRemainder;
		}
		// processing portion of the function call
		// checks if blockRemainder >= count 
		// if true,  condition (1) full count can be transferred and exit while loop
		// if false, condition (2) remainder of block is transferred, and block will be refreshed with LBAread until condition is true
		if(fcbArray[fd].blockRemainder >= count){ // condition (1)
			// memcpy used to transfer data from my buffer to caller's buffer with their offsets
			// transferring over count bytes due to condition (1)
                	memcpy(buffer + offset, bufferArray[fd] + (blockRemainderOffset - fcbArray[fd].blockRemainder), count);
			fcbArray[fd].blockRemainder-=count;	// reducing blockRemainder since count data has been transferred
			bytesRead+=count;			// increasing bytesRead    since count data has been transferred	
			fcbArray[fd].bytesRead+=count;		// increasing accumulated bytesRead for debugging
			count = 0;				// setting count to 0 since all requested data is transferred
		} else { //condition (2)
			// transferring data from buffer to caller's buffer with offsets
			// transferring over by blockRemainder because of condition (2)
			memcpy(buffer + offset, bufferArray[fd] + (blockRemainderOffset - fcbArray[fd].blockRemainder), fcbArray[fd].blockRemainder);
			count-=fcbArray[fd].blockRemainder; 	// reducing count by blockRemainder to keep track of data transferred
			offset+=fcbArray[fd].blockRemainder;	// increasing offset for buffer to keep track of current location in buffer
			bytesRead+=fcbArray[fd].blockRemainder;	// increasing bytesRead since data was transferred to caller
			fcbArray[fd].bytesRead+=fcbArray[fd].blockRemainder; // debugging
			fcbArray[fd].blockRemainder = 0;	// setting to 0 to be in condition with LBAread since count is still >0
		}
	}
	return bytesRead;	//Total transferred bytes
	}
	
// Interface to Close the file	
int b_close (b_io_fd fd)
	{
		printf("Closing...\n");
		b_fcb* fcb = &fcbArray[fd];

		// Whatever function is opened in b_open
		//close(fcb->);
	}
