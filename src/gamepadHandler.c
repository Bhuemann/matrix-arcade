

#include <dirent.h>
#include <pthread.h>
#include <unistd.h>

#include "gamepadEventHandler.h"
#include "msgque.h"

#define TRUE         1
#define FALSE        0
#define NUM_DEVICES  4

int gamepadHandler(){

	//Device pipe paths
	char *devices[] = {"/dev/input/js0",
	  				   "/dev/input/js1",
					   "/dev/input/js2",
					   "/dev/input/js3"};

	int *devRunning[NUM_DEVICES];
	pthread_t *devHandler[NUM_DEVICES];


	//Construct message queue
	struct mq_attr mq_gp_attr;
	mqd_t mq;

	
	mq_gp_attr.mq_maxmsg = MAX_MQ_MSGS;
	mq_gp_attr.mq_msgsize = sizeof(mq_msg_t);

	mq = mq_open(MQ_NAME, O_CREAT | O_WRONLY | O_NONBLOCK, 0644, &mq_gp_attr);  
	

	while(1){

		int i;
		for(i = 0; i < NUM_DEVICES; i++){

			if(*devRunning[i] == TRUE && access(devices[i], F_OK) == 0){

				args_t thread_args;
				thread_args.mq = &mq;
				thread_args.devPath = devices[i];
				thread_args.runningFlag = devRunning[i];

				if(pthread_create(devHandler[i], NULL, (void*)&gamepadEventHandler, (void*)&thread_args) != 0) {
					printf("ERROR: failed to create pthread for device: %s\n", devices[i]);
					continue;
				}
				
				*devRunning[i] = TRUE;
			}
		}

		usleep(1000);
		
	}
}
