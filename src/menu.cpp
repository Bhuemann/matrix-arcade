
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <mqueue.h>
#include <signal.h>
#include <pthread.h>
#include "led-matrix.h"


#include "gamepadHandler.h"
#include "gamepadEventHandler.h"
#include "menu.h"
#include "msgque.h"

#define MATRIX_ROOT_DIR "./matrix_root"

using rgb_matrix::RGBMatrix;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
	interrupt_received = true;
}


Menu::Menu(char* path){

	this->path = path;
	this->lastEntryIndex = 0;

	DIR *dir = opendir(path);
	if (dir == NULL) {
		printf("Could not open current directory" );
		exit(0);
	}

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL){

		//File is a directory
		if (entry->d_type == DT_DIR){
			if(strcmp("..", entry->d_name) == 0 || strcmp(".", entry->d_name) == 0){
				continue;
			}
			
			entries[lastEntryIndex++] = strdup(entry->d_name);
		}
	}

	closedir(dir);	

}

Menu::~Menu(){

	for(int i = 0; i<lastEntryIndex; i++){
		free(entries[i]);
	}

	
}


int main(int argc, char **argv)
{

	//Set signal handlers
	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);


	//Start GamepadHander thread
	mqd_t mq;
	int runningFlag = 0;
	int haltFlag = 0;
	pthread_t gamepadThread;

	args_t args;
	args.mq = NULL;
	args.devPath = NULL;
	args.haltFlag = &haltFlag;
	args.runningFlag = &runningFlag;
	pthread_create(&gamepadThread, NULL, gamepadHandler, &args);


	//Open message queue for reading
	mq = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK);
	if (mq == -1) {
		printf("Could not open message queue\n");
		haltFlag = 1;
		pthread_join(gamepadThread, NULL);
		exit(1);
	}
	
   
	//Create RGBMatrix
	RGBMatrix::Options defaults;
	rgb_matrix::RuntimeOptions runtime_opt;
	
	defaults.hardware_mapping = "adafruit-hat-pwm";  // or e.g. "adafruit-hat"
	defaults.rows = 32;
	defaults.chain_length = 4;
	defaults.parallel = 1;
	defaults.pixel_mapper_config = "U-mapper;Rotate:180";

	
	if (!ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt)) {
		return usage(argv[0]);
	}
	
	RGBMatrix *matrix = CreateMatrixFromOptions(matrix_options, runtime_opt);

	
	//Start Menu at matrix root
	menuHandler(RGBMatrix* m, char* MATRIX_ROOT_DIR);


	//Cleanup
	printf("Deleting RGBMatrix...\n");
	matrix->Clear();
	delete matrix;

	
	printf("Closing message queue...\n");
	mq_close(mq);
	exit(1);

	
	printf("Halting gamepadHandler...\n");
	haltFlag = 1;
	pthread_join(gamepadThread, NULL);

	printf("Done\n");
	
}

//Recursively handle menu & sub-menus
int menuHandler(RGBMatrix* m, char* path){


	Menu *m = new Menu(m, path);

	
}
