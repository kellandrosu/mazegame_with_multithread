/**
** 	Author:		Andrius Kelly
** 	Date:		February 2, 2018
** 	Description:	CS 344 Project 2. This file generates 7 random room files in a directory kellandr.rooms.<process>, to be used by kellandr.adventure.c
**/

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define true 1
#define false 0
#define TOTAL_ROOMS 7
#define MIN_CONNECT 3
#define MAX_CONNECT 6

int main (void) {

	int i, j, counter;
	
	time_t t;
	srand( (unsigned) time(&t));

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
	char* hardCodeNames = "Gate Keep Dungeon Tower Turret Sewers Throne Kitchen Court Vault";
	char* randomRooms[10];

	//copy string literal hardCodeNames so we can tokenize it
	char* roomNames = malloc( (strlen(hardCodeNames) + 1) * sizeof(char) );
	strcpy(roomNames, hardCodeNames);
	char* token = strtok(roomNames, " ");

	//convert roomsNames string to array of name
	for( i = 0; i < 10; i++) {

	randomRooms[i] = malloc( (strlen(token) + 1) * sizeof(char) );
		strcpy( randomRooms[i], token);
		token = strtok(NULL, " ");
	}

	//randomize rooms
	int randomIndex;
	char* tempString;
	for( i = 10; i > 0; i-- ) {
		randomIndex = rand() %  i ;
		tempString = randomRooms[randomIndex];
		randomRooms[randomIndex] = randomRooms[ i - 1 ];
		randomRooms[ i - 1 ] = tempString;
	}


//TEST PRINT
	for (i=0; i<TOTAL_ROOMS; i++) {
		for (j=0; j<TOTAL_ROOMS; j++){
			printf("%d ", connections[i][j]);
		}
		printf("\n");
	}

	printf("%s\n", hardCodeNames);

	for (i=0; i<10; i++) { 
		printf("%s ", randomRooms[i]);
	}
	
	printf("\n\n");

	//save rooms as files.

		//set room types for first and last room

	//free randomrooms

	free(roomNames);

	return 0;
}



