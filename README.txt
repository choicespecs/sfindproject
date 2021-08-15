Project 3: Sfind

Created by:
Christopher Lee

1. Introduction
-------------------
This function is similar to find. It will print out the directories and files. However; it will also take arguments
to search for specific substrings and types. This is different than find in that you can print and search for files, directories,
and other types based on substrings. Sfind can take arguments to search for substrings or types. Both are optional arguments
and the only required argument for sfind is just the directory to search through.

A few important things to note about my sfind are:
1) If NO name is selected it will give you the relative path in the directory selected as would find
2) If A name is specified it will give you the absolute path in the directory files selected.
3) Both of these conditions are irregardless of the type selected so even if a type is specfifed or not it will be more dependent on whether the name is specified or not.

2. Installation.
--------------------
This file will require the following files for installation.

    a) sfind.c
    b) sfind_helper_functions.c
    c) sfind_helper_functions.h
    d) sfind_include.h

A makefile has been provided to assist with the installation of the sfind file (as well as
included the functionality to remove all relevant files). The sfind.c file contains mainly
error checking for the functions utilized within sfind_helper_function file.

If any problems occur with the makefile compilation, you should be able to create an executable
using the following command with gcc:
    "gcc sfind.c sfind_helper_functions.c -o sfind"


3. Files / Methods
--------------------
The chunk file is separated only into two main files.

    a) sfind.c
    b) sfind_helper_functions.c

a) sfind.c
    this file is the driver for the file, and doesn't contain any methods itself. It will
    perform error checking throughout the reading and writing of the files that are read or written within sfind.

b) sfind_helper_functions.c

    i) static int checkType(char type)

    this function will convert the char type argument to the bitmask flag. It is used within searchdir to obtain the flag to check for. If an applicable char type is inserted it will return the 
    allocated type bitmask flag. However, if that type is not found or seen, this function will return -2 for error.

    ii) int searchdir (char * dirname, char * findme, char type)

    this function will search through the dirname to find the substring given by findme and will look through by the char type given. This function will print out the appropriate files and directories
    that match the findme substring and the type specified. It will return 0 if successful and will return 1 if there is an error with accessing the directory.

    iii) searchdir_all(char * dirname, char * findme)

    this function is similar to searchdir except this will search all files and directories without looking for any specific type so will print all directories, files, etc which will match the findme.


4. References
----------------

lstat man page
//man7.org/linux/man-pages/man2/lstat.2.html

inode man page
https://man7.org/linux/man-pages/man7/inode.7.html

strstr man page
https://man7.org/linux/man-pages/man3/strstr.3.html

strcmp man page
https://www.tutorialspoint.com/c_standard_library/c_function_strcmp.htm

