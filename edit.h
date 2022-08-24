#ifndef EDIT_H
#define EDIT_H

#include "types.h"
#include "common.h" 

//Function prototypes for edit

//Function to read and validate edit arguments
Status read_and_validate_edit_args(char *argv[], SongInfo *songInfo);

//Function to edit details
Status edit_detail(char * edit_type, SongInfo *songInfo);

//Function to open files
Status open_edit_files(SongInfo *songInfo);

//Function to reopen files
Status reopening_edit_files(SongInfo *songInfo);

#endif
