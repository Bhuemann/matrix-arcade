

#include <string.h>  // for strdup
#include <unistd.h>  // for usleep

#include "led-matrix.h"
#include "menu.h"

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

Menu::Menu(RGBMatrix* m, Font *font, Color c){

	this->lastEntryIndex = 0;
	this->matrix = m;
	this->font = font;
	this->lineSpacing = 0;
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

