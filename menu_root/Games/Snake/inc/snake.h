#ifndef SNAKE_H
#define SNAKE_H

#include "graphics.h"
#include "led-matrix.h"
#include "common.h"


#define BDF_FONT_FILE "../../../matrix/fonts/4x6.bdf"

using namespace rgb_matrix;

enum direction{
	NORTH,
	EAST,
	SOUTH,
	WEST
};


class Segment{
 public:
	int x;
	int y;
	struct Color c;

	Segment *next;
	Segment *prev;

	Segment(Point pt, Color c, Segment *prev = nullptr, Segment *next = nullptr);
	Segment(Segment *obj);
};


class Snake{

 private:
	Segment *head;
	Segment *tail;
	int growth;
	int size;
	direction bearing;


 public:
	Snake(Point pt, Color c, direction bearing);
	~Snake();
	Point move();
	void setDirection(direction d);
	bool isCollision(Point pt);
	void grow(int n);
	void draw(FrameCanvas *canvas);

};

void snakeGame(RGBMatrix* matrix, mqd_t mq);

#endif
