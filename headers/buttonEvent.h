#ifndef COMMON_H
#define COMMON_H

typedef enum {

	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_LB,
	BUTTON_RB,
	BUTTON_SELECT,
	BUTTON_START,
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT

}buttonName;

typedef enum {

	PRESS,
	RELEASE

}pressType;

typedef struct {

	buttonName name;
	pressType type;

}gamepad_event_t;

#endif
