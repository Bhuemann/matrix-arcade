
#ifndef COMMON_H
#define COMMON_H


class Point{
 public:
	int x,y;
	Point(int x, int y){this->x = x; this->y = y;}
	bool operator==(const Point & rhs) const { return (this->x == rhs.x && this->y == rhs.y);}
};

#endif
