#ifndef COMMON_H
#define COMMON_H

#include "types.h" 

//Stucture declaration to store details of song file and temp file
typedef struct _SongInfo
{
    //Original Song  information
    char *track_name;
    FILE *fptr_song;
    char *title;
    char *artist;
    char *album;
    char *year;
    char *comment;

    //Temp file information
    FILE *fptr_temp;
    char *temp_file;

    //Edit details
    char *edit_type;
    char *edit_content;
} SongInfo;

//Coomon function prototypes
//Function to fetch the size
int get_size(FILE *fptr);

//Function to check header id and version
Status check_version(SongInfo *songInfo);

//Function to check operation type
OperationType check_operation_type(char *argv[]);

#endif
