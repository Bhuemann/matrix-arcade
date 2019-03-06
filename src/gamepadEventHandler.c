#include <sys/ioctl.h>
#include <linux/joystick.h>
#include "../headers/gamepadEventHandler.h"
#include "../headers/msgque.h"

//Does not work quite yet
/* int sendMsg(mqd_t *mq, mq_msg_t *msg, int prio){ */

/* 	if(mq_send(*mq, (const char*)msg, sizeof(msg), prio) < 0){ */
/* 		//TODO error handling; errno should be set here */
/* 		// ignore errors: fine to let packets be dropped if queue fills */
/* 	} */


/* } */

void gamepadEventHandler(void *args){

	args_t *params = (args_t*)args;
	struct js_event jse;
	int devFd;

	mq_msg_t msg;
	button_event_t event;
	js_attr_t attr;

	int  gp_num_buttons = 0;
	int  gp_num_axis = 0;
	int  gp_version = 0;
	char gp_identifier[80];

		
	//Opens gamepad pipe in nonblocking mode
	if ((devFd = open(params->devPath, O_RDONLY | O_NONBLOCK)) < 0) {
		printf("Error opening device '%s'\n", params->devPath);
		*(params->runningFlag) = FALSE;
		return;
	}


	//Grab gamepad attributes for init msg
	ioctl(devFd, JSIOCGAXES, &gp_num_axis);
	ioctl(devFd, JSIOCGBUTTONS, &gp_num_buttons);
	ioctl(devFd, JSIOCGVERSION, &gp_version);
	ioctl(devFd, JSIOCGNAME(80), &gp_identifier);


	attr.gp_num_buttons = gp_num_buttons;
	attr.gp_num_axis = gp_num_axis;
	attr.gp_version = gp_version;
	strcpy(attr.gp_identifier, gp_identifier);
	strcpy(attr.gp_dev_name, params->devPath);
	
	msg.type = DATA_TYPE_INIT;
	strcpy(msg.dev, params->devPath);
	msg.data.init = attr;

	//sendMsg(params->mq, &msg, 1);

	if(mq_send(*(params->mq), (const char*)&msg, sizeof(msg), 1) < 0){
		//TODO error handling; errno should be set here
		// ignore errors: fine to let packets be dropped if queue fills
	}

	
	
	//TODO: Stop if gamepad is disconnected & send warning or sigint is recieved
	// done: gamepad should clear runningFlag and closes fd if open
	while(*(params->haltFlag) == FALSE){
		
		int bytes;
		if((bytes = read(devFd, &jse, sizeof(jse))) != sizeof(jse)){
		   
			if(bytes == -1){                    /*Error: check errno */

				switch(errno){
				case EAGAIN:
					usleep(20000);
					continue;
				case ENODEV:
					printf("WARN: %s removed\n", params->devPath);
					msg.type = DATA_TYPE_WARNING;
					msg.data.warnmsg = WARNING_GP_DISCONECT;
					//sendMsg(params->mq, &msg, 1);
					break;
				default:
					perror("Error: ");
				}
			}
			else{
				printf("Error: unexpected bytes from %s:%d\n",params->devPath, bytes);
			}

			*(params->runningFlag) = FALSE;
			close(devFd);
			return;
			
		}
		
		// ignore button initializations for now? Eric testing LED matrix game menu
		// is there a use for these in the future or keep ignoring them?
		if (jse.type & 0x80)
			continue;
		//jse.type &= ~JS_EVENT_INIT;

		if (jse.type == GP_EVENT_AXIS) {
			switch (jse.number) {
			case GP_AXIS_X1:
				event.name = AXIS_X1;
				break;
			case GP_AXIS_Y1:
				event.name = AXIS_Y1;
				break;
			case GP_AXIS_X2:
				event.name = AXIS_X2;
				break;
			case GP_AXIS_Y2:
				event.name = AXIS_Y2;
				break;
			}
			
		} else if (jse.type == GP_EVENT_BUTTON) {
			event.name = jse.number;
		}

		event.timestamp = jse.time;
		event.type = jse.type;
		event.value = jse.value;

		//Create message and send
		msg.type = DATA_TYPE_EVENT;
		msg.data.event = event;
		//sendMsg(params->mq, &msg, 1);

		if(mq_send(*(params->mq), (const char*)&msg, sizeof(msg), 1) < 0){
			//TODO error handling; errno should be set here
			// ignore errors: fine to let packets be dropped if queue fills
		}

		
			
		
	}

	*(params->runningFlag) = FALSE;
	close(devFd);
	
}
