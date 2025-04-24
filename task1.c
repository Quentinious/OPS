#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <libgen.h>
#include <errno.h>

#define PAGESIZE 4096


void ReverseString(char *s)
{
    int len = strlen(s);
    for (int index = 0; index < len / 2; index++) 
    {
        char copy = s[index];
        s[index] = s[len - index - 1];
        s[len - index - 1] = copy;
    }
}

void ReverseBuffer(char *buffer, size_t len)
{
    for (size_t i = 0; i < len / 2; i++) 
    {
        char tmp = buffer[i];
        buffer[i] = buffer[len - i - 1];
        buffer[len - i - 1] = tmp;
    }
}



void CopyFileAnfReverse(const char *from, const char *to) 
{
    FILE *fromFile = fopen(from, "rb");
    if (!fromFile) 
    {
        fprintf(stderr, "Error opening source file '%s': %s\n", from, strerror(errno));
        return;
    }

    FILE *toFile = fopen(to, "wb");
    if (!toFile) 
    {
        fprintf(stderr, "Error creating destination file '%s': %s\n", to, strerror(errno));
        fclose(fromFile);
        return;
    }

    unsigned char buffer[PAGESIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fromFile)) > 0) 
    {
        ReverseBuffer(buffer, bytesRead);
        size_t totalWritten = 0;
        while (totalWritten < bytesRead) 
        {
            size_t bytesWritten = fwrite(buffer + totalWritten, 1, bytesRead - totalWritten, toFile);
            if (bytesWritten == 0) 
            {
                fprintf(stderr, "Write error to file '%s': %s\n", to, strerror(errno));
                fclose(fromFile);
                fclose(toFile);
                return;
            }
            totalWritten += bytesWritten;
        }
    }

    if (ferror(fromFile)) 
    {
        fprintf(stderr, "Read error from file '%s': %s\n", from, strerror(errno));
        fclose(fromFile);
        fclose(toFile);
        return;
    }
    fclose(fromFile);
    fclose(toFile);
}

void CopyDir(const char *src, const char *dst) 
{
    DIR *dir = opendir(src);
    if (!dir) 
    {
        fprintf(stderr, "Failed to open directory '%s': %s\n", src, strerror(errno));
        return;
    }

    struct stat st;
    if (stat(src, &st) != 0) 
    {
        fprintf(stderr, "Failed to stat directory '%s': %s\n", src, strerror(errno));
        closedir(dir);
        return;
    }

    if (mkdir(dst, st.st_mode) != 0 && errno != EEXIST)
    {
        fprintf(stderr, "Failed to create directory '%s': %s\n", dst, strerror(errno));
        closedir(dir);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;

        char srcPath[PAGESIZE];
        snprintf(srcPath, sizeof(srcPath), "%s/%s", src, entry->d_name);

        char reversedName[PAGESIZE];
        strncpy(reversedName, entry->d_name, sizeof(reversedName) - 1);
        reversedName[sizeof(reversedName) - 1] = '\0';
        ReverseString(reversedName);

        char dstPath[PAGESIZE];
        snprintf(dstPath, sizeof(dstPath), "%s/%s", dst, reversedName);

        if (stat(srcPath, &st) != 0)
        {
            fprintf(stderr, "Failed to stat '%s': %s\n", srcPath, strerror(errno));
            continue;
        }

        if (S_ISREG(st.st_mode)) 
        {
            CopyFileAnfReverse(srcPath, dstPath);
        }
        else if (S_ISDIR(st.st_mode)) 
        {
            if (mkdir(dstPath, st.st_mode) != 0 && errno != EEXIST)
            {
                fprintf(stderr, "Failed to create subdirectory '%s': %s\n", dstPath, strerror(errno));
                continue;
            }
            CopyDir(srcPath, dstPath);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{

    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    char buffRealPath[PAGESIZE];
    if (realpath(argv[1], buffRealPath) == NULL) 
    {
        fprintf(stderr, "Invalid path '%s': %s\n", argv[1], strerror(errno));
        return EXIT_FAILURE;
    }

    char dirPrevName[PAGESIZE] = "";
    char dirName[PAGESIZE] = "";
    char *lastSlash = strrchr(buffRealPath, '/');

    if (lastSlash != NULL)
    {
        if (lastSlash == buffRealPath) 
        {
            snprintf(dirPrevName, sizeof(dirPrevName), "/");
            snprintf(dirName, sizeof(dirName), "%s", lastSlash + 1);
        } 
        else
        {
            snprintf(dirPrevName, sizeof(dirPrevName), "%.*s", (int)(lastSlash - buffRealPath), buffRealPath);
            snprintf(dirName, sizeof(dirName), "%s", lastSlash + 1);
        }
    }

    ReverseString(dirName);

    char reverseDirPath[PAGESIZE];
    snprintf(reverseDirPath, sizeof(reverseDirPath), "%s/%s", dirPrevName, dirName);
    if (mkdir(reverseDirPath, 0755) != 0 && errno != EEXIST)
    {
        fprintf(stderr, "Failed to create root reversed directory '%s': %s\n", reverseDirPath, strerror(errno));
        return 1;
    }

    CopyDir(buffRealPath, reverseDirPath);

    printf("Copy completed successfully!\n");
    return 0;
}
