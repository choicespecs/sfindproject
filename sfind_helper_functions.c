#include "sfind_include.h"

/*
 * this function will set the flag depending on the char type set in. This is used together with the searchdir & search_dirAll functions.
 * if there is no error it will return the bitmask of the appropriate flag, however; if there is an invalid option call then it will return -2;
 */
static int checkType(char type)
{
    int flag = 0;
    switch(type)
    {
        // case for block buffered special
        case 'b':
            flag = flag | S_IFBLK;
            break;
        // case for character special
        case 'c':
            flag = flag | S_IFCHR;
            break;
        // case for directory
        case 'd':
            flag = flag | S_IFDIR;
            break;
        // case for named pipe (FIFO)
        case 'p':
            flag = flag | S_IFIFO;
            break;
        // case for regular file
        case 'f':
            flag = flag | S_IFREG;
            break;
        // case for symbolic link
        case 'l':
            flag = flag | S_IFLNK;
            break;
        // case for socket
        case 's':
            flag = flag | S_IFSOCK;
            break;
        // any other value is considered invalid;
        default:
            return -2;
            break;
    }
    return flag;
}


/*
 * this function will go through and iterate through and print the appropriate files and directories depending on the substring "findme" and the type specified. 
 * it will return 0 on successful execution and will return other values depending on errors within function.
 */
int searchdir (char * dirname, char *findme, char type)
{
    DIR * mydir;
    struct dirent *myfile = NULL;
    struct stat mystat;
    char * filename;
    char * sympathlink;
    static int booleanprint = 0; // boolean to check subdirectory if they match with substring
    int length_dir_path;
    int length;
    int lengthDir;
    char buffer[PATH_MAX];
    char * dirpath = realpath(dirname, buffer);
    lstat(dirname, &mystat); 
        
    int flag = 0;
    flag = checkType(type); //convert char type to flag used to check type
    //invalid type 
    if (flag == -2) 
    {
        return 22;
    }
    
    if ((mystat.st_mode & S_IRUSR) == 0)
    {
        errno = EACCES;
        fprintf( stderr, "Directory \'%s\' ", dirname);
        perror("");
        return 13;
    }

    if (*findme == '\0')
    {
        if (((mystat.st_mode & S_IFMT) == flag) && (booleanprint == 0))
        {
            printf("%s\n", dirname);
        }
    }
    else
    {
        if ((strstr(dirname, findme) != NULL) && ((mystat.st_mode & S_IFMT) == flag) && (booleanprint == 0))
        {
            printf("%s\n", dirpath);
        }
    }


    mydir = opendir(dirname);
    if (mydir == NULL)
    {
        errno = EPERM;
        fprintf(stderr, "Directory \'%s\' cannot be accessed ", dirname);
        perror("");
        return 1;
    }

    if (S_ISLNK(mystat.st_mode))
    {
        closedir(mydir);
        return 0;
    }

    myfile = readdir(mydir);
    while (myfile != NULL)
    {
        // create filename for each file within directory
        length = strlen(myfile->d_name);
        lengthDir = strlen(dirname);
        length = length + lengthDir + 2;
        filename = calloc(length, sizeof (char));
        strcpy(filename, dirname);
        strcat(filename, "/");
        strcat(filename, myfile->d_name);

        //search stat with filename
        lstat(filename, &mystat);

        /*
         * To execute print the following conditions must be met.
         * 1) file must match flag set.
         * 2) file must contain substring.
         * 3) File is not a directory (since directory is already printed or will be printed as it is entered)
         */

        if (*findme == '\0') 
        {

            if (((mystat.st_mode & S_IFMT) == flag) && (!(S_ISDIR(mystat.st_mode))))
            {
                printf("%s/", dirname);
                printf("%s\n", myfile->d_name);
            }
        }
        else
        {

            if (((mystat.st_mode & S_IFMT) == flag) && ((strstr(myfile->d_name, findme) != NULL)) && (!(S_ISDIR(mystat.st_mode))))
            {
                if (S_ISLNK(mystat.st_mode))
                {
                    length_dir_path = strlen(dirpath);
                    length = length_dir_path + length;
                    sympathlink = calloc(length, sizeof(char));
                    strcpy(sympathlink, dirpath);
                    strcat(sympathlink, "/");
                    strcat(sympathlink, myfile->d_name);
                    printf("%s\n", sympathlink);
                    free(sympathlink);
                }

                else
                {
                    char * path = realpath(filename, NULL);
                    printf("%s\n", path);
                    free(path);
                }
            }
        }

        // If file is a directory and is not "." or ".."
        if ((S_ISDIR(mystat.st_mode)) && ((strcmp(myfile->d_name, "..")) != 0) && ((strcmp(myfile->d_name, ".")) != 0))
        {   
            // this directory does not contain substring so set static booleanprint to 1 (so it does not print when entered)
            if (strstr(myfile->d_name, findme) == NULL) 
            {
                booleanprint = 1;
            }
            // this directory does contain substring so set static boolean print to 0.
            else 
            {
                booleanprint = 0;
            }
            // execute recursively and go into the directory.
            searchdir(filename, findme, type);
        }
        free(filename); 
        myfile = readdir(mydir);
    }
    closedir(mydir);

   return 0; 
}

/*
 * this function will search through directories and files to match the files and directories with the findme, this function is similar to searchdir except this function
 * does not specify by type and will search all types. this function will return 0 if successful and will return 1 if the directory is null.
 */
int searchdir_all (char * dirname, char *findme)
{
    
    DIR * mydir;
    struct dirent *myfile = NULL;
    struct stat mystat;
    static int booleanprint = 0;
    char * filename;
    char * sympathlink;
    char buffer[PATH_MAX];
    char * dirpath = realpath(dirname, buffer);
    int length_dir_path;
    int length;
    int lengthDir;
    lstat(dirname, &mystat); 
    //check if directory has substring and booleanprint is 0.
    
    if ((mystat.st_mode & S_IRUSR) == 0)
    {
        errno = EACCES;
        fprintf( stderr, "Directory \'%s\' ", dirname);
        perror("");
        return 13;
    }

    if (*findme == '\0')
    {
        printf("%s\n", dirname);
    }
    else
    {
        if ((strstr(dirname, findme) != NULL) && (booleanprint == 0))
        {
            printf("%s\n", dirpath);
        }
    }


    mydir = opendir(dirname);
    if (mydir == NULL)
    {
        errno = EPERM;
        fprintf(stderr, "Directory \'%s\' cannot be accessed ", dirname);
        perror("");
        return 1;
    }

    if (S_ISLNK(mystat.st_mode))
    {
        closedir(mydir);
        return 0;
    }

    myfile = readdir(mydir);
    while (myfile != NULL)
    {   
        //create filename for each file in directory
        length = strlen(myfile->d_name);
        lengthDir = strlen(dirname);
        length = length + lengthDir + 2;
        filename = calloc(length, sizeof (char));
        strcpy(filename, dirname);
        strcat(filename, "/");
        strcat(filename, myfile->d_name);

        //get information from stat about file.
        lstat(filename, &mystat);

        // only will check whether file matches substring and is not a directory.
        if (*findme == '\0')
        {
            if ((!(S_ISDIR(mystat.st_mode))))
            {
                printf("%s/", dirname);
                printf("%s\n", myfile->d_name);
            }
        }
        else
        {
            if ((strstr(myfile->d_name, findme) != NULL) && (!(S_ISDIR(mystat.st_mode))))
            {
                if (S_ISLNK(mystat.st_mode))
                {   
                    length_dir_path = strlen(dirpath);
                    length = length_dir_path + length;
                    sympathlink = calloc(length, sizeof(char));
                    strcpy(sympathlink, dirpath);
                    strcat(sympathlink, "/");
                    strcat(sympathlink, myfile->d_name);
                    printf("%s\n", sympathlink);
                    free(sympathlink);
                }
                else
                {
                    char * path = realpath(filename, NULL);
                    printf("%s\n", path);
                    free(path);
                }
            }   
        }
        // if file is a directory and is not "." or ".."
        if ((S_ISDIR(mystat.st_mode)) && ((strcmp(myfile->d_name, "..")) != 0) && ((strcmp(myfile->d_name, ".")) != 0))
        {
            // directory does not contain substring so booleanprint = 1 and will not print when entering directory
            if (strstr(myfile->d_name, findme) == NULL)
            {
                booleanprint = 1;
            }
            // directory does contain substring so booleanprint = 0 so it will print when entering directory.
            else
            {
                booleanprint = 0;
            }
            
            //continue recursively.
            searchdir_all(filename, findme);
        }
        free(filename);
        myfile = readdir(mydir);
        filename = NULL;
    }
    closedir(mydir);

   return 0; 
}
