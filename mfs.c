//insert header here

#include "mfs.h"
#include "parsePath.c"
#include "fsInit.c"
// Key directory functions
//int fs_mkdir(const char *pathname, mode_t mode);

// Key directory functions
//
// void parsePath(directoryEntry* cwd, directoryEntry* root, char* pathToParse, parseInfo* info)
int fs_mkdir(const char *pathname, mode_t mode){
	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, pathname);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(cwd, root, pathToParse, info);
	// TODO: check return value for error
	if(info->isPathValid == 0)
		return (-2);
	if(info->lastElementIndex >= 0)
		return (-2);
	directoryEntry* newDir = createDir(info->newEntryName, info->isFile, info->parent);
	
	return newDir;
}

int fs_rmdir(const char *pathname){
}

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

		parsedInfo->d_reclen = loadDir(info->lastElementIndex);
		parsedInfo->directoryStartLocation = 0;
		parsedInfo->dirEntryPosition = info->lastElementIndex.size / sizeof(directoryEntry);
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
	free(info);
	info = NULL;

	return 0;
}

// Misc directory functions
char * fs_getcwd(char *buf, size_t size){
	if((buf != NULL) &&& strlen(cwd) < size){
		strcpy(buf, cwd);
	}
	return buf;
}

int fs_setcwd(char *buf){
}	//linux chdir

int fs_isFile(char * path){
}     //return 1 if file, 0 otherwise

int fs_isDir(char * path){
}             //return 1 if directory, 0 otherwise
int fs_delete(char* filename){
}//removes a file


