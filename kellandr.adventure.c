/**
**  Author:     Andrius Kelly
**  Date:       February 2, 2018
**  Description:    CS 344 Project 2. This program reads the most recently created .room files and executes the adventure program.
**/


#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
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
void writeTime();
char* readRoomFile( char* filepath);
void printRoom(struct Room* room) ;
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
	int i;
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


    //get start room
    struct Room* currentRoom;
    for( i=0; i<TOTAL_ROOMS; i++) {
        currentRoom = rooms[i];
        if ( strcmp(currentRoom->type, "START_ROOM") )
            break;
    }

	//play game
	char* userInput = NULL;
    size_t maxBuffer = 256;
    struct Room* nextRoom;
    int steps = 0;
	struct Room* path[256];

    printRoom(currentRoom);
    
    while( strcmp( currentRoom->type, "END_ROOM") != 0 ) {
        
        printf(".\nWHERE TO? >");
        
        getline( &userInput, &maxBuffer, stdin);
        //scrub trailing \n from input
        userInput[ strlen(userInput) - 1 ] = '\0';
        
        if( strcmp( userInput, "time") == 0 ) {
            //create a thread that records and prints time
            writeTime();
            //lock time output file
            continue;
        }

        //find room connection by name
		nextRoom = NULL;
        for( i=0; i<TOTAL_ROOMS; i++) {
            if ( currentRoom->connections[i] != NULL) {
                if( strcmp( currentRoom->connections[i]->name, userInput) == 0 ) {
                    nextRoom = currentRoom->connections[i];
                    break;
                }
            }
		}
        if ( nextRoom == NULL ) {
            printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
        }
        else {
            currentRoom = nextRoom;
            path[steps] = currentRoom;
            steps++;
            
            if ( strcmp( currentRoom->type, "END_ROOM") != 0 ) {
                printRoom(currentRoom);
            }
        }
        printf("\n");
    }

	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:", steps);
	for( i=0; i<steps; i++) {
		printf("\n%s", path[i]->name);
	}

    //free memory
	free (userInput);
    for( i=0; i<TOTAL_ROOMS; i++) {
		free(rooms[i]);
	}
	printf("\n");


	return 0;
}



/* --------------------------- FUNCTIONS -------------------------- */

//open a file and write it the time to it
void writeTime(){

    time_t t;
    struct tm *tmp;
    
    t = time(NULL);
    tmp = localtime(&t);
    
    if (tmp == NULL) {
       perror("localtime");
       exit(EXIT_FAILURE);        
    }

    char timeString[256];

    if( strftime( timeString, sizeof(timeString), "%I:%M%p, %A, %B, %e, %G\n" , tmp) == 0 ) {
        fprintf(stderr, "strftime returned 0");
        exit(EXIT_FAILURE);
    }
    
    
    char* fileName = "currentTime.txt";

    int fileDesc = open( fileName, O_WRONLY | O_TRUNC | O_CREAT, 0755);

    if ( fileDesc < 0 ){
        fprintf(stderr, "Could not open %s\n", fileName );
        perror("Error in writeTime()");
        exit(1);
    }

    write( fileDesc, timeString, strlen(timeString) * sizeof(char));

    close( fileDesc );

//TODO: DELETE THIS:
printf("%s",timeString);
}


//print the current room location and connections
void printRoom(struct Room* room) {    
    printf("CURRENT LOCATION: %s\nPOSSIBLE CONNECTIONS: %s", room->name, room->connections[0]->name);

    int i=1;

    while( room->connections[i] != NULL ) {
        printf(", %s", room->connections[i]->name);
        i++;
    }
}


//  Adds the connections and room type to a Room struct from a string
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


//read roomfile and return it as a string
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


