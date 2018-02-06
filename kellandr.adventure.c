/**
**  Author:     Andrius Kelly
**  Date:       February 2, 2018
**  Description:    CS 344 Project 2. This program reads the most recently created .room files and executes the adventure program.
**/


#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



#define true 1
#define false 0

int main(void) {

	//find most recent rooms directory
	int latestTime = -1;
	char roomDirPrefix[] = "kellandr.rooms.";
	char roomDir[256] = {'\0'};	

	DIR* currentDir;
	struct dirent *fileInDir;
	struct stat dirAttributes;

	//open current directory
	currentDir = opendir(".");
	if ( currentDir > 0 ) {
		//read through files in curent dir
		while  ( (fileInDir = readdir(currentDir)) != NULL ) {
	
			if ( strstr( fileInDir->d_name, roomDirPrefix) != NULL ) {
				stat( fileInDir->d_name, &dirAttributes );
				
				if( (int)dirAttributes.st_mtime > latestTime ) {
					latestTime = (int)dirAttributes.st_mtime;
					memset(roomDir, '\0', sizeof(roomDir));
					strcpy(roomDir, fileInDir->d_name);
				}

			}
		}

	}
	else {		//exit if no rooms directory was found

	}

	//read rooms into memory
	printf("Latest Directory is %s\n", roomDir);

	//play game


	return 0;
}
