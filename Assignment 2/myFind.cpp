#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

/*
 * This class will find all files in directory and display to console
 * Reference: https://github.com/coskunsahin1/CPSC457/blob/master/week4/recursive_ls.c
 */

void find(const char *path, int level) 
{
    //open directory
    DIR *directory = opendir(path);    
    if(!directory) return;

    struct dirent *entry = NULL;

    //for every entry in the directory
    while(entry = readdir(directory)) 
    {
        if(strcmp(".", entry->d_name) && strcmp("..", entry->d_name)) 
        {
            char childPath[1024];
            sprintf(childPath, "%s/%s", path, entry->d_name);
            
            //if entry is a file   
            if(entry->d_type == DT_REG)
            {
                //print full path
                printf("%s\n", childPath);
            }
            //if entry is a directory
            else if(entry->d_type == DT_DIR)
            {
                //keep searching
                find(childPath, level + 1);
            }
        }
    }
    //close directory
    closedir(directory);
}

int main() 
{
    find(".", 0);
    return 0;
}
