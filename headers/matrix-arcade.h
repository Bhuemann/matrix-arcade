
#ifndef LED_MATRIX_H
#define LED_MATRIX_H

int menuHandler(RGBMatrix* m, FrameCanvas *offscreen_canvas, mqd_t mq, string path, Font* font, Color c);
bool findExecutable(const char* path, char* buffer);
bool runExecutable(RGBMatrix* matrix, const char* path);

#endif
