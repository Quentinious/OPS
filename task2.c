#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PAGESIZE 4096

void GetLs(const char *path)
{
    struct dirent **namelist;
    int n = scandir(path, &namelist, NULL, NULL);
    if (n == -1)
    {
        fprintf(stderr, "Error scanning directory '%s': %s\n", path,strerror(errno));
        return;
    }
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", namelist[i]->d_name);
        free(namelist[i]);
    }
    free(namelist);
}

void RemoveDirectory(const char *path)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        fprintf(stderr, "Failed to open directory '%s': %s\n", path,strerror(errno));
        return;
    }

    struct dirent *entry;
    char filepath[PAGESIZE];
    while ((entry = readdir(dir)))
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
        continue;

        snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);
        struct stat st;

        if (stat(filepath, &st) == -1)
        {
            fprintf(stderr, "Failed to stat file '%s': %s\n", filepath,strerror(errno));
            continue;
        }

        if (S_ISDIR(st.st_mode))
        {
            RemoveDirectory(filepath);
        }
        else
        {
            if (unlink(filepath) == -1)
            {
                fprintf(stderr, "Failed to remove file '%s': %s\n", filepath,strerror(errno));
            }
        }
    }

    closedir(dir);
    if(rmdir(path) == -1)
    {
        fprintf(stderr, "Failed to remove directory '%s': %s\n", path,strerror(errno));
    }
}

void FileTouch(const char *path)
{
    int fd = open(path, O_CREAT | O_WRONLY, 0666);
    if(fd == -1)
    {
        fprintf(stderr, "Failed to create file '%s': %s\n", path, strerror(errno));
    }
    else
    {
        close(fd);
        printf("File created: %s\n", path);
    }
}

void CreateDir(const char *path)
{
    if (mkdir(path, 0777) == -1)
    {
        fprintf(stderr, "Failed to create directory '%s': %s\n", path,strerror(errno));
    }
    else
    {
        printf("Directory created: %s\n", path);
    }
}

void FileCat(const char *path)
{
    char buffer[PAGESIZE];
    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "Failed to open file '%s': %s\n", path, strerror(errno));
        return;
    }

    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
    {
        fwrite(buffer, 1, bytesRead, stdout);
    }

    if (bytesRead == -1)
    {
        fprintf(stderr, "Error reading file '%s': %s\n", path, strerror(errno));
    }

    close(fd);
}

void RemoveFile(const char *path)
{
    if (unlink(path) == -1)
    {
        fprintf(stderr, "Failed to remove file '%s': %s\n", path, strerror(errno));
    }
    else
    {
        printf("File removed: %s\n", path);
    }
}

const char *GetTypeOfByte(mode_t mode)
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

void CreateSymLink(const char *target, const char *linkPath)
{
    if (symlink(target, linkPath) == -1)
    {
        fprintf(stderr, "Failed to create symbolic link from '%s' to '%s': %s\n",linkPath, target, strerror(errno));
    }
}

void ReadSymLink(const char *path)
{
    char buffer[PAGESIZE];
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len == -1)
    {
        fprintf(stderr, "Failed to read symbolic link '%s': %s\n", path,strerror(errno));
        return;
    }
    buffer[len] = '\0';
    printf("Symbolic link points to: %s\n", buffer);
}

void ReadFileFromSymLink(const char *path)
{
    char targetPath[PAGESIZE];
    ssize_t len = readlink(path, targetPath, sizeof(targetPath) - 1);
    if (len == -1)
    {
        fprintf(stderr, "Failed to read symbolic link '%s': %s\n", path,strerror(errno));
        return;
    }
    targetPath[len] = '\0';
    FileCat(targetPath);
}

void RemoveSymLink(const char *path)
{
    if (unlink(path) == -1)
    {
        fprintf(stderr, "Failed to remove symbolic link '%s': %s\n", path,strerror(errno));
    }
    else
    {
        printf("Symbolic link removed: %s\n", path);
    }
}

void CreateHardLink(const char *path, const char *linkPath)
{
    if (link(path, linkPath) == -1)
    {
        fprintf(stderr, "Failed to create hard link from '%s' to '%s': %s\n",linkPath, path, strerror(errno));
    }
}

void RemoveHardLink(const char *path)
{
    if (unlink(path) == -1)
    {
        fprintf(stderr, "Failed to remove hard link '%s': %s\n", path,strerror(errno));
    }
    else
    {
        printf("Hard link removed: %s\n", path);
    }
}

void GetChmod(const char *path)
{
    struct stat st;
    if (stat(path, &st) == -1)
    {
        fprintf(stderr, "Failed to get file status for '%s': %s\n", path,strerror(errno));
        return;
    }

    printf("Type: %s\n", GetTypeOfByte(st.st_mode));

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

    printf("Permissions: %s\n", rwx);
    printf("Number of links: %ld\n", st.st_nlink);
}

void SetPermitChmod(const char *path, mode_t mode)
{
    if (chmod(path, mode) == -1)
    {
        fprintf(stderr, "Failed to change permissions for '%s': %s\n", path,strerror(errno));
    }
    else
    {
        printf("Permissions changed: %s\n", path);
    }
}


char *ExtractFileNameFromPath(char *path)
{
    char *lastSlash = strrchr(path, '/');
    return lastSlash ? lastSlash + 1 : path;
}



int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <arguments>\n", argv[0]);
        return 1;
    }

    char *command = ExtractFileNameFromPath(argv[0]);

    if (!strcmp(command, "mkdir"))
    {
        CreateDir(argv[1]);
    }
    else if (!strcmp(command, "ls"))
    {
        GetLs(argv[1]);
    }
    else if (!strcmp(command, "rmdir"))
    {
        RemoveDirectory(argv[1]);
    }
    else if (!strcmp(command, "touch"))
    {
        FileTouch(argv[1]);
    }
    else if (!strcmp(command, "cat"))
    {
        FileCat(argv[1]);
    }
    else if (!strcmp(command, "rm"))
    {
        RemoveFile(argv[1]);
    }
    else if (!strcmp(command, "stat"))
    {
        GetChmod(argv[1]);
    }
    else if (!strcmp(command, "chmod"))
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: chmod <file> <permissions>\n");
            return EXIT_FAILURE;
        }
        SetPermitChmod(argv[1], strtol(argv[2], NULL, 8));
    }
    else if (!strcmp(command, "lns"))
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: lns <target> <link>\n");
            return EXIT_FAILURE;
        }
        CreateSymLink(argv[1], argv[2]);
    }
    else if (!strcmp(command, "catln"))
    {
        ReadSymLink(argv[1]);
    }
    else if (!strcmp(command, "catlnfile"))
    {
        ReadFileFromSymLink(argv[1]);
    }
    else if (!strcmp(command, "unlink"))
    {
        RemoveSymLink(argv[1]);
    }
    else if (!strcmp(command, "ln"))
    {
        CreateHardLink(argv[1], argv[2]);
    }
    else if (!strcmp(command, "unlinkhard"))
    {
        RemoveHardLink(argv[1]);
    }
    else
    {
        fprintf(stderr, "Unknown command: %s\n", command);
    }

    return 0;
}
