
#include <thread>

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
#include <cstring>


#include "gamepadHandler.h"
#include "gamepadEventHandler.h"
#include "menu.h"
#include "msgque.h"
#include "led-matrix.h"
#include "graphics.h"

#include "matrix-arcade.h"

#define MENU_ROOT_DIR "./menu_root"
#define BDF_FONT_FILE "./matrix/fonts/helvR12.bdf"


volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
	interrupt_received = true;
}


int main(int argc, char **argv)
{

	//Set signal handlers
	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);


	//Start GamepadHander thread
	mqd_t mq;
	bool gph_execution_flag = true;
	thread gamepadThread(gamepadHandler, ref(gph_execution_flag));


	//Allow some time for gamepadHandler to open message queue.
	//Should be temporary solution until proper semaphores are implemented
	sleep(2);
	
	
	//Open message queue for reading
	mq = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
	if (mq == -1) {
		printf("Could not open message queue\n");
		perror("");
		gph_execution_flag = false;
		gamepadThread.join();
		exit(1);
	}
	
   
	//Create RGBMatrix
	RGBMatrix::Options matrix_options;
	RuntimeOptions runtime_opt;

	runtime_opt.drop_privileges = 0;
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

	FrameCanvas *offscreen_canvas = m->CreateFrameCanvas();
	
	//Start Menu at matrix root
	menuHandler(m, offscreen_canvas, mq, MENU_ROOT_DIR, &font, c);


	//Cleanup
	printf("Deleting RGBMatrix...\n");
	m->Clear();
	delete m;

	
	printf("Closing message queue...\n");
	mq_close(mq);

	
	printf("Halting gamepadHandler...\n");
	gph_execution_flag = 0;
	gamepadThread.join();

	printf("Done\n");
	
}

//Recursively handle menu & sub-menus
int menuHandler(RGBMatrix* m, FrameCanvas *offscreen_canvas, mqd_t mq, string path, Font* font, Color c){


	DIR *dir;
	struct dirent *entry;
	vector<string> directories;
	mq_msg_t msg;
	
	dir = opendir(path.c_str());                                                   /*Check for executables in current directory */
	if (dir == NULL) {
		printf("Error: Could not open '%s'", path);
		return 0;
	}

	string filename, filepath;
	while ((entry = readdir(dir)) != NULL){

		if (entry->d_type == DT_REG || entry->d_type == DT_LNK){

			filename = string(entry->d_name);
			filepath = path + "/" + string(entry->d_name);
			
			struct stat sb;
			if(stat(filepath.c_str(), &sb) == 0 && sb.st_mode & S_IXUSR) {
				break;
			}

			filename.clear();
			filepath.clear();
			
		}
	}

	closedir(dir);


	if(!filepath.empty()){                                                              /* If an executable is present, fork and run it. Then wait */
		m->StopRefresh();                                                               /* until executable is finished and return to previous menu */
															   
		int PID = fork();
		if (PID == 0){
			char * argv[filepath.length() + 2];
			std::strcpy(argv[0], filepath.c_str());
			argv[1] = NULL;
			execv(filepath.c_str(), argv);
			perror("EXECV");
			return 0;
		}else{
			waitpid(PID, NULL, 0);
		}

		m->StartRefresh();
		return 1;
	}


	
	dir = opendir(path.c_str());                                                        /*Get names of all directories in path directory */
	if (dir == NULL) {
		perror("OPENDIR");
		exit(0);
	}

	while ((entry = readdir(dir)) != NULL){

		//File is a directory
		if (entry->d_type == DT_DIR){
			if(strcmp("..", entry->d_name) == 0 || strcmp(".", entry->d_name) == 0){
				continue;
			}
			
			directories.push_back(entry->d_name);
		}
	}

	closedir(dir);	



	
	Menu menu = Menu(font, c, m->width(), m->height(), directories);                  /*Create new Menu of directory names*/ 

	while(!interrupt_received){                                                       /*Handle gamepad events*/
		
		if(mq_receive(mq, (char*)&msg, sizeof(msg), NULL) != -1) {

			if(msg.type == DATA_TYPE_EVENT){
				button_event_t event = msg.data.event;

				if(event.type == GP_EVENT_BUTTON && event.value == 0){

					switch(event.name){

					case BUTTON_A:                                                     /*Go forward to selected directory*/

						if(!menu.isEmpty()){

 							offscreen_canvas->Clear();
							string newPath = path + "/" + menu.getSelection();
							menuHandler(m, offscreen_canvas, mq, newPath, font, c);
						
						}
						
						break;
					case BUTTON_B:                                                     /*Go back to previous directory*/

						if(path != MENU_ROOT_DIR){
							offscreen_canvas->Clear();
							return 0;
						}
						
						break;
					case BUTTON_X:
						break;
					case BUTTON_Y:
						break;
					case BUTTON_LB:
						menu.setScrollSpeed(20);
						menu.scrollRight();
						break;
					case BUTTON_RB:
						menu.setScrollSpeed(20);
						menu.scrollLeft();
						break;
					case BUTTON_START:
						interrupt_received = true;
						break;
					case BUTTON_SELECT:
						break;
					}


				}else if(event.type == GP_EVENT_AXIS && event.value != 0){

					if(event.name == AXIS_X1){
						

						if(event.value > 0)
							menu.scrollLeft();
						else if(event.value < 0)
							menu.scrollRight();

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

		offscreen_canvas->Clear();
		menu.drawMenu(offscreen_canvas);
		offscreen_canvas = m->SwapOnVSync(offscreen_canvas);

		
	}


	
	
}

