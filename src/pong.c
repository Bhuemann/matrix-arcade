#include <math.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../headers/msgque.h"
#include "../headers/font.h"

#define BALL_SPEED 1
#define P1_WINS 1
#define P2_WINS 2

struct color {
	char r, g, b, unused;
};

void print_pong(int paddle1X, int paddle1Y, int paddle2X, int paddle2Y, int paddleHeight, int ballX, int ballY, int lines, int cols, struct color *textBuf);
void printm(const struct color *buf, unsigned int lines, unsigned int cols);
void bputs(struct bitmap_font font, int invert, const char *s, int line, int col, int lines, int cols, struct color *buf, struct color fg);

float MAX_ANGLE = 0.785398;
int paddleHeight;
int paddle1X, paddle2X;
int paddle1Y, paddle2Y, rows, cols;
float ballX, ballY, ballVX, ballVY;

int paddle1Dir, paddle2Dir;

int ceilY, floorY;

void init_pong(int r, int c)
{
	rows = r;
	cols = c;

	ballX = cols / 2;
	ballY = paddle1Y = paddle2Y = rows / 2;

	ballVX = 1;
	ballVY = 0;

	ceilY = -1;
	floorY = rows;

	paddle1X = 1;
	paddle2X = cols - 2;
	paddleHeight = 5;

	paddle1Dir = paddle2Dir = 0;
}

void read_from_controllers(mqd_t mq)
{
	mq_msg_t msg;
	while (mq_receive(mq, (char*)&msg, sizeof msg, NULL) != -1) {
		if (msg.type == DATA_TYPE_EVENT) {
			button_event_t event = msg.data.event;
			if (event.type == 2 && event.name == 9) {
				if (event.value > 0)
					paddle1Dir = 1;
				if (event.value < 0)
					paddle1Dir = -1;
				if (event.value == 0)
					paddle1Dir = 0;
			}
			if (event.type == 2 && event.name == 8) {
				if (event.value > 0)
					paddle2Dir = 1;
				if (event.value < 0)
					paddle2Dir = -1;
				if (event.value == 0)
					paddle2Dir = 0;
			}
		}
	}
}

int ball_will_collide_with_paddle1(int futureX, int futureY)
{
	return (futureX == paddle1X && futureY >= paddle1Y && futureY <= paddle1Y + paddleHeight);
}

int ball_will_collide_with_paddle2(int futureX, int futureY)
{
	return (futureX == paddle2X && futureY >= paddle2Y && futureY <= paddle2Y + paddleHeight);
}

int ball_will_collide_with_ceil(int futureY)
{
	return futureY == ceilY;
}

int ball_will_collide_with_floor(int futureY)
{
	return futureY == floorY;
}

void update_ball()
{
	int futureX = (int)(ballX + ballVX);
	int futureY = (int)(ballY + ballVY);
	if (ball_will_collide_with_ceil(futureY)
		|| ball_will_collide_with_floor(futureY)) {
		ballVY = -ballVY;
	}
	if (ball_will_collide_with_paddle1(futureX, futureY)) {
		int paddleHitLocation = paddle1Y + paddleHeight / 2 - ballY;
		float normalizedLocation = paddleHitLocation * 2 / (double)paddleHeight;
		float bounceAngle = MAX_ANGLE * normalizedLocation;
		ballVX = cos(bounceAngle);
		ballVY = sin(bounceAngle);
	}
	if (ball_will_collide_with_paddle2(futureX, futureY)) {
		int paddleHitLocation = paddle2Y + paddleHeight / 2 - ballY;
		float normalizedLocation = paddleHitLocation * 2 / (double)paddleHeight;
		float bounceAngle = MAX_ANGLE * normalizedLocation;
		ballVX = -cos(bounceAngle);
		ballVY = sin(bounceAngle);
	}

	ballX += ballVX;
	ballY += ballVY;
}

int is_game_over()
{
	if (ballX < paddle1X)
		return P2_WINS;
	if (ballX > paddle2X)
		return P1_WINS;
	return 0;
}

void update_paddles()
{
	paddle1Y += paddle1Dir;
	if (paddle1Y < 0)
		paddle1Y = 0;
	else if (paddle1Y >= cols)
		paddle1Y = cols - 1;
	paddle2Y += paddle2Dir;
	if (paddle2Y < 0)
		paddle2Y = 0;
	else if (paddle2Y >= cols)
		paddle2Y = cols - 1;
}

void pong(int rows, int cols)
{
	// open msg queue
	mqd_t mq;
	mq = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK);
	if (mq == -1) {
		puts("message queue not available");
		return;
	}

	init_pong(rows, cols);

	sleep(1);
	int checkVal = 50000;

	while (1) {
		read_from_controllers(mq);
		if (!checkVal) {
			update_paddles();
			update_ball();
			print_pong(paddle1X, paddle1Y, paddle2X, paddle2Y, paddleHeight, (int)ballX, (int)ballY, rows, cols, NULL);
			int winner = is_game_over();
			if (winner) {
				extern struct bitmap_font font5x7;
				struct color fg = {200, 0, 0};
				struct color buf[1024];
				memset(buf, 0, sizeof buf);
				bputs(font5x7, 0, winner == P1_WINS ? "P1 Won" : "P2 Won", 2, 2, 32, 32, buf, fg);
				printm(buf, 32, 32);
				print_pong(paddle1X, paddle1Y, paddle2X, paddle2Y, paddleHeight, (int)ballX, (int)ballY, rows, cols, buf);
				sleep(5);
				return;
			}
			checkVal = 50000;
		}
		checkVal--;
	}
}
