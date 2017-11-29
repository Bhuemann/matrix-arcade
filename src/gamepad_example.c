#include <pthread.h>
#include <mqueue.h>
#include <signal.h>
#include "gamepadHandler.h"
#include "gamepadEventHandler.h"
#include "msgque.h"

mqd_t mq;
int runningFlag = 0;
int haltFlag = 0;
pthread_t gamepadThread;

void onClose(int signo){

	printf("Halting gamepadHandler...\n");
	haltFlag = 1;
	pthread_join(gamepadThread, NULL);

	
	printf("Closing message queue...\n");
	mq_close(mq);
	exit(1);
}


int main(int argc, void * argv[]){
	
	
	args_t args;
	args.mq = NULL;
	args.devPath = NULL;
	args.haltFlag = &haltFlag;
	args.runningFlag = &runningFlag;
	
	
	pthread_create(&gamepadThread, NULL, gamepadHandler, &args);
	signal(SIGINT, onClose);

	//wait for gamepadHandler to create thread
	usleep(2000);

	mq = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK);
	if (mq == -1) {
		puts("message queue not available");
		perror("");
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
						printf("Player%c pressed A\n", msg.dev[strlen(msg.dev)-1]);
						break;
					case BUTTON_B:
						printf("Player%c pressed B\n", msg.dev[strlen(msg.dev)-1]);
						break;
					case BUTTON_X:
						printf("Player%c pressed X\n", msg.dev[strlen(msg.dev)-1]);
						break;
					case BUTTON_Y:
						printf("Player%c pressed Y\n", msg.dev[strlen(msg.dev)-1]);
						break;
					case BUTTON_LB:
						printf("Player%c pressed LB\n", msg.dev[strlen(msg.dev)-1]);
						break;
					case BUTTON_RB:
						printf("Player%c pressed RB\n", msg.dev[strlen(msg.dev)-1]);
						break;
					case BUTTON_START:
						printf("Player%c pressed START\n", msg.dev[strlen(msg.dev)-1]);
						break;
					case BUTTON_SELECT:
						printf("Player%c pressed SELECT\n", msg.dev[strlen(msg.dev)-1]);
						break;
					}

				
				}else if(event.type == JS_EVENT_AXIS && event.value != 0){

					if(event.name == AXIS_X1){
					        printf("Player%c moved axis x1 with value of %d\n", msg.dev[strlen(msg.dev)-1], event.value);
					}
					else if(event.name == AXIS_Y1){
					        printf("Player%c moved axis y1 with value of %d\n", msg.dev[strlen(msg.dev)-1], event.value);
					}
					else if(event.name == AXIS_X2){
						printf("Player%c moved axis x2 with value of %d\n", msg.dev[strlen(msg.dev)-1], event.value);
					}
					else if(event.name == AXIS_Y2){
						printf("Player%c moved axis y2 with value of %d\n", msg.dev[strlen(msg.dev)-1], event.value);
					}
					
				
				}
			
			}
			else if(msg.type == DATA_TYPE_INIT){

				printf("\nInit data recieved...\n");
				printf("Gamepad identifier: %s\n", msg.data.init.gp_identifier);
				printf("Gamepad device: %s\n", msg.data.init.gp_dev_name);
				printf("Gamepad version: %d\n", msg.data.init.gp_version);
				printf("Gamepad number of axis: %d\n", msg.data.init.gp_num_axis);
				printf("Gamepad number of button: %d\n", msg.data.init.gp_num_buttons);
				
				
			}
		}

	}
}
