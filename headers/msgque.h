#ifndef MSGQUE_H
#define MSGQUE_H

#include "gamepadEventHandler.h"

#define MQ_NAME "/mq_gp_event" 
#define MAX_MQ_MSGS 100


//add warnings here
#define WARNING_GP_DISCONECT         0x01;


enum {
	DATA_TYPE_INIT,
	DATA_TYPE_WARNING,
	DATA_TYPE_EVENT
};

typedef union {

	button_event_t event;
	unsigned char warnmsg;
	js_attr_t init;

} mq_data_t;

typedef struct {

	int type;
	mq_data_t data;
	
}mq_msg_t;


#endif
