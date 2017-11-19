#include <dirent.h>
#include <pthread.h>
#include <mqueue.h>
#include <unistd.h>

#include "../headers/gamepadEventHandler.h"
#include "../headers/msgque.h"

#define NUM_DEVICES  4
#define MAX_MSG_FILE "/proc/sys/fs/mqueue/msg_max"

//int usleep(unsigned long);

void *gamepadHandler(void *unused){

	//Device pipe paths
	char *devices[] = {"/dev/input/js0",
	  				   "/dev/input/js1",
					   "/dev/input/js2",
					   "/dev/input/js3"};

	int devRunning[NUM_DEVICES] = {};
	pthread_t devHandler[NUM_DEVICES];


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

	while(1){

		int i;
		for(i = 0; i < NUM_DEVICES; i++){

			if(devRunning[i] == FALSE && access(devices[i], F_OK) == 0){

				// open() will fail if it is called on /dev/input/jsX as soon as a controller is plugged in
				// doesn't actually matter because the loop will try again.. up to preference
				usleep(50000);
				//printf("gamepad %d plugged in\n", i);
				args_t thread_args;
				thread_args.mq = &mq;
				thread_args.devPath = devices[i];
				thread_args.runningFlag = &devRunning[i];

				devRunning[i] = TRUE;
				if(pthread_create(&devHandler[i], NULL, (void*)&gamepadEventHandler, (void*)&thread_args) != 0) {
					//printf("ERROR: failed to create pthread for device: %s\n", devices[i]);
					continue;
				}
			}
		}

		usleep(1000);
		
	}
}
