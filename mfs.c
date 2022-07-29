//insert header here

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
	directoryEntry* newDir = createDir(info->newEntryName, info->isFile, info->parent);
	
	// TODO: free info
	// TODO: Add WriteDir part	
	
	return newDir;
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
	
	if(info->lastElementIndex < 0)
		return (-2);	// if last element doesn't exist, error
	if(!info->parent->entries[info->lastElementIndex]->isFile)
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
		parsedInfo->d_reclen = loadDir(info->lastElementIndex);
		parsedInfo->directoryStartLocation = 0;
		parsedInfo->dirEntryPosition = info->parent->entries[info->lastElementIndex]->size / sizeof(directoryEntry);
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
	char 
	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, buf);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(buf, root, pathToParse, info);

	fs_opendir(pathToParse)

	if(info->lastElement ==1){
		fdDir* ret = malloc(sizeof(fdDir));
		
	}



	/*if((buf != NULL) &&& strlen(cwd) < size){
		strcpy(buf, cwd);
	}
	return buf;*/
}



//setcwd (set current working directory)
//  - parse path
//      - if lastelement exist and is a dir continue else error
//      - cwdptr = loaddir(parent[i].location)
//      - char* cwdname = mallock()

int fs_setcwd(char *buf){
	char * cwdPtr;

	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, buf);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(buf, root, pathToParse, info);



	if(info->lastELementIndex == 1){
			directoryEntry* cwdptr = info->parent
			char *cwdname = malloc();
			strcpy(cwdname.path);
	}else
	{
		fs_delete(path);
		parsePath(buf, root, pathToparse, info);

		if(info->lastELementIndex == 1 && info->isFile ==1){
			fs_delete(info->parent)

		}
	}

}





	
	
	

	//linux chdir

int fs_isFile(char * path){
	//call parsepath, return the isFile field
	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, path);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(cwd, root, pathToParse, info);

	printf("isFile returned: %d \n", info->isFile);

	return info->isFile;

}     //return 1 if file, 0 otherwise

int fs_isDir(char * path){
	//call isFile, return opposite.
	return !fs_isFile(path);

}             //return 1 if directory, 0 otherwise
struct fs_diriteminfo *fs_readdir(fdDir *dirp){
	//takes fddir ptr
	//print values from dirp
	printf("reclen= %hu\n", dirp->d_reclen);
	printf("dEP= %hu\n", dirp->dirEntryPosition);
	printf("dSL=%ld \n",dirp->directoryStartLocation);

	// det. how to translate the values into the output format

	// take those steps

	//returns firiteminfo struct.

	
}



int fs_stat(const char *path, struct fs_stat *buf){
	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, path);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(cwd, root, pathToParse, info);
	
	directoryEntry* toInsert = info->parent->entries[info->lastElementIndex];
	DIR_ENTRY_BLOCKS;
	ENTRY_MEMORY;
	buf->st_size =
	buf->st_blksize
	buf->st_blocks
	buf->st_accesstime
	buf->st_modtime
	buf->st_createtime

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
	
	// return directoryEntry memory location of the child 
	return currentDir;
}

// LBAxxx( buffer, lbaCount, lbaPosition) lbaCount -> block #, lbaPosition -> block location
int writeDir(directoryEntry* entry){ // LBAwrite, returns location 
	int lbaCount = (entry->size + VCBPtr->blockSize - 1) / VCBPtr->blockSize;
	return LBAwrite(entry, lbaCount, entry->location);

}

int loadDir(directoryEntry* entry){ // LBAread
	int lbaCount = (entry->size + VCBPtr->blockSize - 1 ) / VCBPtr->blockSize;
	return LBAread(entry, lbaCount, entry->location);
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

