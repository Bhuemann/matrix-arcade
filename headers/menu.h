
#include <dirent.h>

#define MAX_NUM_DIRS 16

class Menu {

 private:
	char *path;
	char *entries[MAX_NUM_DIRS];
	int lastEntryIndex;
	int selectedIndex;
	
	//char* getSelection();
	//void scrollRight();
	//void scrollLeft();
	
 public:
	Menu(char* path);
	~Menu();
	//void drawMenu();
	//void clearMenu();
	
};
