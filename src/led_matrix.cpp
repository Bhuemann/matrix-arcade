#include <stdio.h>
#include <string.h>

#include "graphics.h"
#include "led-matrix.h"
#include "led-matrix-c.h"

struct RGBLedMatrix *matrix;
struct LedCanvas *offscreen_canvas;

extern "C" {

#include "../headers/menu.h"

void init_canvas(int argc, char **argv, unsigned int lines)
{
	struct RGBLedMatrixOptions opts;
	memset(&opts, 0, sizeof opts);
	opts.rows = lines;
	opts.chain_length = 1;

	matrix = led_matrix_create_from_options(&opts, &argc, &argv);
	if (!matrix) {
		puts("unable to create matrix");
		exit(1);
	}

	offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);
}

// print buf to LED matrix
void printm(const struct color *buf, unsigned int lines, unsigned int cols)
{
	for (int x = 0; x < lines; x++) {
		for (int y = cols - 1; y >= 0; y--) {
			led_canvas_set_pixel(offscreen_canvas, x, y, buf->r, buf->g, buf->b);
			buf++;
		}
	}
	offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
}

// print snake
void printsk(int X, int Y, int FruitX, int FruitY, int *tailX, int *tailY, int nTail, int WIDTH, int HEIGHT) {
    // clear canvas 
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
			led_canvas_set_pixel(offscreen_canvas, i, j, 0, 0, 0);
        }
    }

    led_canvas_set_pixel(offscreen_canvas, X, Y, 0, 0, 255);
    led_canvas_set_pixel(offscreen_canvas, FruitX, FruitY, 0, 255, 0);

    for (int i = 0; i < nTail; i++) {
        led_canvas_set_pixel(offscreen_canvas, tailX[i], tailY[i], 0, 0, 255);
    }

	offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
}


void delete_matrix()
{
	led_matrix_delete(matrix);
}

}
