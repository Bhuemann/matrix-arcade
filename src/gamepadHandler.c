#define _DEFAULT_SOURCE
#include <dirent.h>
#include <pthread.h>
#include <mqueue.h>
#include <unistd.h>

#include "../headers/gamepadEventHandler.h"
#include "../headers/msgque.h"

#define NUM_DEVICES  4
#define MAX_MSG_FILE "/proc/sys/fs/mqueue/msg_max"


//TODO: 
//      Add mutex and thread conditionals for Open() calls explained below
//      Replace Access() call with inotify() calls for better performance
//      Implement error handling

void *gamepadHandler(void *args){

	//Device pipe paths
	char *devices[] = {"/dev/input/js0",
			   "/dev/input/js1",
			   "/dev/input/js2",
			   "/dev/input/js3"};

	int gp_execution_flag[NUM_DEVICES] = {0};
	pthread_t gp_event_thread[NUM_DEVICES];

	args_t *gph_args = (args_t*)args;

	//Construct message queue
	struct mq_attr mq_gp_attr;
	mqd_t mq;

	// read max value system will allow for maxmsg
	// mq_open() will fail if maxmsg bigger than that value
	FILE *fd = fopen(MAX_MSG_FILE, "r");
	if (!fd) {
		mq_gp_attr.mq_maxmsg = 10;
	} else {
		fscanf(fd, "%ld", &mq_gp_attr.mq_maxmsg);
		fclose(fd);
	}
	
	mq_gp_attr.mq_flags = 0;
	mq_gp_attr.mq_msgsize = sizeof(mq_msg_t);
	mq_gp_attr.mq_curmsgs = 0;

	// unlink to remove pre-existing queue if it exists
	mq_unlink(MQ_NAME);
	mq = mq_open(MQ_NAME, O_CREAT | O_WRONLY | O_NONBLOCK, 0644, &mq_gp_attr);

	while(*(gph_args->thread_execution_flag) == 1){

		int i;
		for(i = 0; i < NUM_DEVICES; i++){

			if(gp_execution_flag[i] == FALSE && access(devices[i], F_OK) == 0){

				// open() will fail if it is called on /dev/input/jsX as soon as a controller is plugged in
				// doesn't actually matter because the loop will try again.. up to preference
				//TODO: Implement mutex/thread conditionals here to avoid sleeping
				//usleep(50000);

				args_t thread_args;
				thread_args.mq = &mq;
				thread_args.devPath = devices[i];
				thread_args.thread_execution_flag = &(gp_execution_flag[i]);


				gp_execution_flag[i] = 1;
				if(pthread_create(&gp_event_thread[i], NULL, (void*)&gamepadEventHandler, (void*)&thread_args) != 0) {
					printf("ERROR: failed to create pthread for device: %s\n", devices[i]);
					continue;
				}
			}
		}

		usleep(1000);
		
	}

	//Close down child threads
	int i;
	for(i = 0; i < NUM_DEVICES; i++){
		if(gp_execution_flag[i] == TRUE){
			printf("Stopping %s...\n", devices[i]);
			gp_execution_flag[i] = 0;
			if(pthread_join(gp_event_thread[i], NULL) != 0){
				printf("Error failed to stop thread for device: %s\n", devices[i]);
				//TODO handle thread halt error
			}
		}
	}

	//Close msg queue
	mq_close(mq);
	mq_unlink(MQ_NAME);
	
}
