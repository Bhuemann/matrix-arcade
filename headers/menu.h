
#ifndef MENU_H
#define MENU_H

#include <dirent.h>
#include <chrono>
#include <vector>
#include <string>

#include "graphics.h"
#include "led-matrix.h"

#define BDF_FONT_FILE "./matrix/fonts/helvR12.bdf"

//using rgb_matrix::RGBMatrix;
//using rgb_matrix::Color;

using std::vector;
using std::string;
//using namespace std::chrono;
using namespace rgb_matrix;

using Clock = std::chrono::high_resolution_clock;
//using TimePoint = std::chrono::time_point<Clock>;
using TimePoint = Clock::time_point;

class Menu {

 private:

	vector<string> entries;                                   //Entries displayed inside menu
	string curr;                                              //Current entry to be displayed
  	string prev;                                              //Previous entry that was displayed
	int selectedIndex;                                        //Current selected entry index
	int width, height;                                        //width and height of the display in pixels

	int defaultScrollSpeed;                                   //Default scroll speed
	int scrollSpeed;                                          //Current speed menu is scrolling
	int usecWaitTime;                                         //Time between updates in usec
	int scrollAction;                                         //Current scroll action: left (-1), right (1), holding (0)
	int x_curr_end;                                           //Ending x pos for scrolling entries 
	int x_curr, x_prev;                                       //Current x pos for scrolling entries
	int y;                                                    //y pos does not change on scroll
	
	
	TimePoint timeSinceLastUpdate;                            //Time since last update was made to canvas 
	
	Font *font;                                               //Fontface that will be printed
	struct Color defaultColor;                                //Color text will appear as
	int lineSpacing;                                          //Line space between characters
	
	FrameCanvas *canvas;                                      //Canvas we will be drawing to
	
	
	int stringWidth(string str);



	
 public:
	Menu(Font* font, Color c, int width, int height, vector<string> entires);
	~Menu();
	
	int drawMenu(FrameCanvas *canvas);
	int scrollRight();
	int scrollLeft();
	string getSelection();
	bool isEmpty();
	//void loadEntries(const char* path);
	//void loadEntries(int size, char* entries[]);

	void setDefaultScrollSpeed(int speed);
	void setScrollSpeed(int speed);
	int getScrollAction();
};

	
#endif
