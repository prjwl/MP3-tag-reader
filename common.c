#include <stdio.h>
#include <string.h>
#include "common.h"

//Function to check type of operation
OperationType check_operation_type(char *argv[])
{
    //Comparing argument with strings
    if (strcmp(argv[1], "-h") == 0)
    {
	return help;
    }
    else if (strcmp(argv[1], "-v") == 0)
    {
	return view;
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
	return edit;
    }
    else
    {
	return unsupported;
    }
}

//Function to check the version
Status check_version(SongInfo *songInfo)
{
    int i, arr[2] = {3, 0};
    char ch;
    char * data = "ID3";

    //To check ID3
    for (i = 0; i < 3; i++)
    {
	fread(&ch, sizeof(char), 1, songInfo->fptr_song);
	if (ch != data[i])
	{
	    return failure;
	}
    }

    //To check version
    for (i = 0; i < 2; i++)
    {
	fread(&ch, sizeof(char), 1, songInfo->fptr_song);
	if (((int)ch) != (arr[i]))
	{
	    return failure;
	}
    }
    return success;
}


//Function to calculate size
int get_size(FILE *fptr)
{
    //Local variable declaration
    int i, size = 0;
    char ch;

    //Loop to calculate size
    for (i = 0; i < 4; i++)
    {
	uint mask = 0xFF;
	fread(&ch, sizeof(char), 1, fptr);
	mask = mask & (int)ch;
	size = size | mask;
	if (i < 3)
	{
	    size <<= 8;
	}
    }
    return size;
}

