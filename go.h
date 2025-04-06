#ifndef TASK2_H
#define TASK2_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>


#define PAGESIZE 4096


int ScanDirectory(const struct dirent **firstItem, const struct dirent **secondItem);
void getLs(const char *path);
void removeDirectory(const char *path);

void fileTouch(char *path);
void createDir(const char *path);
void fileCat(char *path);

void removeFile(char *path);
const char* getTypeOfByte(mode_t mode);
void createSymlink(char *path, char *linkPath);
void readSymlink(char *path);

void readFileFromSymlink(char *path);
void removeSymlink(char *path);
void createHardLink(char *path, char *linkPath);

void removeHardLink(char *path);
void getChmod(const char *path);
void setPermitChmod(const char *path, mode_t mode);

char* getLastSlashInPath(char* input);
char* extractFileNameFromPath(char path[]);






#endif