#include <stdio.h>
#include <string.h>

#include "../rpi-rgb-led-matrix/include/graphics.h"
#include "../rpi-rgb-led-matrix/include/led-matrix.h"
#include "../rpi-rgb-led-matrix/include/led-matrix-c.h"

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

void delete_matrix()
{
	led_matrix_delete(matrix);
}

}
