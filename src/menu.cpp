


#include <string.h>  // for strdup
#include <unistd.h>  // for usleep

#include "led-matrix.h"
#include "menu.h"
/*
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
*/
Menu::Menu(Font *font, Color c, int width, int height, vector<string> entries){

	this->font = font;
	this->lineSpacing = 0;
	this->defaultColor = c;
	this->selectedIndex = 0;
	this->defaultScrollSpeed = 6;
	this->scrollSpeed = defaultScrollSpeed;
	this->scrollAction = 0;
	this->timeSinceLastUpdate = Clock::now();
	this->width = width;
	this->height = height;
	this->entries = entries;

	this->y = height / 4;
	this->usecWaitTime = 1000000 / scrollSpeed / font->CharacterWidth('W');

	if(!entries.empty()) this->curr = entries.front();

}

Menu::~Menu(){

	
}

void Menu::setDefaultScrollSpeed(int speed){
	defaultScrollSpeed = speed;
	usecWaitTime = 1000000 / scrollSpeed / font->CharacterWidth('W');	
	if (usecWaitTime < 0) usecWaitTime = 2000;

}

void Menu::setScrollSpeed(int speed){
	scrollSpeed = speed;
	usecWaitTime = 1000000 / scrollSpeed / font->CharacterWidth('W');	
	if (usecWaitTime < 0) usecWaitTime = 2000;
}

int Menu::getScrollAction(){
	return scrollAction;
}

int Menu::stringWidth(string str){

	const char* pin = str.c_str();
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

int Menu::drawMenu(FrameCanvas* canvas){


	if(scrollAction == 0){

		string str = isEmpty() ? "NULL": curr;
		int x = (canvas->width() / 2) - (this->stringWidth(str) / 2);
		int y = canvas->height() / 4;

		rgb_matrix::DrawText(canvas, *font, x, y, defaultColor, NULL, str.c_str(), lineSpacing);   

	}
	else if(scrollAction > 0){

		auto diff = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - timeSinceLastUpdate).count();
		if(diff >= usecWaitTime){
			
			x_curr++;
			x_prev++;
			
			if(x_curr >= x_curr_end){
				scrollAction = 0;
				this->setScrollSpeed(defaultScrollSpeed);
			}
			
			timeSinceLastUpdate = Clock::now();
		}

		rgb_matrix::DrawText(canvas, *font, x_prev, y, defaultColor, NULL, prev.c_str(), lineSpacing);
		rgb_matrix::DrawText(canvas, *font, x_curr, y, defaultColor, NULL, curr.c_str(), lineSpacing);


	}
	else if(scrollAction < 0){

		auto diff = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - timeSinceLastUpdate).count();
		if(diff >= usecWaitTime){

			x_curr--;
			x_prev--;
			
			if(x_curr <= x_curr_end){
				scrollAction = 0;
				this->setScrollSpeed(defaultScrollSpeed);
				
			}
			
			timeSinceLastUpdate = Clock::now();
		}

		rgb_matrix::DrawText(canvas, *font, x_prev, y, defaultColor, NULL, prev.c_str(), lineSpacing);
		rgb_matrix::DrawText(canvas, *font, x_curr, y, defaultColor, NULL, curr.c_str(), lineSpacing);

		
	}


	return 0;
}


int Menu::scrollLeft(){


	if(scrollAction < 0){
		this->setScrollSpeed(scrollSpeed*2);
	}

	
	if(curr != entries.back()){
		
		if(scrollAction == 0){

			scrollAction = -1;
			prev = curr;
			curr = entries.at(++selectedIndex);
			x_curr_end = (width / 2) - (this->stringWidth(curr) / 2);
			
			x_prev = (width / 2) - (this->stringWidth(prev) / 2);
			x_curr = x_curr_end + width;
		}
		else if(scrollAction > 0){

			scrollAction = -1;

			prev = curr;
			curr = entries.at(++selectedIndex);
			x_curr_end = (width / 2) - (this->stringWidth(curr) / 2);
			
			std::swap(x_prev, x_curr);
		}
		
	}

	
	return 0;
	
}

int Menu::scrollRight(){

	if(scrollAction > 0){
		this->setScrollSpeed(scrollSpeed*2);
	}
	
	
	if(curr != entries.front()){
		
		if(scrollAction == 0){

			scrollAction = 1;
			prev = curr;
			curr = entries.at(--selectedIndex);
			x_curr_end = (width / 2) - (this->stringWidth(curr) / 2);

			x_prev = (width / 2) - (this->stringWidth(prev) / 2);
			x_curr = x_curr_end - width;
			
		}
		else if(scrollAction < 0){

			scrollAction = 1;
					
			prev = curr;
			curr = entries.at(--selectedIndex);
			x_curr_end = (width / 2) - (this->stringWidth(curr) / 2);
			std::swap(x_prev, x_curr);

		}
	}

	
	return 0;
}

string Menu::getSelection(){

	return isEmpty() ? NULL: curr;
}

bool Menu::isEmpty(){
	return entries.empty();
}

