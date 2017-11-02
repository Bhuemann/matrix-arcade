#include "../headers/gamepadEventHandler.h"
#include "../headers/msgque.h"

void clearRunningFlag(void *args){
	*(((args_t*)args)->runningFlag) = FALSE;
}

void gamepadEventHandler(void *args){

	struct js_event jse;
	button_event_t event;
	mq_msg_t msg;
	
	//Opens gamepad pipe in blocking mode
	int devFd = open(((args_t*)args)->devPath, O_RDONLY);
	
	//Check if devFd is valid
	if (devFd < 0) {
		//printf("open failed.\n");
		clearRunningFlag(args);
		return;
	}

	//TODO: add ioctls to grab gamepad attributes for init msg
	
	//TODO: Stop if gamepad is disconnected & send warning or sigint is recieved
	// done: gamepad should clear runningFlag and closes fd if open
	while(errno != ENODEV){
		
		//wait until pipe has data
		int bytes;
		if(bytes = read(devFd, &jse, sizeof(jse)) != sizeof(jse)){
			printf("Unexpected bytes from gamepad:%d\n", bytes);
			puts("Gamepad removed");
			clearRunningFlag(args);
			close(devFd);
			continue;
		}
		
		// ignore button initializations for now? Eric testing LED matrix game menu
		// is there a use for these in the future or keep ignoring them?
		if (jse.type & 0x80)
			continue;
		//jse.type &= ~JS_EVENT_INIT;

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

		//Create message and send
		msg.type = DATA_TYPE_EVENT;
		msg.data.event = event;

		int status;
		status = mq_send(*(((args_t*)args)->mq), (const char*)&msg, sizeof(msg), 1);

		if(status != 0){
			//TODO error handling; errno should be set here
			// ignore errors: fine to let packets be dropped if queue fills
		}
		
	}
}

/* void gamepadEventHandlerCleanup(int devFd, int *runningFlag){ */

/* 	close(devFd); */
/* 	*runningFlag = FALSE; */
	
/* } */
