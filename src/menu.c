#include <assert.h>
#include <dirent.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../headers/font.h"
#include "../headers/menu.h"
#include "../headers/msgque.h"

#define LINES 32
#define COLS 32

#define GAMES_DIR "/home/pi/rpi-led-game-system-master/games"

char *strdup(const char *s);
void *gamepadHandler(void*);
int usleep(unsigned long);

static int min(int a, int b)
{
	return a < b ? a : b;
}

static int calc_width(struct bitmap_font font, const char *s)
{
	int width = 0;
	while (*s) {
		width += font.Widths[*s - ' ' + 1];
		s++;
	}
	return width;
}

void bputs(struct bitmap_font font, int invert, const char *s, int line, int col, int lines, int cols, struct color *buf, struct color fg)
{
	struct color *copy = buf;

	if ((line < 0 && -line > font.Height) || line >= lines)
		return;

	int height;
	int hoffset;

	if (line >= 0) {
		buf += line * cols;
		hoffset = 0;
		height = min(font.Height, lines - line);
	} else {
		hoffset = -line;
		height = min(font.Height, lines);
	}

	while (*s) {
		const unsigned char *p = font.Bitmap + (*s - ' ' + 1) * font.Height;
		int woffset, width;
		struct color *buf2;

		if (col >= 0) {
			if (col >= cols)
				return;
			buf2 = buf + col;
			woffset = 0;
			width = min(font.Widths[*s - ' ' + 1], cols - col);
		} else {
			width = font.Widths[*s - ' ' + 1];
			if (-col >= width) {
				col += width;
				s++;
				continue;
			}
			buf2 = buf;
			woffset = -col;
			width = min(width, cols);
		}

		for (int i = hoffset; i < height; i++) {
			assert((buf2 >= copy) && (buf2 + width - woffset) <= (copy + lines * cols));
			for (int j = 7 - woffset; j > 7 - width - woffset; j--) {
				if ((p[i] >> j) & 1)
					buf2[7 - j] = fg;
			}
			buf2 += cols;
		}

		col += width;
		s++;
	}
}

void color_memset(struct color *dest, struct color c, unsigned int n)
{
	for (int i = 0; i < n; i++)
		*dest++ = c;
}

int menu(struct bitmap_font font, const char *items[], unsigned int count, int lines, int cols, struct color *buf, mqd_t mq, struct color fg, struct color bg)
{
	const int pause_time = 5;
	int widths[count], selected = 0, scroll = -1, pause = pause_time;

	for (int i = 0; i < count; i++)
		widths[i] = calc_width(font, items[i]);

	for (;;) {
		mq_msg_t msg;
		while (mq_receive(mq, (char*)&msg, sizeof msg, NULL) != -1) {
			// read all messages
			if (msg.type == DATA_TYPE_EVENT) {
				button_event_t event = msg.data.event;

				if (event.type == 1) {
					// button press
					if (event.name == 0 && event.value == 1) {
						// A pressed
						return selected;
					}
				} else if (event.type == 2) {
					// axis
					if (event.name == 8) {
						// left-right axis
					} else if (event.name == 9) {
						// up-down axis
						if (event.value == 32767) {
							// down press
							selected = (selected + 1) % count;
						}
						if (event.value == -32767) {
							// up press
							selected = (selected - 1) % count;
						}
					}
				}
			}
			if (mq_receive(mq, (char*)&msg, sizeof msg, NULL) == -1) {
				scroll = -1;
				pause = pause_time;
			}
		}

		struct color blank = {0, 0, 0, 0};
		color_memset(buf, blank, lines * cols);

		int line = 1;
		if (selected + 1> lines / font.Height) {
			line = 1 - (1 + font.Height) * (selected + 1 - lines / (1 + font.Height));
		}

		for (int i = 0; i < count; i++, line += 1 + font.Height) {
			int is_selected = i == selected;
			int m = min((1 + font.Height) * cols, lines * cols - (line - 1) * cols);
			if (is_selected) color_memset(buf + (line - 1) * cols, bg, m);
			bputs(font, is_selected, items[i], line, i == selected ? -scroll : 1, lines, cols, buf, fg);
		}

		printm((const struct color*)buf, lines, cols);
		usleep(100000);

		if (widths[selected] > cols) {
			if (pause > 0)
				pause--;
			else {
				scroll++;

				if (scroll > widths[selected])
					scroll = -cols;

			}
		}
	}
}

unsigned int get_files_in_dir(const char *path, char **files)
{
	DIR *dir;
	struct dirent *ent;
	unsigned int i = 0;
	if ((dir = opendir(path))) {
		while ((ent = readdir(dir))) {
			if (ent->d_name[0] != '.')
				files[i++] = strdup(ent->d_name);
		}
		closedir(dir);
	}
	return i;
}

int main(int argc, char **argv)
{
	init_canvas(argc, argv, LINES);

	pthread_t gamepadThread;
	pthread_create(&gamepadThread, NULL, gamepadHandler, NULL);

	sleep(1);

	extern const struct bitmap_font font5x7;
	struct color buf[LINES * COLS];
	char *games[10];
	unsigned int numGames = get_files_in_dir(GAMES_DIR, games);
	mqd_t mq;
	mq = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK);
	if (mq == -1) {
		puts("message queue not available");
		exit(1);
	}

	struct color fg = {0, 200, 200, 0}, bg = {0, 60, 0, 0};
	int selected = menu(font5x7, (const char**)games, numGames, LINES, COLS, buf, mq, fg, bg);
	printf("%s\n", games[selected]);

	// call Sean's game (games[selected]) here, then do this in a loop

	for (int i = 0; i < numGames; i++)
		free(games[i]);
	delete_matrix();
}
