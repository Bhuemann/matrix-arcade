#define _DEFAULT_SOURCE
#include <thread>
#include <string>

#include <dirent.h>
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

using namespace std;

void gamepadHandler(bool &execution_flag){

	//Device pipe paths
	string devices[] = {"/dev/input/js0",
			   	   	   "/dev/input/js1",
					   "/dev/input/js2",
					   "/dev/input/js3"};

	bool gp_execution_flag[NUM_DEVICES] = {false};
	thread gp_event_thread[NUM_DEVICES];

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

	while(execution_flag){

		int i;
		for(i = 0; i < NUM_DEVICES; i++){

			if( !gp_execution_flag[i] && access(devices[i].c_str(), F_OK) == 0 ){

				// open() will fail if it is called on /dev/input/jsX as soon as a controller is plugged in
				// doesn't actually matter because the loop will try again.. up to preference
				//TODO: Implement mutex/thread conditionals here to avoid sleeping
				//usleep(50000);

				gp_execution_flag[i] = true;
				gp_event_thread[i] = thread(gamepadEventHandler, devices[i].c_str(), ref(mq), ref(gp_execution_flag[i]));
			}
		}

		usleep(1000);
		
	}

	//Close down child threads
	int i;
	for(i = 0; i < NUM_DEVICES; i++){
		if(gp_execution_flag[i]){
			printf("Stopping %s...\n", devices[i]);
			gp_execution_flag[i] = false;
			gp_event_thread[i].join();
		}
	}

	//Close msg queue
	mq_close(mq);
	mq_unlink(MQ_NAME);
	
}
