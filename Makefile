default: adventure rooms

adventure: kellandr.adventure.c
	gcc -g -Wall -lpthread kellandr.adventure.c -o adventure

rooms: kellandr.rooms.c
	gcc kellandr.rooms.c -o rooms
