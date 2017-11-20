#include <math.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../headers/msgque.h"
#include "../headers/font.h"

#define MAX_SCORE 5
#define P1_WINS 1
#define P2_WINS 2

struct color {
	char r, g, b, unused;
};

void print_pong(int paddle1X, int paddle1Y, int paddle2X, int paddle2Y, int paddleHeight, int ballX, int ballY, int lines, int cols, struct color *textBuf);
void printm(const struct color *buf, unsigned int lines, unsigned int cols);
void bputs(struct bitmap_font font, int invert, const char *s, int line, int col, int lines, int cols, struct color *buf, struct color fg);

int rows, cols;
int paddleHeight;
int paddle1X, paddle2X, paddle1Y, paddle2Y, paddle1Dir, paddle2Dir;
int ballX, ballY, ballVX, ballVY, ballRefresh;
int p1Score, p2Score;
int ceilY, floorY;
int origBallX, origBallY, origBallVX, origBallVY, origPaddleY, currBallRefresh, origBallRefresh;

void init_pong(int r, int c)
{
	p1Score = p2Score = 0;

	rows = r;
	cols = c;

	ballX = cols / 2;
	ballY = rows / 2;
	paddleHeight = 5;
	paddle1Y = paddle2Y = ballY - paddleHeight / 2;

	ballVX = 1;
	ballVY = 0;

	ceilY = -1;
	floorY = rows;

	ballRefresh = 50000;
	paddle1X = 1;
	paddle2X = cols - 2;

	paddle1Dir = paddle2Dir = 0;

	origBallX = ballX;
	origBallY = ballY;
	origBallVX = ballVX;
	origBallVY = ballVY;
	origPaddleY = paddle1Y;
	origBallRefresh = currBallRefresh = ballRefresh;
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

void reset_ball_and_paddles()
{
	ballX = origBallX;
	ballY = origBallY;
	ballVX = origBallVX;
	ballVY = origBallVY;

	paddle1Y = paddle2Y = origPaddleY;

	currBallRefresh = origBallRefresh;
}

void check_for_goal()
{
	if (ballX > 0 && ballX < cols - 1)
		return;

	// determine who earned the point
	if (!ballX)
		p1Score++;
	else
		p2Score++;

	reset_ball_and_paddles();
}

void update_ball()
{
	int futureX = ballX + ballVX;
	int futureY = ballY + ballVY;
	if (ball_will_collide_with_ceil(futureY)
		|| ball_will_collide_with_floor(futureY)) {
		ballVY = -ballVY;
	}

	if (ball_will_collide_with_paddle1(futureX, futureY)) {
		int paddleHitLocation = paddle1Y + paddleHeight / 2 - ballY;
		if (paddleHitLocation > 0)
			ballVY = -1;
		else if (paddleHitLocation < 0)
			ballVY = 1;
		else
			ballVY = 0;
		ballVX = -ballVX;
		currBallRefresh -= 500;
	}

	if (ball_will_collide_with_paddle2(futureX, futureY)) {
		int paddleHitLocation = paddle2Y + paddleHeight / 2 - ballY;
		if (paddleHitLocation > 0)
			ballVY = -1;
		else if (paddleHitLocation < 0)
			ballVY = 1;
		else
			ballVY = 0;
		ballVX = -ballVX;
		currBallRefresh -= 500;
	}

	if (currBallRefresh < 10000)
		currBallRefresh = 10000;

	// update ball location based on X and Y velocities
	ballX += ballVX;
	ballY += ballVY;
}

int is_game_over()
{
	if (p1Score == MAX_SCORE)
		return P1_WINS;
	if (p2Score == MAX_SCORE)
		return P2_WINS;
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

	extern struct bitmap_font font5x7;
	struct color fg = {200, 0, 0};
	struct color buf[1024];

	while (1) {
		read_from_controllers(mq);
		if (!ballRefresh) {
			update_paddles();
			update_ball();
			check_for_goal();

			char score[4];
			score[0] = p1Score + '0';
			score[1] = '-';
			score[2] = p2Score + '0';
			score[3] = 0;
			memset(buf, 0, sizeof buf);
			bputs(font5x7, 0, score, 2, 8, rows, cols, buf, fg);
			print_pong(paddle1X, paddle1Y, paddle2X, paddle2Y, paddleHeight, ballX, ballY, rows, cols, buf);

			int winner = is_game_over();
			if (winner) {
				memset(buf, 0, sizeof buf);
				bputs(font5x7, 0, winner == P1_WINS ? "P1 Won" : "P2 Won", 2, 2, rows, cols, buf, fg);
				printm(buf, rows, cols);
				print_pong(paddle1X, paddle1Y, paddle2X, paddle2Y, paddleHeight, ballX, ballY, rows, cols, buf);
				sleep(5);
				return;
			}
			ballRefresh = currBallRefresh;
		}
		ballRefresh--;
	}
}
