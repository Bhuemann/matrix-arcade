#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

void clear_offscreen_canvas(unsigned int lines, unsigned int cols)
{
	for (int x = 0; x < lines; x++) {
		for (int y = 0; y < cols; y++) {
			led_canvas_set_pixel(offscreen_canvas, x, y, 0, 0, 0);
		}
	}
}

// print snake
void printsk(int X, int Y, int FruitX, int FruitY, int *tailX, int *tailY, int nTail, int WIDTH, int HEIGHT) {
	clear_offscreen_canvas(HEIGHT, WIDTH);

    led_canvas_set_pixel(offscreen_canvas, X, Y, 0, 0, 255);
    led_canvas_set_pixel(offscreen_canvas, FruitX, FruitY, 0, 255, 0);

    for (int i = 0; i < nTail; i++) {
        led_canvas_set_pixel(offscreen_canvas, tailX[i], tailY[i], 0, 0, 255);
    }

	offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
}

void print_pong(int paddle1X, int paddle1Y, int paddle2X, int paddle2Y, int paddleHeight, int ballX, int ballY, int lines, int cols, struct color *textBuf, struct color border)
{
	clear_offscreen_canvas(lines, cols);

	// print any text on screen
	if (textBuf) {
		for (int x = 0; x < lines; x++) {
			for (int y = cols - 1; y >= 0; y--) {
				led_canvas_set_pixel(offscreen_canvas, x, y, textBuf->r, textBuf->g, textBuf->b);
				textBuf++;
			}
		}
	}

	// print paddles
	for (int i = 0; i < paddleHeight; i++) {
		led_canvas_set_pixel(offscreen_canvas, paddle1Y + i, paddle1X, 0, 0, 255);
		led_canvas_set_pixel(offscreen_canvas, paddle2Y + i, paddle2X, 0, 0, 255);
	}

	// draw border around edge of LED matrix 
	for (int i = 0; i < lines; i++) {
		led_canvas_set_pixel(offscreen_canvas, i, 0, border.r, border.g, border.b);
		led_canvas_set_pixel(offscreen_canvas, i, cols - 1, border.r, border.g, border.b);
	}
	for (int i = 1; i < cols - 1; i++) {
		led_canvas_set_pixel(offscreen_canvas, 0, i, border.r, border.g, border.b);
		led_canvas_set_pixel(offscreen_canvas, lines - 1, i, border.r, border.g, border.b);
	}

	// print ball
	led_canvas_set_pixel(offscreen_canvas, ballY, ballX, 0, 255, 0);

	offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
}

void delete_matrix()
{
	led_matrix_delete(matrix);
}

}
