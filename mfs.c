/**************************************************************
* Class:  CSC-415-02 Fall 2021
* Names: Abdoulfatah Abdillahi, Brandon Butler, Jacob Cross, Kevin Manago
* Student IDs: 921623492, 920819091, 916168167, 917784912
* GitHub Name: Abdoul1996, BrandonButler08, JacobCross, Kmanago95
* Group Name: Team Shield
* Project: Basic File System
*
* File: mfs.c
*
* Description: Basic File System - Key File I/O Operations
*
**************************************************************/

#include "mfs.h"
#include "parsePath.c"
#include "fsFreeSpace.c"
// Key directory functions
//
// void parsePath(directoryEntry* cwd, directoryEntry* root, char* pathToParse, parseInfo* info)

// TODO: Figure out where cwd and root come from, will have to be changed later

int fs_mkdir(const char *pathname, mode_t mode){
	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, pathname);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(cwd, root, pathToParse, info);
	if(info->isPathValid == 0)
		return (-2);
	if(info->lastElementIndex >= 0)
		return (-2);
	directoryEntry* newDir = createDir(info->newEntryName, 0, info->parent);
	
	printf("Created dir: %s\n", newDir->name);	
	// TODO: free info
	// TODO: Add WriteDir part	
	writeDir(newDir);	

	return 0;
}

int fs_rmdir(const char *pathname){
	// /foo/bar/this/that means:
	// from parsePath, given info struct which contains parent
	// parent->entries[lastElementIndex] = the child, in this case,
	// parent = this, and parent->entries[lastElementIndex] = that
	// goal: remove "that" -> need to write remove directory

	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, pathname);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(cwd, root, pathToParse, info);
	printf("IN RMDIR . . .\n lastElementindex: %d, isFile: %d\n", info->lastElementIndex, info->parent->entries[info->lastElementIndex]->isFile);	
	if(info->lastElementIndex < 0)
		return (-2);	// if last element doesn't exist, error
	if(info->parent->entries[info->lastElementIndex]->isFile)
		return (-2);	// if last element is not a directory (0 = dir, 1 = file), error

	directoryEntry* dirToDelete = info->parent->entries[info->lastElementIndex];
	// check if dirToDelete->entries is empty (only . and ..)
	int itr = 1;
	while(dirToDelete->entries[++itr] != NULL) // if dir is not empty, return error
		return (-2);
	return deleteEntry(info->parent, info->lastElementIndex); // 0 = success


}

int fs_delete(char* filename){

 	char pathToParse[NAME_LIMIT];
        strcpy(pathToParse, filename);
        parsedInfo* info = malloc(sizeof(parsedInfo));
        parsePath(cwd, root, pathToParse, info);

        if(info->lastElementIndex < 0)
                return (-2);    // if last element doesn't exist, error
        if(!info->parent->entries[info->lastElementIndex]->isFile)
                return (-2);    // if last element is not a directory (0 = dir, 1 = file), error
	return deleteEntry(info->parent, info->lastElementIndex);

}//removes a file


// Directory iteration functions
fdDir * fs_opendir(const char *name){
	// Parsing process
	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, name);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(cwd, root, pathToParse, info);

	// Checking if the last element exists and is a directory.
	if(info->lastElementIndex > 0){
		fdDir* parsedInfo = malloc(sizeof(fdDir));

		//TODO : still need to write loadDir
		parsedInfo->directory = loadDir(info->parent->entries[info->lastElementIndex]);
		parsedInfo->d_reclen = info->parent->size;
		parsedInfo->directoryStartLocation = info->parent->location;
		parsedInfo->dirEntryPosition = info->lastElementIndex;
		return parsedInfo;
	} else {
		printf("Error opening Directory! Check line 31 or mfs.c");
		return -1;
	}	
}

// Closes the directory
int fs_closedir(fdDir *dirp){
	printf("Closing Directory...");
	
	// Free and NULL all mallocs
		// info is from fs_mkdir
	//free(info);
	//info = NULL;

	return 0;
}


// Misc directory functions
char * fs_getcwd(char *buf, size_t size){
	// TODO: Put loop for errors
	
	// Copies the current working directory into the buffer
	strcpy(buf, cwd->name);
	// Returns the now copied buffer.
	return buf;
}

int fs_setcwd(char *buf){
	// Parse Path
	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, buf);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(cwd, root, pathToParse, info);

	// Checking to see if the last element exists and is an element. 
	if(info->lastElementIndex > 0){
		// Gets the location of the parent
		directoryEntry * cwdPtr = loadDir(info->parent->entries[info->lastElementIndex]);
		// Allocates the cwd to the length of the buffer.
		char * localCwd = malloc(strlen(buf));
		// Copies the cwd with the buffer.
		strcpy(localCwd, buf);
	} else {
		// Error message
		printf("Error in setcwd. Check line 128 of mfs.c");
		return -1;
	}
}

//linux chdir
int fs_isFile(char * path){
	//call parsepath, return the isFile field
	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, path);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(cwd, root, pathToParse, info);

	return info->isFile;

}     //return 1 if file, 0 otherwise

int fs_isDir(char * path){
	//call isFile, return opposite.
	return !fs_isFile(path);

}             //return 1 if directory, 0 otherwise
struct fs_diriteminfo *fs_readdir(fdDir *dirp){
	// idea here is to fill a dE struct with the data from disk that is
	// being referred to in the args to this fn. we want to fill this de
	// struct so we can access the data from the dir. the arg. asks for.
	fs_diriteminfo* newInfo = malloc(sizeof(fs_diriteminfo));

	// printf("reclen= %hu\n", dirp->d_reclen);
	// printf("dEP= %hu\n", dirp->dirEntryPosition);
	// printf("dSL=%ld \n",dirp->directoryStartLocation);

	// malloc buffer for LBAread
	directoryEntry* dirBuf = malloc(sizeof(directoryEntry));

	//read the directory from the volume/disk
	LBAread(dirBuf, dirp->d_reclen, dirp->directoryStartLocation);
	// printf("readdir fetched: %s \n", dirBuf->name);

	
	//increment current dirEntry pos
	dirp->dirEntryPosition += 1;

	// possible the left-hand side needs "newInfo->___" instead of "newInfo.___"
	newInfo->d_reclen = dirp->d_reclen;
	newInfo->fileType = dirBuf->entries[dirp->dirEntryPosition]->isFile;
	//newInfo->d_name = dirBuf->entries[dirp->dirEntryPosition]->name;
	strcpy(newInfo->d_name, dirBuf->entries[dirp->dirEntryPosition]->name);

	return newInfo;
}


// TODO: error checks
int fs_stat(const char *path, struct fs_stat *buf){
	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, path);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(cwd, root, pathToParse, info);
	
	directoryEntry* toInsert = info->parent->entries[info->lastElementIndex];
	buf->st_size = toInsert->size;
	buf->st_blksize = VCBPtr->blockSize;
	buf->st_blocks = DIR_ENTRY_BLOCKS; // does this have to be calculated??
	buf->st_accesstime = toInsert->time;
	buf->st_modtime = toInsert->time;
	buf->st_createtime = toInsert->time;

	return 0; 

}

directoryEntry * createDir(char* name, int isFile, directoryEntry* parent){
	int index = -1;


	// Create a directoryEntry in the parent's entries
	// Checking if there is any space
	for(int i = 2; i < NUM_ENTRIES; i++){
		if(parent->entries[i] == NULL){
			index = i;
			break;
		}
	}

	if(index == -1)
		return NULL;
	
	// allocating memory for the new directory to be added
	directoryEntry* currentDir = malloc(sizeof(directoryEntry));
	int freeSpaceBlock = freeSpaceRequest(DIR_ENTRY_BLOCKS);
	time_t rawTime;
	strcpy(currentDir->name, name);
	currentDir->size = ENTRY_MEMORY;
	currentDir->location = freeSpaceBlock;
	currentDir->time = time( &rawTime );
	currentDir->isFile = isFile;
	
	// allocating memory for the entries of new directory to be added
	currentDir->entries = malloc(sizeof(directoryEntry*) * NUM_ENTRIES);

	// free state
	for(int i = 0; i < NUM_ENTRIES; i++){
		currentDir->entries[i] = NULL;
	}
	// initializing . (self) and .. (parent) directories
	currentDir->entries[0] = malloc(sizeof(directoryEntry));
	strcpy(currentDir->entries[0]->name, ".");
	currentDir->entries[0]->size = currentDir->size;
	currentDir->entries[0]->location = freeSpaceBlock;
	currentDir->entries[0]->time = currentDir->time;
	currentDir->entries[0]->isFile = currentDir->isFile;
	
	currentDir->entries[1] = malloc(sizeof(directoryEntry));
	strcpy(currentDir->entries[1]->name, "..");
	currentDir->entries[1]->size = parent->size;
	currentDir->entries[1]->location = parent->location;
	currentDir->entries[1]->time = parent->time;
	currentDir->entries[1]->isFile = parent->isFile;
	currentDir->entries[1]->entries = parent->entries;

	// setting . to go back to self
	currentDir->entries[0]->entries = currentDir->entries;
	// setting the new directory to the index of parent 
	parent->entries[index] = currentDir;
	
	writeDir(currentDir);	
	// return directoryEntry memory location of the child 
	return currentDir;
}

// LBAxxx( buffer, lbaCount, lbaPosition) lbaCount -> block #, lbaPosition -> block location
int writeDir(directoryEntry* entry){ // LBAwrite, returns location 
	return LBAwrite(entry, DIR_ENTRY_BLOCKS, entry->location); // TODO: error check

}

//LBAread reads location data into the given buffer
directoryEntry* loadDir(directoryEntry* parent){ // LBAread
	directoryEntry* buffer = malloc(sizeof(directoryEntry));
	LBAread(buffer, DIR_ENTRY_BLOCKS, parent->location);	// TODO: error check
	return buffer;
}

// TODO: Free Space allocator
int deleteEntry(directoryEntry* parent, int elementIndex){

	// ex. /foo/bar/this/that
	// remove "that"
	// given parent "this" and index of this->entries[index] = that, 
	// in "this", set this->entries[index] = NULL and free this-entries[index] 
	if(parent == NULL)		// invalid parent
		return (-2);
	if(elementIndex < 0)	// Invalid index (entry doesn't exist)
		return (-2);
	parent->entries[elementIndex] = NULL;
	free(parent->entries[elementIndex]);
	return 0; // return 0 = success
}	

