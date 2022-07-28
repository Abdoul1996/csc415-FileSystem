//insert header here


#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fsLow.h"
#include "fsFreeSpace.c"
#include "fsFunctions.h"
#include "parsePath.c"



int fs_isFile(directoryEntry * entry){
	return entry->isFile;
}

int fs_isDir(directoryEntry * entry){
	return !(entry->isFile);
}


int fs_mkdir(char* path){
	
	parsePathStruct * pp = malloc(sizeof(parsePathStruct));
	parsePath(path, pp);
	// TODO: check parsePath for error
	if(pp->lastElementIndex >= 0)
		return (-2);

	// TODO: adapt initRootDirectory to createDir function
}

