#include <pthread.h>
#include <mqueue.h>
#include "gamepadHandler.h"
#include "gamepadEventHandler.h"
#include "msgque.h"



int main(int argc, void * argv[]){

	pthread_t gamepadThread;
	pthread_create(&gamepadThread, NULL, gamepadHandler, NULL);

	usleep(1000);
	
	mqd_t mq;
	mq = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK);
	if (mq == -1) {
		puts("message queue not available");
		exit(1);
	}

	mq_msg_t msg;

	while(1){
		if(mq_receive(mq, (char*)&msg, sizeof(msg), NULL) != -1) {

			if(msg.type == DATA_TYPE_EVENT){
				button_event_t event = msg.data.event;

				if(event.type == JS_EVENT_BUTTON && event.value == 1){

					switch(event.name){

					case BUTTON_A:
						printf("You pressed A\n");
						break;
					case BUTTON_B:
						printf("You pressed B\n");
						break;
					case BUTTON_X:
						printf("You pressed X\n");
						break;
					case BUTTON_Y:
						printf("You pressed Y\n");
						break;
					case BUTTON_LB:
						printf("You pressed LB\n");
						break;
					case BUTTON_RB:
						printf("You pressed RB\n");
						break;
					case BUTTON_START:
						printf("You pressed START\n");
						break;
					case BUTTON_SELECT:
						printf("You pressed SELECT\n");
						break;
					}

				
				}else if(event.type == JS_EVENT_AXIS){

					if(event.name == AXIS_X1){
						printf("You moved axis x1 with value of %d\n", event.value);
					}
					else if(event.name == AXIS_Y1){
						printf("You moved axis y1 with value of %d\n", event.value);
					}
					else if(event.name == AXIS_X2){
						printf("You moved axis x2 with value of %d\n", event.value);
					}
					else if(event.name == AXIS_X2){
						printf("You moved axis x2 with value of %d\n", event.value);
					}
					
				
				}
			
			
			}
		
		}

	}
}
