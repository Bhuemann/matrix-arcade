#ifndef GAMEPADEVENTHANDLER_H
#define GAMEPADEVENTHANDLER_H

#include "buttonEvent.h"
#include <errno.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define JS_TYPE_BUTTON         0x01    /* button pressed/released */
#define JS_TYPE_AXIS           0x02    /* joystick moved */
#define JS_TYPE_INIT           0x80    /* initial state of device */

#define JS_NUMBER_AXIS_X       0x00    //UP/DOWN
#define JS_NUMBER_AXIS_Y       0x01    //LEFT/RIGHT

#define JS_STATE_RELEASE       0x00

struct js_event {
	unsigned int time;      /* event timestamp in milliseconds */
	short value;            /* value */
	unsigned char type;     /* event type */
	unsigned char number;   /* axis/button number */
};

/* struct wwvi_js_event { */
/* 	int button[11]; */
/* 	int stick_x; */
/* 	int stick_y; */
/* }; */


void gamepadEventHandler(const char *mq_name, const char *devName);


int openDevice();
int closeDevice();
int readEvent(struct js_event *jse);


#endif
