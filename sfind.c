#include "sfind_include.h"
#include "sfind_helper_functions.h"


int main (int argc, char* argv[] )
{
    char *dirname; // directory is always second argument.
    char * substring = "\0"; // default substring
    char type = '\0'; // default type 
    int error = 0; //error checking within the functions
    int opt;
    int declare_directory = 0; //check if directory has been declared or not within argument.

    // parse through cmd line arguments
    while ((opt = getopt(argc, argv, "-:n:t:s:")) != -1)
    {
        switch(opt)
        {
            // case for types
            case 't':
                // check if length of type is exactly 1 if not return error.
                if ((strlen(optarg)) > 1)
                {
                    errno = EINVAL;
                    perror("type option is too long, must be single char");
                    return 22;
                }
                type = optarg[0];
                break;
            // case for substring
            case 'n':
                substring = optarg;
                break;
            // case for directory
            case 's':
                // directory has already been declared within the argument.
                if (declare_directory == 1)
                {
                    errno = EINVAL;
                    perror("You must declare sfind directory only once in argument");
                    return 22;
                }
                dirname = optarg;
                declare_directory = 1; // directory has been declared.
                break;
            // check for optargments
            case ':':
                // option is not been set so make sure to send error to user.
                fprintf(stderr, "missing argument for %c: ", optopt);   
                errno = EINVAL;
                perror("");
                return 22;
                break;
            // check for all non optional arguments.
            case 1:
                // this is the second argument
                if (optind == 2)
                {
                    // directory has already been declared
                    if (declare_directory == 1)
                    {
                        errno = EINVAL;
                        perror("You must declare sfind directory only once in argument");
                        return 22;
                    }
                    dirname = optarg;
                    declare_directory = 1;
                } 
                else 
                {
                    errno = EINVAL;
                    perror("you must declare sfind directory using either -s [directory] or declare directory after sfind");
                    return 22;
                }
                
                // too many arguments are being used within sfind.
                if (optind > 7)
                {
                    errno = EINVAL;
                    perror("Too many arguments declared in sfind");
                    return 22;
                }
                break;
            //check all other options
            case '?':
                errno = EINVAL;
                fprintf(stderr, "option -%c does not exist ", optopt);
                perror("");
                return 22;
                break;
        }
    }
    
    // directory has not been declared yet so use "." as default.
    if (declare_directory == 0)
    {
        dirname = ".";
    }

    // type has not been set so all types will be searched
    if (type == '\0')
    {
        error = searchdir_all(dirname, substring);
        // first directory has perm error or cannot be accessed.
        if (error == 1)
        {
            errno = EPERM;
            perror("Directory cannot be accessed");
            return 1;
        }
        if (error == 13)
        {
            errno = EACCES;
            perror("Directory cannot be accessed");
            return 13;
        }
    }
    // type has been set so type will be assessed.
    else 
    {
        error = searchdir(dirname, substring, type);
        // type is invalid and has exit
        if (error == 22)
        {
            errno = EINVAL;
            perror("Invalid Type Set");
            return 22;
        }
        // first directory cannot be accessed.
        if (error == 1)
        {
            errno = EPERM;
            perror("Directory cannot be accessed");
            return 1;
        }
        if (error == 13)
        {
            errno = EACCES;
            perror("Directory cannot be accessed");
            return 13;
        }
    }
    return 0;
}
