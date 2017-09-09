#ifndef COMMON_H
#define COMMON_H

enum buttonName{

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

};

enum pressType{

	PRESS,
	RELEASE

};

typedef struct {

	buttonName name;
	pressType type;

}button_event_t;

#endif
