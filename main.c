#include "realstrix.c"



int main(int argc, char *argv[]) 
{


    char MyRealPath[PAGESIZE];
    if (argc != 2) 
    {
        printf("%s error format\n", argv[0]);
        return 0;
    }

    realpath(argv[1], MyRealPath);
    char directParName[PAGESIZE] = "";   
    char directName[PAGESIZE] = "";   
    char *lastSlash = strrchr(MyRealPath, '/'); 

    if (lastSlash != NULL) 
    {
        snprintf(directParName, sizeof(directParName), "%.*s", (int)(lastSlash - MyRealPath), MyRealPath);
        snprintf(directName, sizeof(directName), "%s", lastSlash + 1);
    } 
    else
    {
        snprintf(directParName, sizeof(directParName), ".");
        snprintf(directName, sizeof(directName), "%s", MyRealPath);
    }
    
    ReverseOrder(directName);
    
    char reverseDirPath[PAGESIZE] = "";
    snprintf(reverseDirPath, sizeof(reverseDirPath), "%s/%s", directParName, directName);
    mkdir(reverseDirPath, 0755);
    DirectoryCopy(MyRealPath, reverseDirPath);
    printf("Completed!\n");
    return 0;
}
