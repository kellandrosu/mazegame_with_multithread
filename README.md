# Text Adventure with Multithreading
A simple text adventure maze that uses multithreading to write and read the current time to a file.
Inputing "time" during the game will cause main to create a pthread that will write the time to an output file "currentTime.txt".
Main will then read from currentTime.txt after the pthread ends.
