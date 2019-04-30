#include <pthread.h>
#include <mqueue.h>
#include <signal.h>
#include "gamepadEventHandler.h"
#include "msgque.h"

void *gamepadHandler(void *unused);

volatile int interrupt_received = 0;
static void InterruptHandler(int signo) {
	interrupt_received = 1;
}



int main(int argc, void * argv[]){
	

	//Set signal handlers
	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);

	
	mqd_t mq;
	int gph_execution_flag = 1;
	pthread_t gamepadThread;

	
	args_t args;
	args.mq = NULL;
	args.devPath = NULL;
	args.thread_execution_flag = &gph_execution_flag;
	
	
	pthread_create(&gamepadThread, NULL, gamepadHandler, &args);

	//wait for gamepadHandler to create thread
	sleep(2);
	
	mq = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
	if (mq == -1) {
		puts("message queue not available");
		perror("");
		exit(1);
	}

	mq_msg_t msg;

	while(interrupt_received != 1){
		if(mq_receive(mq, (char*)&msg, sizeof(msg), NULL) != -1) {

			if(msg.type == DATA_TYPE_EVENT){
				button_event_t event = msg.data.event;
				
				if(event.type == GP_EVENT_BUTTON){

					printf("Player%c ", msg.dev[strlen(msg.dev)-1]);

					//event.value indicates a press/unpress
					if(event.value == 1)
						printf("pressed ");
					else if (event.value == 0)
						printf("unpressed ");

					//event.name indicates the button
					switch(event.name){

					case BUTTON_A:
						printf("A\n");
						break;
					case BUTTON_B:
						printf("B\n");
						break;
					case BUTTON_X:
						printf("X\n");
						break;
					case BUTTON_Y:
						printf("Y\n");
						break;
					case BUTTON_LB:
						printf("LB\n");
						break;
					case BUTTON_RB:
						printf("RB\n");
						break;
					case BUTTON_START:
						printf("START\n");
						break;
					case BUTTON_SELECT:
						printf("SELECT\n");
						break;
					}

				
				}else if(event.type == GP_EVENT_AXIS){

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


	printf("Halting gamepadHandler...\n");
	gph_execution_flag = 0;
	pthread_join(gamepadThread, NULL);

	
	printf("Closing message queue...\n");
	mq_close(mq);
	exit(1);


}
