
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <signal.h>
#include <mqueue.h>

#include "snake.h"
#include "msgque.h"

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
	interrupt_received = true;
}


Segment::Segment(Point pt, Color c, Segment *prev, Segment *next){

	this->x = pt.x;
	this->y = pt.y;
	this->c = c;
	this->prev = prev;
	this->next = next;
	
}
Segment::Segment(Segment *obj){

	this->x = obj->x;
	this->y = obj->y;
	this->c = obj->c;
	this->next = obj;
	this->prev = nullptr;
}




Snake::Snake(Point pt, Color c, direction bearing){

	head = new Segment(pt, c, nullptr, nullptr);
	tail = head; 
	growth = 0;
	size = 1;
	this->bearing = bearing;
	
}

Snake::~Snake(){

	Segment* ptr = head;
	Segment* prev;
	while(ptr != nullptr){
		prev = ptr;
		ptr = ptr->next;
		delete prev;
	}
	
}

Point Snake::move(){

	Segment *newHead = new Segment(head);

	switch(bearing){
	case NORTH:
		newHead->y--;
		break;
	case EAST:
		newHead->x++;
		break;
	case SOUTH:
		newHead->y++;
		break;
	case WEST:
		newHead->x--;
	}

	head->prev = newHead;
	head = newHead;

	if(growth <= 0){
		Segment *tmp = tail;
		tail = tmp->prev;
		tail->next = nullptr;
		delete tmp;
	}else{
		growth--;
		size++;
	}

	return Point(newHead->x, newHead->y);
}

void Snake::setDirection(direction d){
	this->bearing = d;
}

direction Snake::getDirection(){
	return this->bearing;
}


bool Snake::isCollision(Point pt){

	//TODO find solution using this function with own head...Meantime just exclude head segment.
	Segment *ptr = head->next;
	while(ptr != nullptr){
		if(ptr->x == pt.x && ptr->y == pt.y){
			return true;
		}else{
			ptr = ptr->next;
		}
	}

	return false;
}

void Snake::grow(int n){
	growth += n;
}

void Snake::draw(FrameCanvas* canvas){

	Segment *ptr = head;
	while(ptr != nullptr){
		canvas->SetPixel(ptr->x, ptr->y, ptr->c.r, ptr->c.g, ptr->c.b);
		ptr = ptr->next;
	}
}



int main(int argc, char** argv){

	//Set signal handlers
	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);
	
	//Open message queue for reading
	mqd_t mq = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK);
	if (mq == -1) {
		printf("Could not open message queue\n");
		exit(1);
	}
	
   
	//Create RGBMatrix
	RGBMatrix::Options matrix_options;
	rgb_matrix::RuntimeOptions runtime_opt;
	
	matrix_options.hardware_mapping = "adafruit-hat-pwm";  // or e.g. "adafruit-hat"
	matrix_options.rows = 32;
	matrix_options.chain_length = 4;
	matrix_options.parallel = 1;
	matrix_options.pixel_mapper_config = "U-mapper;Rotate:180";

	/* 
     * Get options from cmd line...we will create our own for now
     *
	if (!ParseOptionsFromFlags(&argc, &argv, &matrix_options)) {
		return usage(argv[0]);
	}
	*/

	RGBMatrix *m = CreateMatrixFromOptions(matrix_options, runtime_opt);

	
	//Set Color and font
	Color c;
	c.r = 255;
	c.g = 255;
	c.b = 255;
	
	Font font;
	if (!font.LoadFont(BDF_FONT_FILE)) {
		fprintf(stderr, "Couldn't load font '%s'\n", BDF_FONT_FILE);
		return 1;
	}

	
	snakeGame(m, mq);


	//Cleanup
	printf("Deleting RGBMatrix...\n");
	m->Clear();
	delete m;

	
	printf("Closing message queue...\n");
	mq_close(mq);
	exit(1);	

	printf("Done\n");

	
}

void snakeGame(RGBMatrix* matrix, mqd_t mq){

	printf("SNAKE GAME\n");
	srand(time(0));
		
	Point start = Point(matrix->width()/2, matrix->height()/2);
	Color c = Color(255, 255, 0);
	Snake p1 = Snake(start, c, NORTH);
	p1.grow(3);
	
	FrameCanvas *offscreen_canvas = matrix->CreateFrameCanvas();
	
	Point apple = Point(rand() % matrix->width(), rand() % matrix->width());
	
	offscreen_canvas->SetPixel(apple.x, apple.y, c.r, c.g, c.b);
	p1.draw(offscreen_canvas);
	
	
	bool gameOver = false;
	while(!gameOver && !interrupt_received){
		
		mq_msg_t msg;
		if(mq_receive(mq, (char*)&msg, sizeof(msg), NULL) != -1) {
		
			if(msg.type == DATA_TYPE_EVENT){
				button_event_t event = msg.data.event;

				if(event.type == GP_EVENT_AXIS && event.value != 0){

					if(event.name == AXIS_X1){
						//printf("Player%c moved axis x1 with value of %d\n", msg.dev[strlen(msg.dev)-1], event.value);

						if(p1.getDirection() != EAST && p1.getDirection() != WEST){
							if(event.value > 0){
								p1.setDirection(EAST);
							}
							else if(event.value < 0){
								p1.setDirection(WEST);
							}
						}

					}
					else if(event.name == AXIS_Y1){
						//printf("Player%c moved axis y1 with value of %d\n", msg.dev[strlen(msg.dev)-1], event.value);

						if(p1.getDirection() != NORTH && p1.getDirection() != SOUTH){
							if(event.value > 0){
								p1.setDirection(SOUTH);
							}
							else if(event.value < 0){
								p1.setDirection(NORTH);
							}
						}
					}
					
				}
				
			}
		}
		

	
		Point loc = p1.move();

		if(
		   p1.isCollision(loc) ||
		   loc.x > matrix->width()-1 ||
		   loc.y > matrix->height()-1 ||
		   loc.x < 0 ||
		   loc.y < 0
		   ){

			gameOver = true;

		}else{

			offscreen_canvas->Clear();
			
			if(loc == apple){
				p1.grow(3);
				apple = Point(rand() % matrix->width(), rand() % matrix->width());
			}

			offscreen_canvas->SetPixel(apple.x, apple.y, c.r, c.g, c.b);
			p1.draw(offscreen_canvas);
			offscreen_canvas = matrix->SwapOnVSync(offscreen_canvas);
		}

		usleep(100*1000);
		   		
	}
	
}
