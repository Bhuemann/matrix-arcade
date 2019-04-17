
#ifndef LED_MATRIX_H
#define LED_MATRIX_H

int menuHandler(RGBMatrix* m, mqd_t mq, char* path, Font* font, Color c);
bool findExecutable(const char* path, char* buffer);
bool runExecutable(RGBMatrix* matrix, const char* path);

#endif
