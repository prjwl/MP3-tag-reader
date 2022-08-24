#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"
#include "common.h" 

//Function prototypes of display operation
//Function to validate arguments
Status read_and_validate_display_args(char *argv[], SongInfo *songInfo);

//Function to decode and display
Status decode_and_display(SongInfo * songInfo);

//Function to open files
Status open_files(SongInfo *songInfo);

//Function to read detail
Status read_details(SongInfo *songInfo);

//Function to fetch detail
Status fetch_detail(int size, FILE * fptr, char * data);

#endif
