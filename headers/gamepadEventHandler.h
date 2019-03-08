#ifndef GAMEPADEVENTHANDLER_H
#define GAMEPADEVENTHANDLER_H

#include <errno.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define GP_EVENT_BUTTON         0x01    /* button pressed/released */
#define GP_EVENT_AXIS           0x02    /* joystick moved */
#define GP_EVENT_INIT           0x80    /* initial state of device */

#define GP_BUTTON_A            0x00
#define GP_BUTTON_B            0x01
#define GP_BUTTON_X            0x02
#define GP_BUTTON_Y            0x03
#define GP_BUTTON_LB           0x04
#define GP_BUTTON_RB           0x05
#define GP_BUTTON_SELECT       0x06
#define GP_BUTTON_START        0x07

#define GP_AXIS_X1             0x00
#define GP_AXIS_Y1             0x01
#define GP_AXIS_X2             0x02
#define GP_AXIS_Y2             0x03

#define TRUE 1
#define FALSE 0

typedef struct args_t {
	mqd_t *mq;
	char *devPath;
	int *thread_execution_flag;
}args_t;

/* struct js_event { */
/* 	unsigned int time;      /\* event timestamp in milliseconds *\/ */
/* 	short value;            /\* value *\/ */
/* 	unsigned char type;     /\* event type *\/ */
/* 	unsigned char number;   /\* axis/button number *\/ */
/* }; */

typedef struct js_attr_t{


	int  gp_num_buttons;
	int  gp_num_axis;
	int  gp_version;
	char gp_identifier[80];
	char gp_dev_name[16];
	
}js_attr_t;

typedef enum buttonName{

	BUTTON_A = GP_BUTTON_A,
	BUTTON_B = GP_BUTTON_B,
	BUTTON_X = GP_BUTTON_X,
	BUTTON_Y = GP_BUTTON_Y,
	BUTTON_LB = GP_BUTTON_LB,
	BUTTON_RB = GP_BUTTON_RB,
	BUTTON_SELECT = GP_BUTTON_SELECT,
	BUTTON_START = GP_BUTTON_START,
	AXIS_X1,
	AXIS_Y1,
	AXIS_X2,
	AXIS_Y2

}buttonName;

typedef enum buttonType{
	AXIS = GP_EVENT_AXIS,
	BUTTON = GP_EVENT_BUTTON
}buttonType;
	

typedef struct {

	unsigned int timestamp;
	buttonName name;
	buttonType type;
	short value;
	
}button_event_t;

void gamepadEventHandler(void *args);

#endif
