#include <pthread.h>
#include <mqueue.h>
#include "../headers/gamepadHandler.h"
#include "../headers/gamepadEventHandler.h"
#include "../headers/msgque.h"

#define FALSE 0
#define TRUE 1

// gamepad
#define AXISPOSIVE (32767)
#define AXISNEGATIVE (-32767)

//direction
#define STOP 0
#define UP 	1
#define RIGHT	2
#define DOWN	3
#define LEFT	4

// number of tails
#define NTAIL 100

void printsk(int X, int Y, int FruitX, int FruitY, int *tailX, int *tailY, int nTail, int WIDTH, int HEIGHT);

const int WIDTH = 32;
const int HEIGHT = 32;

int X, Y, FruitX, FruitY, Score;
int Over;
int DIR;

/*int TailX[WIDTH*HEIGHT];*/
/*int TailY[WIDTH*HEIGHT];*/
int TailX[NTAIL];
int TailY[NTAIL];
int nTail;

void setup() {
    //init screen
    //clear screen
    // set handle input
    //initscr();
    //clear();
    //cbreak();
    //curs_set(0);

    Over = FALSE;
    DIR = STOP;
    X = WIDTH/2;
    Y = HEIGHT/2;
    FruitX = (rand() % WIDTH);
    FruitY = (rand() % HEIGHT);
    Score = 0;

    // init tail number and tails arrays
    nTail = 0;
    for (int i = 0; i < NTAIL; i++) {
        TailX[i] = 0;
        TailY[i] = 0;
    }

}

/*void draw() {*/
    /*// clear the screen*/

    /*for (int i = 0; i < WIDTH; i++) {*/
        /*for (int j = 0; j < HEIGHT; j++) {*/
            /*if (i == Y && j == X) {*/
                /*// set the Head of snake to read*/
            /*} else if (i == FruitY && j == FruitX) {*/
                /*// set the fruit pixel to green*/
            /*} else*/
                /*for (int k = 0; k < nTail; k++) {*/
                    /*if (i == TailY[k] && j == TailX[k]) {*/
                        /*// set the tails to blue*/
                    /*}*/
                /*}*/
        /*}*/
    /*}*/

    /*// refresh*/

    /*// get next input from handle*/


    /*// clear screen*/
/*}*/

void input(mqd_t mq) {

	mq_msg_t msg;
    // check receive message queue or not
    if(mq_receive(mq, (char*)&msg, sizeof(msg), NULL) != -1) {

        if(msg.type == DATA_TYPE_EVENT){
            button_event_t event = msg.data.event;

            // event.value = 1 : press the button
            // event.value = 0 : release the button
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


            /*} else if(event.type == JS_EVENT_AXIS && event.value == 1) {*/
            // event.value = 1 : press the axis
            // event.value = 0 : release the axis
            } else if(event.type == JS_EVENT_AXIS && event.value != 0) {

                if(event.name == AXIS_X1){
                    if (event.value == AXISNEGATIVE) {
                        /*printf("LEFT moved axis x1 with value of %d\n", event.value);*/
                        DIR = LEFT;
                        /*DIR = UP;*/
                    } else if (event.value == AXISPOSIVE) {
                        /*printf("RIGHT moved axis x1 with value of %d\n", event.value);*/
                        DIR = RIGHT;
                    }
                }
                else if(event.name == AXIS_Y1){
                    if (event.value == AXISNEGATIVE) {
                        /*printf("UP moved axis x1 with value of %d\n", event.value);*/
                        DIR = UP;
                    } else if (event.value == AXISPOSIVE) {
                        /*printf("DOWN moved axis x1 with value of %d\n", event.value);*/
                        DIR = DOWN;
                    }
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


void logic() {

    int prevX = TailX[0];
    int prevY = TailY[0];
    int prev2X, prev2Y;
    TailX[0] = X;
    TailY[0] = Y;

    for (int i = 1; i < nTail; i++) {
        prev2X = TailX[i];
        prev2Y = TailY[i];
        TailX[i] = prevX;
        TailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // move the head
    switch(DIR) {
        case LEFT:
            Y++;
            /*X--;*/
            break;
        case RIGHT:
            Y--;
            break;
        case UP:
            X--;
            break;
        case DOWN:
            X++;
            break;
        default:
            break;
    }

    // check hit boundary
    if (X >= WIDTH || X < 0 || Y >= HEIGHT || Y < 0)
        Over = TRUE;

    // eat the fruit
    if (X == FruitX && Y == FruitY) {
        Score++;
        FruitX = ( rand() % WIDTH );
        FruitY = ( rand() % HEIGHT );

        nTail++;
    }

    // head hit body game over
    /*for (int i = 0; i < nTail; i++) {*/
        /*if (TailX[i] == X && TailY[i] == Y) {*/
            /*Over = TRUE;*/
        /*}*/
    /*}*/

}

/*int snake(int argc, void * argv[]){*/
int snake() {

    setup();
	/*pthread_t gamepadThread;*/
	/*pthread_create(&gamepadThread, NULL, gamepadHandler, NULL);*/

	usleep(1000);

	
    // create message queue
	mqd_t mq;
	mq = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK);
	if (mq == -1) {
		puts("message queue not available");
		exit(1);
	}

    while (Over == FALSE) {
        /*draw();*/
        usleep(100000);
        printsk(X, Y, FruitX, FruitY, TailX, TailY, nTail, WIDTH, HEIGHT);
        input(mq);
        logic();
    }

}


