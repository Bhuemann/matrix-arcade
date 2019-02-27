
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <mqueue.h>

#include "menu.h"
#include "msgque.h"

#define MATRIX_ROOT_DIR "./matrix_root"


char** getDirNames(char* path, int* numDirs){

	char ** dirs;
	int index = 0;
	
	struct dirent *de;  // Pointer for directory entry

	// opendir() returns a pointer of DIR type.
	DIR *dir = opendir(MATRIX_ROOT_DIR);

	if (dir == NULL) {
		printf("Could not open current directory" );
		return 0;
	}

	while ((de = readdir(dr)) != NULL){

		//File is a directory
		if (entry->d_type == DT_DIR){

			if(strcmp("..", de->d_name) == 0 || strcmp(".", de->dename) == 0){
				continue;
			}

			dirs[index++] = strdup(de->d_name);
		}
	}

	closedir(dr);

	*numDirs = index + 1;
	return dirs; 

	
}

struct dirent* nextDir(DIR *dir){

		while ((entry = readdir(dir)) != NULL){

			//File is a directory
			if (entry->d_type == DT_DIR){

				if(strcmp("..", entry->d_name) == 0 || strcmp(".", entry->dename) == 0){
					continue;
				}

				return entry;
			}
		}

		return NULL;
	
}

int menu(char* path){


	DIR *dir = opendir(path);
	struct dirent *entry;  // Pointer for directory entry

	if (dir == NULL) {
		printf("Could not open current directory" );
		return 0;
	}

	


	

	closedir(dr);
	return 0; 
	
}

int main(int argc, char **argv)
{

  
}
