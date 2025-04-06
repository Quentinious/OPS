#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>

#define PAGESIZE 4096




void copyFilesAndReversed(const char *from, const char *to);
void ReverseOrder(char *s);
void DirectoryCopy(const char *src, const char *dst);


#endif
