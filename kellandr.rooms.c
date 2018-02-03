/**
** 	Author:		Andrius Kelly
** 	Date:		February 2, 2018
** 	Description:	CS 344 Project 2. This file generates 7 random room files in a directory kellandr.rooms.<process>, to be used by kellandr.adventure.c
**/

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>


#define true 1
#define false 0
#define TOTAL_ROOMS 7
#define MIN_CONNECT 3
#define MAX_CONNECT 6

void createRoomFile( char* filepath, int roomNum,  char* type, char** names, int connections[][TOTAL_ROOMS]);

const char* hardCodeNames = "Gate Keep Dungeon Tower Turret Sewers Throne Jailcell Court Vault";

int main (void) {

	int i, j, counter;
	
	time_t t;
	srand( (unsigned) time(&t));

	//graph for connections
	int connections [TOTAL_ROOMS][TOTAL_ROOMS];
	memset(connections, false, (TOTAL_ROOMS * TOTAL_ROOMS) * sizeof(int) );	

//randomly fill connections
	int isComplete = false;
	while ( isComplete == false ) {

		//create random connection
		i = rand() % ( TOTAL_ROOMS );
		j = rand() % ( TOTAL_ROOMS );
		if ( j != i ) {
			connections[i][j] = true;
			connections[j][i] = true;
		}

		//check if all rooms have 3 connections 
		isComplete = true;
		for (i=0; i<TOTAL_ROOMS; i++) {
			counter=0;
			for (j=0; j<TOTAL_ROOMS; j++){
				if ( i != j && connections[i][j] ) {
					counter++;	
				}
			}
			isComplete = (counter >= 3) && isComplete;
		}
	}
	
	 
//randomly assign names
	char* randomNames[10];

	//copy string literal hardCodeNames so we can tokenize it
	char* roomNames = malloc( (strlen(hardCodeNames) + 1) * sizeof(char) );
	strcpy(roomNames, hardCodeNames);
	char* token = strtok(roomNames, " ");

	//convert roomsNames string to array of name
	for( i = 0; i < 10; i++) {

	randomNames[i] = malloc( (strlen(token) + 1) * sizeof(char) );
		strcpy( randomNames[i], token);
		token = strtok(NULL, " ");
	}

	//randomize rooms
	int randomIndex;
	char* tempString;
	for( i = 10; i > 0; i-- ) {
		randomIndex = rand() %  i ;
		tempString = randomNames[randomIndex];
		randomNames[randomIndex] = randomNames[ i - 1 ];
		randomNames[ i - 1 ] = tempString;
	}


/*//TEST PRINT
	for (i=0; i<TOTAL_ROOMS; i++) {
		for (j=0; j<TOTAL_ROOMS; j++){
			printf("%d ", connections[i][j]);
		}
		printf("\n");
	}

	printf("%s\n", hardCodeNames);

	for (i=0; i<10; i++) { 
		printf("%s ", randomNames[i]);
	}
	
	printf("\n\n");
//END TEST PRINT
*/

//save rooms as file
	
	//create directory
	char* directory = NULL;
	asprintf( &directory, "kellandr.rooms.%d", getpid());

	mkdir( directory, 0755);

	char* filepath = NULL;
	
	//do START_ROOM
	asprintf( &filepath, "%s/%s", directory, randomNames[0]);
	createRoomFile( filepath, 0, "START_ROOM", randomNames, connections); 
	free(filepath);

	//loop through MID_ROOMs
	for( i=1; i<TOTAL_ROOMS - 1; i++) {
		asprintf( &filepath, "%s/%s", directory, randomNames[i]);
		createRoomFile( filepath, i, "MID_ROOM", randomNames, connections); 
		free(filepath);
	}

	//last room is END_ROOM
	asprintf( &filepath, "%s/%s", directory, randomNames[TOTAL_ROOMS - 1]);
	createRoomFile( filepath, TOTAL_ROOMS - 1, "END_ROOM", randomNames, connections); 
	free(filepath);

	//free memory
	free(directory);
	for (i=0; i<10; i++){ free(randomNames[i]); }
	free(roomNames);

	return 0;
}


//write roomfile 
void createRoomFile( char* filepath, int roomNum,  char* type, char** names, int connections[][TOTAL_ROOMS]) {

	//open file for writing	
	int fileDescriptor;
	fileDescriptor = open( filepath, O_WRONLY | O_CREAT | O_TRUNC, 0755 );
	
	if ( fileDescriptor < 0 ){
		fprintf(stderr, "Could not open %s\n", filepath );
		perror("Error in createRoomFile()");
		exit(1);
	}

	char* writeLine = NULL;
	
	asprintf( &writeLine, "ROOM NAME: %s\n", names[roomNum] );
	write(fileDescriptor, writeLine, strlen(writeLine) * sizeof(char) );
	free(writeLine);

	int i;
	int counter = 1;
	for( i=0; i < TOTAL_ROOMS ; i++) {
		if( i != roomNum && connections[roomNum][i] ) {
			asprintf( &writeLine, "CONNECTION %d: %s\n", counter, names[i] );
			write(fileDescriptor, writeLine, strlen(writeLine) * sizeof(char) );
			free(writeLine);
			counter++;
		}
	}
	
	asprintf( &writeLine, "ROOM TYPE: %s\n", type );
	write(fileDescriptor, writeLine, strlen(writeLine) * sizeof(char) );
	free(writeLine);

	close(fileDescriptor);
}

