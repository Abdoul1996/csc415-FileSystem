typedef struct parsedInfo{
    int isValid;  // 1 for valid / 0 for invalid
    int isFile;      // 1 for file  / 0 for directory / -1 if invalid ?
    int lastElementIndex;
    // add more fields here
} parsedInfo;


void parsePath(directoryEntry * entry, parsedInfo * info);

