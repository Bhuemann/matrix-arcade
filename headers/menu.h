
#include <dirent.h>
#include "graphics.h"
#include "led-matrix.h"


#define MAX_NUM_DIRS 16
#define BDF_FONT_FILE "./matrix/fonts/helvR12.bdf"

//using rgb_matrix::RGBMatrix;
//using rgb_matrix::Color;

using namespace rgb_matrix;

class Menu {

 private:

	char *entries[MAX_NUM_DIRS];
	int lastEntryIndex;
	int selectedIndex;

	Font *font;
	struct Color defaultColor;
	int lineSpacing;
	
	RGBMatrix* matrix;
	FrameCanvas *offscreen_canvas;
	
	
	int stringWidth(const char* str);



	
 public:
	Menu(RGBMatrix* m, Font* font, Color c, int lineSpacing);
	~Menu();
	
	int drawMenu();
	void clearMenu();
	int scrollRight(int speed);
	int scrollLeft(int speed);
	char* getSelection();
	bool isEmpty();
	void loadEntries(const char* path);
	void loadEntries(int size, char* entries[]);
};

int menuHandler(RGBMatrix* m, mqd_t mq, char* path, Font* font, Color c);
bool findExecutable(const char* path, char* buffer);
bool runExecutable(RGBMatrix* matrix, const char* path);
	
