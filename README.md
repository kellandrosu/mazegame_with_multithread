# Text Adventure with Multithreading
A simple text adventure maze that uses multithreading to write and read the current time to a file.

## Compiling
"make rooms" will create the executible which populates the room files for the game.
"make adventure" will create the adventure game executible.

## Instructions
Call the rooms executible to create the psuedo-random room files for the adventure in a subdirectory.
Then call adventure executible to run the game. Input the name of the room displayed you wish to go to.

Inputing "time" during the game will cause main to create a pthread that will write the time to an output file "currentTime.txt". Main will then read from currentTime.txt after the pthread ends.

The game ends when the "End Room" is visited.
