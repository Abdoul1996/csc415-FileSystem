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
	parsePath(cwd, root, );
}

// Closes the directory
int fs_closedir(fdDir *dirp){
	printf("Closing Directory...");
	
	// Free and NULL all mallocs
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


//setcwd (set current working directory)
//  - parse path
//      - if lastelement exist and is a dir continue else error
//      - cwdptr = loaddir(parent[i].location)
//      - char* cwdname = mallock()

int fs_setcwd(char *buf){
	char pathToParse[NAME_LIMIT];
	strcpy(pathToParse, pathname);
	parsedInfo* info = malloc(sizeof(parsedInfo));
	parsePath(cwd, root, pathToParse, info);

	directoryEntry* 

	dir
	if(info->lastELementIndex == 1) && 
	
	



	
	
	

}	//linux chdir

int fs_isFile(char * path){
}     //return 1 if file, 0 otherwise

int fs_isDir(char * path){
}             //return 1 if directory, 0 otherwise
int fs_delete(char* filename){
}//removes a file


