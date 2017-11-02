#ifndef GAMEPADEVENTHANDLER_H
#define GAMEPADEVENTHANDLER_H

#include <errno.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */

#define JS_BUTTON_A            0x00
#define JS_BUTTON_B            0x01
#define JS_BUTTON_X            0x02
#define JS_BUTTON_Y            0x03
#define JS_BUTTON_LB           0x04
#define JS_BUTTON_RB           0x05
#define JS_BUTTON_SELECT       0x06
#define JS_BUTTON_START        0x07

#define JS_AXIS_X1             0x00
#define JS_AXIS_Y1             0x01
#define JS_AXIS_X2             0x02
#define JS_AXIS_Y2             0x03

#define TRUE 1
#define FALSE 0

typedef struct args_t {
	mqd_t *mq;
	char *devPath;
	int *runningFlag;
}args_t;

struct js_event {
	unsigned int time;      /* event timestamp in milliseconds */
	short value;            /* value */
	unsigned char type;     /* event type */
	unsigned char number;   /* axis/button number */
};

typedef struct js_attr_t{

	char button_cnt;
	char axis_cnt;
	int version;
	char identifier[16];
	char dev_name[16];
	
}js_attr_t;

typedef enum buttonName{

	BUTTON_A = JS_BUTTON_A,
	BUTTON_B = JS_BUTTON_B,
	BUTTON_X = JS_BUTTON_X,
	BUTTON_Y = JS_BUTTON_Y,
	BUTTON_LB = JS_BUTTON_LB,
	BUTTON_RB = JS_BUTTON_RB,
	BUTTON_SELECT = JS_BUTTON_SELECT,
	BUTTON_START = JS_BUTTON_START,
	AXIS_X1,
	AXIS_Y1,
	AXIS_X2,
	AXIS_Y2

}buttonName;

typedef enum buttonType{
	AXIS = JS_EVENT_AXIS,
	BUTTON = JS_EVENT_BUTTON
}buttonType;
	

typedef struct {

	unsigned int timestamp;
	buttonName name;
	buttonType type;
	short value;
	
}button_event_t;

void gamepadEventHandler(void *args);

#endif
