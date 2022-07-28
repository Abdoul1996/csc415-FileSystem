#include "mfs.h"
//#include "fsInit.c"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct parsePathStruct{
    int isValid;  // 1 for valid / 0 for invalid
    int isFile;      // 1 for file  / 0 for directory / -1 if invalid ?
    // add more fields here
};

//typedef struct parsePath parsePathStruct;

void parsePath(directoryEntry * entries[], struct parsePathStruct * pps);


// to call this fucntion you must pass an empty parsePathStruct
// which this function will fill based upon if the pathname is 
// valid, if path ref's file or directory, etc...

//void parsePath(char * path, parsePathStruct * parsePathStruct){
void parsePath(directoryEntry * entries[], struct parsePathStruct * pps){
    // use the path arg to lookup each part in entries[i]
    // started in fsInit.c

    // examples tested
    //char * path = "/home/var/this/that";
    char path[20] = "home/var/this/that";

    // grab first char value
    //char first = path[0];
    //printf("%c \n", first);

    char *cmp1;
    char *cmp2;

    const char slash = '/';

    //strcpy(cmp1,path[0]);
    //memcpy(cmp1, path[0], 1);


    //strcpy(cmp2,slash);

    // if first == "/" -> load root directory for searching
    // else -> load current working directory for seraching

    // root case - loading root dir.
    printf("name: %s \n", entries[0]->name);
    printf("size: %d \n", entries[0]->size);
    printf("location: %d \n", entries[0]->location);

    //load the first token
    // Returns first token after the slash, before the next.
    char * token = strtok(path, "/");
    printf("first: %s \n", token);

    //char * entryName;
    int isFound = 0;
    int p = 0;
   
    // if first == "/" -> load root directory for searching
    // else -> load current working directory for seraching
    // if(strcmp(path[0],"/")==0){
    if(strcmp(cmp1, cmp2)==0){
        printf("/ matches... \n");
        while (token != NULL) {
            isFound = 0;
            // search through the values of entries[] looking for first token
            // loop over entries with first token
            //for(int p=0;p<NUM_ENTRIES;p++){
            p = 0;
            while(entries[p]!=NULL){
                printf("looping, p=%d \n", p);
                //entryName = entries[p]->name;
                if(strcmp(entries[p]->name,token)==0){
                    isFound = 1;

                    // this token of path is valid, keep going.
                    // printf("error: pathname not valid. \n");
                    // pps->isValid = 0;
                    // return;
                }
                p++;
            }

            //check if it was found
            if(!isFound){
                printf("error: pathname not valid. \n");
                //pps->isValid = 0;
                return;
            }
            // otherwise, if isFound after the loop, continue.

            // load the next token after first run
            token = strtok(NULL, "/");
            printf("token: %s \n", token);
        }
        // if we made it to here, isFound=1 on each loop,
        // so the path is valid.
        //pps->isValid = 1;
        

    }
 
    // Keep printing tokens while one of the
    // delimiters present in str[].


    //if it is "/" -> load root directory, search for next
    // value in the path

    // Returns first token
    //char * firstToken = strtok(str, "");

    // this works to print the root dir first entry.
    printf("name: %s \n", entries[0]->name);
    printf("size: %d \n", entries[0]->size);
    printf("location: %d \n", entries[0]->location);

    // we then need to create a loop to go over all the entries in the root dir,
    // checking for the for the first token
    



    // fill the fields of the the passed struct

    //return parsePathStruct;
}
