#include "task2.h"



int ScanDirectory(const struct dirent **firstItem, const struct dirent **secondItem) 
{
    int res = strcmp((*firstItem)->d_name, (*secondItem)->d_name);
    return res;
}

void getLs(const char *path) 
{
    struct dirent **listOfDirent;
    int n = scandir(path, &listOfDirent, NULL, ScanDirectory);
    
    if (n == -1) 
    {
        printf("Error while scandir");
        return;
    }
    
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", listOfDirent[i]->d_name);
        free(listOfDirent[i]);  
    }
    free(listOfDirent);  
}


void removeDirectory(const char *path)
{
    DIR *dir = opendir(path);
    if (!dir) 
    {
        printf("error opening directory");
        return;
    }

    struct dirent *entry;           //dentry
    char filepath[PAGESIZE];  
    filepath[0] = '\0';
    
    while ((entry = readdir(dir))) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        
        snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);  
        
        struct stat st;
        if (stat(filepath, &st) == -1) 
        {
            printf("error with stat\n");
            continue;
        }
        if (S_ISDIR(st.st_mode)) 
        {
            removeDirectory(filepath); 
        } 
        else
        {
            if (unlink(filepath) == -1)
            {
                printf("error can't unlink\n");
            }
        }
    }
    
    closedir(dir);
    if (rmdir(path) == -1) 
    {
        printf("error cant rmdir\n");
    }
}


void createDir(const char *path)
{
    if (mkdir(path, 0777) == -1)
    {
        printf("error cant create mkdir");
    } 
}

void fileCat(char *path)
{
    char buffer[PAGESIZE];
    int fd = open(path, O_RDONLY);
    if (fd == -1) 
    {
        printf("can't open file");
        return;
    }
    
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, PAGESIZE)) > 0)
    {
        if (bytesRead == -1) 
        {
            printf("error reading file");
            return;
        }
        fwrite(buffer, 1, bytesRead, stdout);
    }
    
    if (bytesRead == -1)
    {
        printf("error reading file");
    }
    
    close(fd);
}


void fileTouch(char *path) 
{
    int fd = open(path, O_CREAT | O_WRONLY, 0666);
    if (fd == -1)
    {
        printf("cant open file");
    }
    else
    {
        close(fd);
        printf("successfully created file: %s\n", path);
    }
}



void removeFile(char *path)
{
    if (unlink(path) == -1)
    {
        printf("cant unlink file");
    } 
    else 
    {
        printf("successfully removed file: %s\n", path);
    }
}


void createSymlink(char *path, char *linkPath)
{
    if (symlink(path, linkPath) == -1)
    {
        printf("Error cant create symLink ");
    }
}


const char* getTypeOfByte(mode_t mode) 
{
    if (S_ISREG(mode))
        return "Regular file";
    if (S_ISDIR(mode))
        return "Directory";
    if (S_ISCHR(mode))
        return "Character device";
    if (S_ISBLK(mode))
        return "Block device";
    if (S_ISFIFO(mode))
        return "FIFO (pipe)";
    if (S_ISLNK(mode))  
        return "Symbolic link";
    if (S_ISSOCK(mode))
        return "Socket";
    return "Unknown";
}


void readSymlink(char *path)
{
    char buffer[PAGESIZE];
    int len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len == -1)
    {
        printf("error read link\n");
        return;
    }
    buffer[len] = '\0'; 
    printf("pointer to-> %s\n", buffer); 
}


void removeSymlink(char *path) 
{
    if (unlink(path) == -1) 
    {
        printf("Error remove synlink");
    }
}


void readFileFromSymlink(char *path)
{
    char buffer[PAGESIZE];
    char targetPath[PAGESIZE];
    int len = readlink(path, targetPath, sizeof(targetPath) - 1);
    if (len == -1) 
    {
        printf("Error in proccess to read symlink");
        return;
    }
    targetPath[len] = '\0';
    fileCat(targetPath);
}

void createHardLink(char *path, char *linkPath) 
{
    if (link(path, linkPath) == -1) 
    {
        printf("e rror creating hard link");
    }
}

void getChmod(const char *path)
{
    struct stat st;
    if (stat(path, &st) == -1)
    {
        printf("error getPermit");
        return;
    }
    printf("type file-> %s\n", getTypeOfByte(st.st_mode));
    char rwx[10];
    rwx[9] = '\0'; 
    rwx[0] = (st.st_mode & S_IRUSR) ? 'r' : '-';
    rwx[1] = (st.st_mode & S_IWUSR) ? 'w' : '-';
    rwx[2] = (st.st_mode & S_IXUSR) ? 'x' : '-';
    rwx[3] = (st.st_mode & S_IRGRP) ? 'r' : '-';
    rwx[4] = (st.st_mode & S_IWGRP) ? 'w' : '-';
    rwx[5] = (st.st_mode & S_IXGRP) ? 'x' : '-';
    rwx[6] = (st.st_mode & S_IROTH) ? 'r' : '-';
    rwx[7] = (st.st_mode & S_IWOTH) ? 'w' : '-';
    rwx[8] = (st.st_mode & S_IXOTH) ? 'x' : '-';

    printf("status -> %s\n", rwx);
    printf("quanity of links -> %ld\n", st.st_nlink);
}


void removeHardLink(char *path) 
{
    if (unlink(path) == -1) 
    {
        printf("error delete link to hard");
    }
    else
    {
        printf("successfully removed hard link: %s\n", path);
    }
}



void setPermitChmod(const char *path, mode_t mode) 
{
    if (chmod(path, mode) == -1)
    {
        printf("chmod error");
    } 
    else 
    {
        printf("Successfully changed permissions: %s\n", path);
    }
}

char* getLastSlashInPath(char* input)
{
    char *lastSlash = NULL;
    for (int it = 0; input[it] != '\0'; it++) 
    {
        if (input[it] == '/')
        {
            lastSlash = &input[it]; 
        }
    }
    return lastSlash; 
}

char* extractFileNameFromPath(char path[])
{ 
    char *lastSlash = getLastSlashInPath(path);  
    return ((lastSlash != NULL) ? lastSlash + 1 : path);
}

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
        printf("there is must be command\n");
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

