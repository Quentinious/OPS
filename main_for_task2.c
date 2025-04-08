#include "task2.c"


int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        printf("error %s please try again\n", argv[0]);
        return 1;
    }
    
    char* commandName = extractFileNameFromPath(argv[0]);
    if (commandName == NULL)
    {
        printf("u need write command name! please try agaain\n");
        return 1;
    }

    if (!strcmp(commandName, "mkdir")) 
    {
        createDir(argv[1]);
    }
    else if (!strcmp(commandName, "ls")) 
    {
        getLs(argv[1]);
    } 
    else if (!strcmp(commandName, "rmdir"))
    {
        removeDirectory(argv[1]);
    } 
    else if (!strcmp(commandName, "touch"))
    {
        fileTouch(argv[1]);
    } 
    else if (!strcmp(commandName, "cat")) 
    {
        fileCat(argv[1]);
    } 
    else if (!strcmp(commandName, "rm")) 
    {
        removeFile(argv[1]);
    }
    else if (!strcmp(commandName, "stat")) 
    {
        getChmod(argv[1]);
    } 
    else if (!strcmp(commandName, "chmod")) 
    {
        if (argc != 3)
        {
            return 1;
        }
        setPermitChmod(argv[1], strtol(argv[2], NULL, 8));
    } 
    else if (!strcmp(commandName, "lns"))
    {
        if (argc != 3)
        {
            return 1;
        }
        createSymlink(argv[1], argv[2]);
    } 
    else if (!strcmp(commandName, "catln")) 
    {
        if (argc != 2) 
        {
            return 1;
        }
        readSymlink(argv[1]);
    } 
    else if (!strcmp(commandName, "catlnfile")) 
    {
        if (argc != 2) 
        {
            return 1;
        }
        readFileFromSymlink(argv[1]);
    } 
    else if (!strcmp(commandName, "unlink")) 
    {
        if (argc != 2) 
        {
            return 1;
        }
        removeSymlink(argv[1]);
    }
    else if (!strcmp(commandName, "ln"))
    {
        if (argc != 3)
        {
            return 1;
        }
        createHardLink(argv[1], argv[2]);
    }

    else if 
    (!strcmp(commandName, "unlinkhard"))
    {
        if (argc != 2) 
        {
            return 1;
        }
        removeHardLink(argv[1]);
    }
    else
    {
        printf("Unknown command\n");
    }
    
    return 0;
    
}
