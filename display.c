#include <stdio.h>
#include <string.h>
#include "common.h"
#include "display.h"
#include "types.h"
#include <stdlib.h>

//Function to validate arguments
Status read_and_validate_display_args(char *argv[], SongInfo *songInfo)
{
    //To check source mp3 file
    if (argv[2] != NULL)
    {
	if (strstr(argv[2], ".mp3"))
	{
	    songInfo->track_name = argv[2];
	    return success;
	}
	else
	{
	    printf("ERROR : Entered source file is not having .mp3 extension.\n");
	    return failure;
	}
    }
    else
    {
	printf("ERROR : Source file is not passed.\n");
	return failure;
    }
}
    
//Function to Perform decode and display
Status decode_and_display(SongInfo * songInfo)
{
    //Calling function to open files
    if (open_files(songInfo) == success)
    {
	//Calling function to check header id and version
	if (check_version(songInfo) == success)
	{
	    //Calling function to read and store tags
	    if (read_details(songInfo) == success)
	    {
		printf("\nMp3 Tag Reader & Editor :\n");
		printf("-------------------------\n");
		printf("Version ID\t: 2.3\nTitle\t\t: %s\nArtist\t\t: %s\nAlbum\t\t: %s\nYear\t\t: %s\nComment\t\t: %s\n", songInfo->title, songInfo->artist, songInfo->album, songInfo->year, songInfo->comment);
		return success;
	    }
	    else
	    {
		printf("ERROR : Reading details failed.\n");
		return failure;
	    }
	}
	else
	{
	    printf("ERROR :Entered file is not ID3 v3\n");
	    return failure;
	}
    }
    else
    {
	printf("ERROR : Opening files failed.\n");
	return failure;
    }
}

// Function to open the files.
Status open_files(SongInfo *songInfo)
{
    // Opening source mp3 file in read mode
    songInfo->fptr_song = fopen(songInfo->track_name, "r");
    
    // Do Error handling
    if (songInfo->fptr_song == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", songInfo->track_name);
	return failure;
    }
    // No failure return e_success
    return success;
}

//Function to read the details
Status read_details(SongInfo *songInfo)
{
    //Skipping first 10bytes
    fseek(songInfo->fptr_song, 10L, SEEK_SET);

    //Local variable declaration
    int count = 0;

    //loop to read all 5 datas
    while (count < 5)
    {
	//Local variable declaration
	uint i, size = 0;
	char array[4] = {0}, ch;

	//Reading the frame ID
	for (i = 0; i < 4; i++)
	{
	    fread(&ch, sizeof(char), 1, songInfo->fptr_song);
	    array[i] = ch;
	}
	
	//Condition to compare frame ID
	//For TIT2 == title
	if (strcmp("TIT2", array) == 0)
	{
	    //Calculating size of the title
	    size = get_size(songInfo->fptr_song);
	    //Skipping flag and null
	    fseek(songInfo->fptr_song, 3L, SEEK_CUR);
	    //Dynamic memory allocation
	    songInfo->title = (char *)malloc(size * sizeof(char));
	    //Reading and storing the title
	    fetch_detail(size, songInfo->fptr_song, songInfo->title);
	    count++;
	}

	//For TPE1 == artist
	else if (strcmp("TPE1", array) == 0)
	{
	    //Calculating size of artist name
	    size = get_size(songInfo->fptr_song);
	    //Skipping flag and null
	    fseek(songInfo->fptr_song, 3L, SEEK_CUR);
	    //Dynamic memory allocation
	    songInfo->artist = (char *)malloc(size * sizeof(char));
	    //Reading and storing artist name
	    fetch_detail(size, songInfo->fptr_song, songInfo->artist);
	    count++;
	}
	
	//For TALB == album
	else if (strcmp("TALB", array) == 0) 
	{
	    //Calculating size of album name
	    size = get_size(songInfo->fptr_song);
	    //Skipping flag and null
	    fseek(songInfo->fptr_song, 3L, SEEK_CUR);
	    //Dynamic memory allocation
	    songInfo->album = (char *)malloc(size * sizeof(char));
	    //Reading and storing album name
	    fetch_detail(size, songInfo->fptr_song, songInfo->album);
	    count++;
	}

	//For Tyear == year
	else if (strcmp("TYER", array) == 0)
	{
	    //Calculating year size
	    size = get_size(songInfo->fptr_song);
	    //Skipping flag and null
	    fseek(songInfo->fptr_song, 3L, SEEK_CUR);
	    //Dynamic memory allocation
	    songInfo->year = (char *)malloc(size * sizeof(char));
	    //Reading and storing year
	    fetch_detail(size, songInfo->fptr_song, songInfo->year);
	    count++;
	}

	//For COMM == comments
	else if (strcmp("COMM", array) == 0)
	{
	    //Getting size of comments
	    size = get_size(songInfo->fptr_song);
	    //Skipping flag and null
	    fseek(songInfo->fptr_song, 3L, SEEK_CUR);
	    //Dynamic memory allocation
	    songInfo->comment = (char *)malloc(size * sizeof(char));
	    //Reading and storing comment
	    fetch_detail(size, songInfo->fptr_song, songInfo->comment);
	    count++;
	}

	//If frame id is other type
	else
	{
	    //Getting the size of data
	    size = get_size(songInfo->fptr_song);
	    //Skipping data and flag
	    fseek(songInfo->fptr_song, (size + 2), SEEK_CUR);
	}
    }
    return success;
}

//Function to fetch the data
Status fetch_detail(int size, FILE * fptr, char * data)
{
    //Local variables declaration
    int i;
    char ch;
    
    //Loop to read and store data except null
    for (i = 0; i < size - 1; i ++)
    {
	fread(&ch, 1, 1, fptr);
	data[i] = ch;
    }
    data[i] = '\0';
    return success;
}

