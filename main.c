/*
    Title       : Mp3 Tag-reader. 
    Author      : Prajwal S
    Date        : 23/08/2020
    Description : Input  : Read choice and pass song file.
                  Output : Depending on choice display, edit tags.
*/

#include <stdio.h>
#include <string.h>
#include "types.h"
#include "display.c"
#include "edit.c"
#include "common.h"
#include "display.h"
#include "edit.h"
#include "common.c"

int main(int argc, char *argv[])
{
    //Declaring the structure
    SongInfo songInfo;

    //Validation for not passing the option
    if (argc > 1)
    {
	//Comparing the operation type for help menu
	if (check_operation_type(argv) == help)
	{
	    printf("\nHelp menu for Mp3 Tag Reader and Editor:\n");
	    printf("----------------------------------------\n");
	    printf("For viewing the tags : ./mp3_tag_reader -v <mp3 file>\nFor editing the tags : ./mp3_tag_reader -e <mp3 file> <modifier> <\"tag\">\n");
	    printf("\nModifier\tFunction\n");
	    printf("-t\t\tModifies Title tag\n-a\t\tModifies Artist tag\n-A\t\tModifies Album tag\n-y\t\tModifies Year tag\n-c\t\tModifies Comment tag\n\n");
	}

	//To display the tags of song
	else if (check_operation_type(argv) == view)
	{
	    //Validating for not passing of arguments
	    if (argc == 3)
	    {
		//Validating the passed arguments
		if (read_and_validate_display_args(argv, &songInfo) == success)
		{
		    //Calling function to dispaly tags
		    if (decode_and_display(&songInfo) == success)
		    {
			printf("\nExtracting the tags of song done.\n");
			return 0;
		    }
		    else
		    {
			printf("ERROR : Decode and dispaly failure\n");
			return -1;
		    }
		}
		else
		{
		    printf("ERROR : Validation failure.\nUsage :./a.out -v <song_file>\n");
		    return -1;
		}
	    }
	    else
	    {
		printf("ERROR : arguments are not passed properly for view.\nUsage :./a.out -v <song_file>\n");
	    }
	}

	//To edit content
	else if (check_operation_type(argv) == edit)
	{
	    //Validation if all 5 arguments are not passed
	    if (argc == 5)
	    {
		//Validating the passed arguments
		if (read_and_validate_edit_args(argv, &songInfo) == success)
		{
		    //Comparing string for each operation
		    if (strcmp(argv[3], "-t") == 0)
		    {
			if (edit_detail("TIT2", &songInfo) == success)
			{
			    decode_and_display(&songInfo);
			    printf("\nTitle modification done.\n");
			    return 0;
			}
			else
			{
			    printf("ERROR :Title modification failed\n");
			    return -1;
			}
		    }
		    else if (strcmp(argv[3], "-a") == 0)
		    {
			if (edit_detail("TPE1", &songInfo) == success)
			{
			    decode_and_display(&songInfo);
			    printf("\nArtist name modification done\n");
			    return 0;
			}
			else
			{
			    printf("ERROR :Artist name modification failed.\n");
			    return -1;
			}
		    }
		    else if (strcmp(argv[3], "-A") == 0)
		    {
			if (edit_detail("TALB", &songInfo) == success)
			{
			    decode_and_display(&songInfo);
			    printf("\nAlbum name modification done.\n");
			    return 0;
			}
			else
			{
			    printf("ERROR :Album name modification failed\n");
			    return -1;
			}
		    }
		    else if (strcmp(argv[3], "-y") == 0)
		    {
			if (edit_detail("TYER", &songInfo) == success)
			{
			    decode_and_display(&songInfo);
			    printf("\nYear modification done.\n");
			    return 0;
			}
			else
			{
			    printf("ERROR :Year modification failed.\n");
			    return -1;
			}
		    }
		    else if (strcmp(argv[3], "-c") == 0)
		    {
			if (edit_detail("COMM", &songInfo) == success)
			{
			    decode_and_display(&songInfo);
			    printf("\nComment modification done.\n");
			    return 0;
			}
			else
			{
			    printf("ERROR :Comment modification failed.\n");
			    return -1;
			}
		    }
		    else
		    {
			printf("ERROR :Passed modifier is wrong.\nFor help :./a.out -h\n");
			return -1;
		    }
		}
		else
		{
		    printf("ERROR :Valadation of edit args failed.\nUsage :./a.out -e <song_file> <modifier> <\"new tag\">\n");
		    return -1;
		}
	    }
	    else
	    {
		printf("ERROR :The arguments are not proper.\nUsage :./a.out -e <song_file> <modifier> <\"new tag\">\n");
		return -1;
	    }
	}
	else
	{
	    printf("ERROR : Unsupported choice\nFor help:./a.out -h\n");
	    return -1;
	}
    }
    else
    {
	printf("ERROR : Invalid arguments.\nUsage :./a.out -h for help.\n");
	return -1;
    }
}
