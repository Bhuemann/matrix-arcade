#include "gamepadEventHandler.h"
#define MAX_DEV_PATH 16;
#define DEV_PATH "/dev/input/"

void gamepadEventHandler(const char *mq_name, const char *devName){

	//Opens existing message queue - created in gamepadHandler()
	mqd_t mq = mq_open(mq_name, O_WRONLY | O_NONBLOCK);

	struct js_event jse;
	button_event_t event;
	
	char *devPath = malloc(strlen(DEV_PATH) + strlen(devName));
	strcat(devPath, DEV_PATH);
	strcat(devPath, devName);

						   
	//Opens gamepad pipe in blocking mode
	int devFd = open(devPath, O_RDONLY);
	
	//Check if devFd is valid
	if (devFd < 0) {
		printf("open failed.\n");
		exit(1);
	}

	//TODO: maybe ioctls to interrogate features here?
	
	//TODO: Stop if gamepad is disconnected & send warning or sigint is recieved
	while(errno != ENODEV){
		
		//wait until pipe has data
		int bytes;
		if(bytes = read(devFd, &jse, sizeof(jse)) != sizeof(jse)){
			printf("Unexpected bytes from gamepad:%d\n", bytes);
			continue;
		}
		
		jse.type &= ~JS_TYPE_INIT;

		if (jse.type == JS_EVENT_AXIS) {
			switch (jse.number) {
			case JS_AXIS_X1:
				event.name = AXIS_X1;
				break;
			case JS_AXIS_Y1:
				event.name = AXIS_Y1;
				break;
			case JS_AXIS_X2:
				event.name = AXIS_X2;
				break;
			case JS_AXIS_Y2:
				event.name = AXIS_Y2;
				break;
			}
			
		} else if (jse.type == JS_EVENT_BUTTON) {
			event.name = jse.number;
		}

		event.timestamp = jse.time;
		event.type = jse.type;
		event.value = jse.value;


		//TODO: mq send event
		
	}

	free(devPath);
}

