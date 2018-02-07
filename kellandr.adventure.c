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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define true 1
#define false 0
#define TOTAL_ROOMS 7

//definitions
struct Room {
	char name[256];
	char type[12];
	struct Room* connections[TOTAL_ROOMS];
};

//prototypes
char* readRoomFile( char* filepath);
void addRoomDetails(char* roomString, struct Room* rooms[], struct Room* room) ;

int main(void) {

	//find most recent rooms directory
	int latestTime = -1;
	char roomDirPrefix[] = "kellandr.rooms.";
	char roomDir[256] = {'\0'};	

	DIR* readingDir;
	struct dirent *fileInDir;
	struct stat dirAttributes;

	//open current directory
	readingDir = opendir(".");
	if ( readingDir > 0 ) {
		//read through files in curent dir
		while  ( (fileInDir = readdir(readingDir)) != NULL ) {
	
			//if the directory has our desired prefix
			if ( strstr( fileInDir->d_name, roomDirPrefix) != NULL ) {
				stat( fileInDir->d_name, &dirAttributes );
				
				//get the latest directory
				if( (int)dirAttributes.st_mtime > latestTime ) {
					latestTime = (int)dirAttributes.st_mtime;
					memset(roomDir, '\0', sizeof(roomDir));
					strcpy(roomDir, fileInDir->d_name);
				}
			}
		}
	}
	else {		//exit if no rooms directory was found
		//TODO
	}
	closedir(readingDir);


	struct Room* rooms[TOTAL_ROOMS] = {NULL};
	
	//add room structs to rooms[] for each file in roomDir
	readingDir = opendir(roomDir);
	if ( readingDir > 0 ) {
	
		int roomIndex = 0;
		
		while  ( (fileInDir = readdir(readingDir)) != NULL ) {	//read through files in curent dir

			//makesure we're not reading ./ or ../ and add room
			if( strchr(fileInDir->d_name, '.') == NULL ) {
				rooms[roomIndex] = malloc(sizeof(struct Room));
				strcpy( rooms[roomIndex]->name, fileInDir->d_name );
				roomIndex++;
			}	
		}
	}
	
	
	//read each room info into memory
	int i,j;
	char filepath[256];
	char* roomFileString;
	for(i=0; i<TOTAL_ROOMS; i++) {
		strcpy( filepath, roomDir );
		strcat( filepath, "/");
		strcat( filepath, rooms[i]->name );
		roomFileString = readRoomFile(filepath);
		addRoomDetails( roomFileString, rooms, rooms[i] );
		free(roomFileString);
	}


	//play game
	}

	
	for( i=0; i<TOTAL_ROOMS; i++) {
		free(rooms[i]);
	}
	printf("\n");


	return 0;
}


/* --------------------------- FUNCTIONS -------------------------- */

void addRoomDetails(char* roomString, struct Room* rooms[], struct Room* room) {

	int i;
	int connectionNum;
	struct Room* connection;

	char* token = strtok(roomString, "\n");			//get each line of roomString

	char label1[20], label2[20], label3[20];			//to store each line's info

	while( token != NULL ) {
	
		sscanf(token, "%s %s %s", label1, label2, label3);	

//printf("%s %s %s\n", label1, label2, label3);
		
		//add connection
		if ( strcmp(label1, "CONNECTION") == 0 ) { 
			//find pointer to room connection
			connection = NULL;
			
			//find index of room connection
			for (i=0; i<TOTAL_ROOMS; i++) {
				if(strcmp(label3, rooms[i]->name) == 0 ) {
					connection = rooms[i];
					break;
				}
			}

			connectionNum = label2[0] - '1';

			//append connection to room->connection
			room->connections[connectionNum] = connection;
		}
		//else add type
		else if ( strcmp( label2, "TYPE:" ) == 0 ) {
			strcpy( room->type, label3 );
		}
		
		token = strtok(NULL, "\n");
	}
}


//read roomfile
char* readRoomFile( char* filepath){

	int fileDescriptor;

	fileDescriptor = open( filepath, O_RDONLY);

	if (fileDescriptor < 0 ) {
		fprintf(stderr, "Could not open %s\n", filepath);
		perror("Error in readRoom()"); 
		exit(1);
	}

	//read file and store in fileString
	char buffer[256] = {'\0'};
	int buffMultiplyer = 1;
	char* temp = NULL; 
	char* fileString = NULL;

	while ( read ( fileDescriptor, buffer, sizeof(buffer) ) > 0 ) {
		temp = malloc( buffMultiplyer * sizeof(buffer) );
		memset( temp, '\0',  buffMultiplyer * sizeof(buffer) + 1 );

		if( fileString != NULL) { 
			strcpy( temp, fileString );
			free(fileString);
		}
		
		strcat( temp, buffer );

		fileString = temp;

		buffMultiplyer += 1;
	}
	
	return fileString;
}


