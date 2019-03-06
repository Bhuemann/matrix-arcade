
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>


#include "gamepadHandler.h"
#include "gamepadEventHandler.h"
#include "menu.h"
#include "msgque.h"
#include "led-matrix.h"

#define MENU_ROOT_DIR "./menu_root"



volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
	interrupt_received = true;
}


void Menu::loadEntries(const char* path){

	DIR *dir = opendir(path);
	if (dir == NULL) {
		printf("Could not open current directory" );
		exit(0);
	}

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL){

		//File is a directory
		if (entry->d_type == DT_DIR){
			if(strcmp("..", entry->d_name) == 0 || strcmp(".", entry->d_name) == 0){
				continue;
			}
			
			entries[lastEntryIndex++] = strdup(entry->d_name);
		}
	}

	closedir(dir);	

}

void Menu::loadEntries(int size, char** entries){

	for(int i = 0; i < size; i++){
		this->entries[i] = strdup(entries[i]);
		this->lastEntryIndex++;
	}
		
}

Menu::Menu(RGBMatrix* m, Font *font, Color c, int lineSpacing){

	this->lastEntryIndex = 0;
	this->matrix = m;
	this->font = font;
	this->lineSpacing = lineSpacing;
	this->defaultColor = c;
	this->selectedIndex = 0;
	this->offscreen_canvas = matrix->CreateFrameCanvas();
	
	

}

Menu::~Menu(){

	for(int i = 0; i<lastEntryIndex; i++){
		free(entries[i]);
	}

	
}


int Menu::stringWidth(const char* str){

	const char* pin = str;
	uint32_t codept;
	int size = 0;
	
	
	while(*pin != '\0'){
		codept = *pin;
		size += font->CharacterWidth(codept) + lineSpacing;
		pin++;
	}

	size -= lineSpacing;
	return size;
	
}

int Menu::drawMenu(){

	const char* str = isEmpty() ? "<empty>": entries[selectedIndex];
	int x = (matrix->width() / 2) - (this->stringWidth(str) / 2);
	int y = matrix->height() / 4;

	rgb_matrix::DrawText(matrix, *font, x, y, defaultColor, NULL, str, lineSpacing);   

	return 0;
}

void Menu::clearMenu(){
	matrix->Clear();
}

int Menu::scrollLeft(int speed){

	if(selectedIndex + 1 < lastEntryIndex){

	
		char* curr = entries[selectedIndex];
		char* next = entries[++selectedIndex];
		
		int x_curr_center = (matrix->width() / 2) - (this->stringWidth(curr) / 2);
		int x_next_center = (matrix->width() / 2) - (this->stringWidth(next) / 2);
		int y = matrix->height() / 4;
		
		int x_curr = x_curr_center;
		int x_next = x_next_center + matrix->width();

		int delay_speed_usec = 1000000 / speed / font->CharacterWidth('W');
		if (delay_speed_usec < 0) delay_speed_usec = 2000;
		
		
		while(x_next >= x_next_center){

			offscreen_canvas->Clear();

			rgb_matrix::DrawText(offscreen_canvas, *font, x_curr--, y, defaultColor, NULL, curr, lineSpacing);
			rgb_matrix::DrawText(offscreen_canvas, *font, x_next--, y, defaultColor, NULL, next, lineSpacing);   

			usleep(delay_speed_usec);
			offscreen_canvas = matrix->SwapOnVSync(offscreen_canvas);
		}  

		return EXIT_SUCCESS;

	}
	
	return EXIT_FAILURE;

	
}

int Menu::scrollRight(int speed){

	if(selectedIndex - 1 >= 0){

	
		char* curr = entries[selectedIndex];
		char* next = entries[--selectedIndex];
		
		int x_curr_center = (matrix->width() / 2) - (this->stringWidth(curr) / 2);
		int x_next_center = (matrix->width() / 2) - (this->stringWidth(next) / 2);
		int y = matrix->height() / 4;
		
		int x_curr = x_curr_center;
		int x_next = x_next_center - matrix->width();

		int delay_speed_usec = 1000000 / speed / font->CharacterWidth('W');
		if (delay_speed_usec < 0) delay_speed_usec = 2000;
		
		
		while(x_next <= x_next_center){

			offscreen_canvas->Clear();

			rgb_matrix::DrawText(offscreen_canvas, *font, x_curr++, y, defaultColor, NULL, curr, lineSpacing);
			rgb_matrix::DrawText(offscreen_canvas, *font, x_next++, y, defaultColor, NULL, next, lineSpacing);   

			usleep(delay_speed_usec);
			offscreen_canvas = matrix->SwapOnVSync(offscreen_canvas);
		}

		return EXIT_SUCCESS;

	}
	
	return EXIT_FAILURE;

	
}

char* Menu::getSelection(){

	return isEmpty() ? nullptr: entries[selectedIndex];
}

bool Menu::isEmpty(){
	return (lastEntryIndex <= 0);
}

int main(int argc, char **argv)
{

	//Set signal handlers
	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);


	//Start GamepadHander thread
	mqd_t mq;
	int runningFlag = 0;
	int haltFlag = 0;
	pthread_t gamepadThread;

	args_t args;
	args.mq = NULL;
	args.devPath = NULL;
	args.haltFlag = &haltFlag;
	args.runningFlag = &runningFlag;
	pthread_create(&gamepadThread, NULL, gamepadHandler, &args);

	sleep(2);
	
	//Open message queue for reading
	mq = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
	if (mq == -1) {
		printf("Could not open message queue\n");
		perror("");
		haltFlag = 1;
		pthread_join(gamepadThread, NULL);
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

	
		
	//Start Menu at matrix root
	menuHandler(m, mq, MENU_ROOT_DIR, &font, c);


	//Cleanup
	printf("Deleting RGBMatrix...\n");
	m->Clear();
	delete m;

	
	printf("Closing message queue...\n");
	mq_close(mq);
	exit(1);

	
	printf("Halting gamepadHandler...\n");
	haltFlag = 1;
	pthread_join(gamepadThread, NULL);

	printf("Done\n");
	
}

//Recursively handle menu & sub-menus
int menuHandler(RGBMatrix* m, mqd_t mq, char* path, Font* font, Color c){

	//Check if exececutable is in current directory
	char exePath[255];
	if(findExecutable(path, exePath)){

		char sudo[] = "/usr/bin/sudo";
		
		int PID = fork();
		if (fork() == 0){
			execl(sudo,sudo,exePath,(char *)NULL);
			printf("BAD ECEC\n");
			exit(0);
		}else{

			int status;
			if(waitpid(PID, &status, 0) == -1){
				//Signal received in child process
			}
		}
		return 0;
	}

	
	Menu *menu = new Menu(m, font, c, 0);
	menu->loadEntries(path);
	menu->drawMenu();
	
	mq_msg_t msg;

	while(!interrupt_received){
		
		if(mq_receive(mq, (char*)&msg, sizeof(msg), NULL) != -1) {

			if(msg.type == DATA_TYPE_EVENT){
				button_event_t event = msg.data.event;

				if(event.type == GP_EVENT_BUTTON && event.value == 0){

					switch(event.name){

					case BUTTON_A:

						if(!menu->isEmpty()){

							menu->clearMenu();

							//Create new path and new menu for sub-directories
							char newPath[255];
							strcpy(newPath, path);
							strcat(newPath, "/");
							strcat(newPath, menu->getSelection());
							menuHandler(m, mq, newPath, font, c);
						
							menu->drawMenu();

						}
						
						break;
					case BUTTON_B:

						if(strcmp(path, MENU_ROOT_DIR) != 0){
							menu->clearMenu();
							return 0;
						}
						
						break;
					case BUTTON_X:
						break;
					case BUTTON_Y:
						break;
					case BUTTON_LB:
						break;
					case BUTTON_RB:
						break;
					case BUTTON_START:
						break;
					case BUTTON_SELECT:
						break;
					}


				}else if(event.type == GP_EVENT_AXIS && event.value != 0){

					if(event.name == AXIS_X1){
						

						if(event.value > 0)
							menu->scrollLeft(3);
						else if(event.value < 0)
							menu->scrollRight(3);

					}
					else if(event.name == AXIS_Y1){
						
					}
					else if(event.name == AXIS_X2){
						
					}
					else if(event.name == AXIS_Y2){
						
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



bool findExecutable(const char* path, char* buffer){

	bool ret = false;
	char newPath[255];
	DIR *dir = opendir(path);
	if (dir == NULL) {
		printf("Could not open current directory" );
		exit(0);
	}

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL){

		if (entry->d_type == DT_REG || entry->d_type == DT_LNK){

			strcpy(newPath, path);
			strcat(newPath, "/");
			strcat(newPath, entry->d_name);
			
			struct stat sb;
			if (stat(newPath, &sb) == 0 && sb.st_mode & S_IXUSR) {
				strcpy(buffer, newPath);
				ret = true;
				break;
			}
			
		}
	}

	closedir(dir);
	return ret;

	
}
