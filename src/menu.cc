#include <iostream>
#include <string>
#include <vector>

#include <dirent.h>
#include <pthread.h>
#include <unistd.h>

#include "../rpi-rgb-led-matrix/include/graphics.h"
#include "../rpi-rgb-led-matrix/include/led-matrix.h"

#define GAMES_DIR "../games"
#define FONT1 "../rpi-rgb-led-matrix/fonts/6x10.bdf"

extern "C" void *gamepadHandler(void*);

void get_files(const char *path, std::vector<std::string> &files)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path))) {
		while ((ent = readdir(dir))) {
			if (ent->d_name[0] != '.')
				files.push_back(std::string(ent->d_name));
		}
		closedir(dir);
	}
}

void startMenu(int argc, char **argv)
{
	std::vector<std::string> games;
	get_files(GAMES_DIR, games);

	rgb_matrix::Font font;
	if (!font.LoadFont(FONT1)) {
		puts("load font failed");
		exit(1);
	}

	rgb_matrix::RGBMatrix::Options matrix_opt;
	rgb_matrix::RuntimeOptions runtime_opt;
	if (!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &matrix_opt, &runtime_opt)) {
		puts("parse options failed");
		exit(1);
	}

	rgb_matrix::RGBMatrix *canvas = rgb_matrix::CreateMatrixFromOptions(matrix_opt, runtime_opt);
	if (!canvas) {
		puts("create canvas failed");
		exit(1);
	}
	canvas->SetBrightness(50);

	rgb_matrix::Color color(255, 255, 0);
	rgb_matrix::Color bg_color(0, 0, 0);

	int yPos = 0;
	for (auto game : games) {
		rgb_matrix::DrawText(canvas, font, 3, yPos + font.baseline(), color, &bg_color, game.c_str(), 0);
		yPos += font.height();
	}
	sleep(5);
	canvas->Clear();
	while (1);
}

int main(int argc, char **argv)
{
	// starts Ben's controller logic as a separate thread
	pthread_t gamepadThread;
	pthread_create(&gamepadThread, NULL, gamepadHandler, NULL);
	// displays a simple menu currently
	startMenu(argc, argv);
	// soon will have the menu interacting with the controller
}
