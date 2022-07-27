/**************************************************************
* Class:  CSC-415-02 - Summer 2022
* Names: Abdoulfatah Abdillahi, Brandon Butler, Jacob Cross, Kevin Manago
* Student IDs: 921623492, 920819091, 916168167, 917784912
* GitHub Name: Abdoul1996, BrandonButler08, JacobCross, Kmanago95
* Group Name: Team Shield
* Project: Basic File System
*
* File: dir.c
*
* Description: Main driver for file system assignment.
*
* Make sure to set the #defined on the CMDxxxx_ON from 0 to 1 
* when you are ready to test that feature
*
**************************************************************/
#include "mfs.h"
#include "dir.h"

//mkdir(path)
//  - parse path
//      - if error -get out
//      - if last element index > 0 -get out
//      - loc = create dir(parent[0].location) refer to MS1
//      - index = get a free DirEntry(parent) needs a helper
//      - parent[index] all info yada yada
//      - writeDir(parent)
int fs_mkdir(const char *pathname, mode_t mode){
    // Parse Path
}

//setcwd (set current working directory)
//  - parse path
//      - if lastelement exist and is a dir continue else error
//      - cwdptr = loaddir(parent[i].location)
//      - char* cwdname = mallock()
int fs_setcwd(char *buf){
    //parse path
    char* cwdname = malloc();
}

//fs_delete(path)
//  - parse path
//      - if last element exists and is file deleteentry(parent, i)
//          - releases blob db is unused
int fs_delete(char* filename){

}

//rmdir
//  - parse path
//      - if last element is a dir
//          - load(parent)
//          - scan parent looks for . and ..
//      - if empty call deleteEntry look above
int fs_rmdir(const char *pathname){
    
}

//getcwd
//  - strcpy(buffer, cwdname)
char * fs_getcwd(char *buf, size_t size){
    strcpy(buf, cwdname);
    return buf;
}

//opendir(path)
//  - parse path
//  -
fdDir * fs_opendir(const char *name){
    int ret = b_open(name, 0);
    if (ret < 0)
}

//readdir(fdDir)
// for (iffddir->current; i < fddir=max;i++)
//  - if(ifdir->directory[i] is used)
//      - copy name of fddir;directory[i].name
//      - fd->item.name
//      - fd->item.type =f/d
struct fs_diriteminfo *fs_readdir(fdDir *dirp){

}

//closedir
int fs_closedir(fdDir *dirp){
    printf("Closing Directory...");
    free cwdname
    return 0;
}

// This is NOT the directory entry, it is JUST for readdir.
struct fs_diriteminfo
    {
    unsigned short d_reclen;    /* length of this record */
    unsigned char fileType;    
    char d_name[256]; 			/* filename max filename is 255 characters */
    };