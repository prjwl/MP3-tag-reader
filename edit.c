#include <stdio.h>
#include <string.h>
#include "common.h"
#include "edit.h"
#include "types.h"
#include <stdlib.h>

//Function to validate arguments
Status read_and_validate_edit_args(char *argv[], SongInfo *songInfo)
{
    //To check source image file
    if (argv[2] != NULL)
    {
	if (strstr(argv[2], ".mp3"))
	{
	    songInfo->track_name = argv[2];
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

    //Checking edit type
    if (argv[3] != NULL)
    {
	songInfo->edit_type = argv[3];
    }
    else
    {
	printf("ERROR : Edit type is not passed\n ");
	return failure;
    }

    //Checking editing content
    if (argv[4] != NULL)
    {
	songInfo->edit_content = argv[4];
    }
    else
    {
	printf("ERROR : Content to be edited is not passed\n");
	return failure;
    }
    return success;
}
    
//Function for editing detail
Status edit_detail(char * edit_type, SongInfo *songInfo)
{
    //Local variables
    char ch;
    int i, flag = 0, size = 0, len = 0;

    //Opening original file in read and temp in write mode
    if (open_edit_files(songInfo) == success)
    {
	//Checking header id and version
	if (check_version(songInfo) == success)
	{
	    //Bringing pointer to start
	    rewind(songInfo->fptr_song);

	    //Copying first 10 bytes
	    for (i = 0; i < 10; i++)
	    {
		fread (&ch, sizeof(char), 1, songInfo->fptr_song);
		fwrite (&ch, sizeof(char), 1, songInfo->fptr_temp);
	    }

	    //Checking each frame id until it is not found
	    while (flag == 0)
	    {
		//Local variable
		int size = 0, len = 0;
		char array[4] = {0};

		//Getting frame id
		for (i = 0; i < 4; i++)
		{
		    fread(&ch, sizeof(char), 1, songInfo->fptr_song);
		    fwrite (&ch, sizeof(char), 1, songInfo->fptr_temp);
		    array[i] = ch;
		}

		//Comparing frame id
		if (strcmp(edit_type, array) == 0)
		{
		    //Calculating the size of the new content
		    size = (strlen(songInfo->edit_content) + 1);

		    //Calculating the size of data in original file
		    len = get_size(songInfo->fptr_song);

		    //writing the size of new content in the temp
		    char * cptr = (char *) &size;
		    for (i = 3; i >= 0; i--)
		    {
			ch = cptr[i];
			fwrite(&ch, sizeof(char), 1, songInfo->fptr_temp);
		    }

		    //Copying flag and null to temp
		    for (i = 0; i < 3; i++)
		    {
			fread (&ch, sizeof(char), 1, songInfo->fptr_song);
			fwrite (&ch, sizeof(char), 1, songInfo->fptr_temp);
		    }

		    //Writing the new data to temp
		    for (i = 0; i < (size - 1); i++)
		    {
			ch = songInfo->edit_content[i];
			fwrite(&ch, sizeof(char), 1, songInfo->fptr_temp);
		    }

		    //Skipping old data in song file
		    fseek (songInfo->fptr_song, len - 1 , SEEK_CUR);

		    //flag to indicate once editing is done
		    flag = 1;
		}

		//If frame id don't match
		else
		{
		    //Getting size of content
		    size = get_size(songInfo->fptr_song);

		    //Moving pointer 4 bytes back since it moved 4 byte during size calculation
		    fseek(songInfo->fptr_song, -4, SEEK_CUR);

		    //Copying size = 4byte, flag = 2 byte and content size bytes
		    for (i = 0; i < size + 6; i++)
		    {
			fread(&ch, 1, 1, songInfo->fptr_song);
			fwrite(&ch, 1, 1, songInfo->fptr_temp);
		    }
		}
	    }

	    //Loop to copy remaining bytes after editing
	    while (fread(&ch , sizeof(char), sizeof(char) , songInfo->fptr_song) > 0)
	    {
		fwrite(&ch , sizeof(char), sizeof(char) , songInfo->fptr_temp);
	    }

	    //Closing the files
	    fclose(songInfo->fptr_song);
	    fclose(songInfo->fptr_temp);

	    //Reopening original file in write mode and temp in read mode
	    if (reopening_edit_files(songInfo) == success)
	    {
		//Copyning all content of temp to original file
		while (fread(&ch , sizeof(char), sizeof(char) , songInfo->fptr_temp) > 0)
		{
		    fwrite(&ch , sizeof(char), sizeof(char) , songInfo->fptr_song);
		}
		//Closing both files
		fclose(songInfo->fptr_song);
		fclose(songInfo->fptr_temp);
		//deleting temp file
		remove("temp.mp3");
		return success;
	    }
	    else
	    {
		printf("ERROR : Reopening of files after editing failed.\n");
		return failure;
	    }
	}
	else
	{
	    printf("ERROR : Header ID and version not matching. Can't edit.\n");
	    return failure;
	}
    }
    else
    {
	printf("ERROR : File opening failed\n");
	return failure;
    }
}


// Functio to open the files.
Status open_edit_files(SongInfo *songInfo)
{
    // Opening source image
    songInfo->fptr_song = fopen(songInfo->track_name, "r");
    // Do Error handling
    if (songInfo->fptr_song == NULL)
    {
	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", songInfo->track_name);
    	return failure;
    }

    //Naming and opening temp file in write mode
    songInfo->temp_file = "temp.mp3";
    songInfo->fptr_temp = fopen(songInfo->temp_file, "w");
    // Do Error handling
    if (songInfo->fptr_temp == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", songInfo->temp_file);
    	return failure;
    }
    return success;
}

//Function  to reopen the files
Status reopening_edit_files(SongInfo *songInfo)
{
    // Opening original file in write mode
    songInfo->fptr_song = fopen(songInfo->track_name, "w");
    // Do Error handling
    if (songInfo->fptr_song == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", songInfo->track_name);
	return failure;
    }

    //Opeing temp file in read mode
    songInfo->fptr_temp = fopen(songInfo->temp_file, "r");
    // Do Error handling
    if (songInfo->fptr_temp == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", songInfo->temp_file);
	return failure;
    }
    return success;
}

