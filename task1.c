#include "task2.h"



void ReverseOrder(char *s)
{
    int len = strlen(s);
    for (int i = 0, j = len - 1; i < j; i++, j--) 
    {
        char copy = s[i];
        s[i] = s[j];
        s[j] = copy;
    }
}


void copyFilesAndReversed(const char *from, const char *to)
{
    FILE *fromFile = fopen(from, "rb");
    if (!fromFile)
    {
        printf("сan't open %s source file plesae try again", from);
        return;
    }
    size_t bytesToWrite,sumOfWrittenBytes;
    FILE *toFile = fopen(to, "wb");
    if (!toFile)
    {
        printf("сan't open %s source file plesae try again", to);
        fclose(fromFile);
        return;
    }

    char buffer[PAGESIZE];
    size_t bytesToRead;

    while ((bytesToRead = fread(buffer, 1, PAGESIZE, fromFile)) > 0) 
    {
        ReverseOrder(buffer);
        sumOfWrittenBytes = 0;
        while (sumOfWrittenBytes < bytesToRead) 
        {
            bytesToWrite = fwrite(buffer + sumOfWrittenBytes, 1, bytesToRead - sumOfWrittenBytes, toFile);
            if (bytesToWrite == 0) 
            {
                printf("fail write file try again");
                fclose(fromFile);
                fclose(toFile);
                return;
            }
            sumOfWrittenBytes += bytesToWrite;
        }
    }

    if (ferror(fromFile))
    {
        printf("error file reading");
    }
    fclose(fromFile);
    fclose(toFile);
}

void DirectoryCopy(const char *src, const char *dst)
{
    
    struct stat st;
    struct dirent *entry;
    
    DIR *dir = opendir(src);
    if (!dir) 
    {
        printf("can't open directory");
        return;
    }
    mkdir(dst, st.st_mode);     
    
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
        {
            continue;
        }
       

        char srcPath[PAGESIZE] = "";
        snprintf(srcPath, sizeof(srcPath), "%s/%s", src, entry->d_name);
        char reversedName[PAGESIZE] = "";
        strncpy(reversedName, entry->d_name, PAGESIZE - 1);
        reversedName[PAGESIZE - 1] = '\0';
        ReverseOrder(reversedName);
        
        char dstPath[PAGESIZE] = "";
        snprintf(dstPath, sizeof(dstPath), "%s/%s", dst, reversedName);

    
        if (S_ISREG(st.st_mode)) 
        {
            copyFilesAndReversed(srcPath, dstPath);
        } 
        if (S_ISDIR(st.st_mode)) 
        {
            mkdir(dstPath, st.st_mode);
            DirectoryCopy(srcPath, dstPath);  
        }
        printf("copy and reverse this file or directory\n");
    }
    closedir(dir);
}

