#include "gamepadEventHandler.h"


void gamepadEventHandler(const char *mq_name, const char *devName){

	//Opens existing message queue - created in gamepadHandler()
	mqd_t mq = mq_open(mq_name, O_WRONLY | O_NONBLOCK);

	//Opens gamepad pipe in blocking mode
	int devFd = open(strcat("/dev/input/", devName), O_RDONLY );

	struct js_event jse;
	char *debugOutput;
	gamepad_event_t event;
	
	//TODO: Stop if gamepad is disconnected or sigint is recieved
	while(errno != EBADF){
		
		//wait until pipe has data
		int bytes;
		if(bytes = read(devFd, &jse, sizeof(jse)) != sizeof(jse)){
			printf("Unexpected bytes from gamepad:%d\n", bytes);
			continue;
		}
		
		jse.type &= ~JS_TYPE_INIT;

		
	}
}

/* event_msg_t createMsg(){ */

/* 			if(jse.value == JS_STATE_RELEASE) */
/* 			event.type = RELEASE; */
/* 		else */
/* 			event.type = PRESS; */


/* 		if(jse.type == JS_TYPE_BUTTON){ */

/* 			switch(jse.number){ */

/* 			case BUTTON_A: */
/* 				debugOutput = "You pressed the A button\n"; */
/* 				event.name = BUTTON_A; */
/* 				break; */
/* 			case BUTTON_B: */
/* 				debugOutput = "You pressed the B button\n"; */
/* 				event.name = BUTTON_B; */
/* 				break; */
/* 			case BUTTON_X: */
/* 				debugOutput = "You pressed the X button\n"; */
/* 				event.name = BUTTON_X; */
/* 				break; */
/* 			case BUTTON_Y: */
/* 				debugOutput = "You pressed the Y button\n"; */
/* 				event.name = BUTTON_Y; */
/* 				break; */
/* 			case BUTTON_LB: */
/* 				debugOutput = "You pressed the LB button\n"; */
/* 				event.name = BUTTON_LB; */
/* 				break; */
/* 			case BUTTON_RB: */
/* 				debugOutput = "You pressed the RB button\n"; */
/* 				event.name = BUTTON_RB; */
/* 				break; */
/* 			case BUTTON_SELECT: */
/* 				debugOutput = "You pressed the SELECT button\n"; */
/* 				event.name = BUTTON_SELECT; */
/* 				break; */
/* 			case BUTTON_START: */
/* 				debugOutput = "You pressed the START button\n"; */
/* 				event.name = BUTTON_START; */
/* 				break; */

/* 			} */
/* 		} */
/* 		else if(jse.type == JS_TYPE_AXIS){ */

/* 			if(jse.number == JS_NUMBER_AXIS_Y){ */

/* 				if(jse.value < 0){ */
/* 					debugOutput = "You pressed the DPAD UP\n"; */
/* 					event.name = DPAD_UP; */
/* 				} */
/* 				else if(jse.value > 0){ */
/* 					debugOutput = "You pressed the DPAD DOWN\n"; */
/* 					event.name = DPAD_DOWN; */
/* 				} */
/* 			} */
/* 			else if(jse.number == JS_NUMBER_AXIS_X){ */

/* 				if(jse.value < 0){ */
/* 					debugOutput = "You pressed the DPAD LEFT\n"; */
/* 					event.name = DPAD_LEFT; */
/* 				} */
/* 				else if(jse.value > 0){ */
/* 					debugOutput = "You pressed the DPAD RIGHT\n"; */
/* 					event.name = DPAD_RIGHT; */
/* 				} */

/* 			} */
/* 		} */


/* } */







/* int readEvent(struct js_event *jse){ */

/* 	return -1; */
/* } */

/* void closeDevice(){ */
/* 	close(devFd); */
/* } */
/* int open_joystick(){ */
/* 	return open(devPath + devName, O_RDONLY | O_NONBLOCK); */
/* } */
