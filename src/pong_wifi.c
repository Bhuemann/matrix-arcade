#include <arpa/inet.h>
#include <math.h>
#include <mqueue.h>
#include <netdb.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "../headers/msgque.h"
#include "../headers/font.h"
#include "../headers/inflection.h"

#define MAX_SCORE 5
#define P1_WINS 1
#define P2_WINS 2
#define BUFF_SIZE 32

struct color {
	char r, g, b, unused;
};

void print_pong(int paddle1X, int paddle1Y, int paddle2X, int paddle2Y, int paddleHeight, int ballX, int ballY, int lines, int cols, struct color *textBuf, struct color border);
void printm(const struct color *buf, unsigned int lines, unsigned int cols);
void bputs(struct bitmap_font font, int invert, const char *s, int line, int col, int lines, int cols, struct color *buf, struct color fg);

jmp_buf jmpbuf;

int rows, cols;
int paddleHeight;
int paddle1X, paddle2X, paddle1Y, paddle2Y;
char paddle1Dir, paddle2Dir;
int ballX, ballY, ballVX, ballVY, ballRefresh;
int p1Score, p2Score;
int ceilY, floorY;
int origBallX, origBallY, origBallVX, origBallVY, origPaddleY, currBallRefresh, origBallRefresh;

static void init_pong(int r, int c)
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

	ceilY = 0;
	floorY = rows - 1;

	ballRefresh = 20;
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

static void send_to_server(int sock, char val)
{
	printf("sending %c\n", val);
	write(sock, &val, 1);
}

static char read_from_server(int sock)
{
	char val;
	int n = read(sock, &val, 1);
	printf("received %c\n", val);

	// change this to a longjmp later!!
	if (n <= 0) {
		longjmp(jmpbuf, 1);
	}
	return val;
}

static void read_from_controllers(mqd_t mq, int sock)
{
	mq_msg_t msg;
	while (mq_receive(mq, (char*)&msg, sizeof msg, NULL) != -1) {
		if (msg.type == DATA_TYPE_EVENT) {
			button_event_t event = msg.data.event;
			if (event.type == AXIS && event.name == AXIS_Y1) {
				if (event.value > 0)
					paddle1Dir = 1;
				if (event.value < 0)
					paddle1Dir = -1;
				if (event.value == 0)
					paddle1Dir = 0;
			}
		}
	}
	send_to_server(sock, paddle1Dir);
	paddle2Dir = read_from_server(sock);
}

static int ball_will_collide_with_paddle1(int futureX, int futureY)
{
	return (futureX == paddle1X && futureY >= paddle1Y && futureY <= paddle1Y + paddleHeight);
}

static int ball_will_collide_with_paddle2(int futureX, int futureY)
{
	return (futureX == paddle2X && futureY >= paddle2Y && futureY <= paddle2Y + paddleHeight);
}

static int ball_will_collide_with_ceil(int futureY)
{
	return futureY == ceilY;
}

static int ball_will_collide_with_floor(int futureY)
{
	return futureY == floorY;
}

static void reset_ball_and_paddles()
{
	ballX = origBallX;
	ballY = origBallY;
	ballVX = origBallVX;
	ballVY = origBallVY;

	paddle1Y = paddle2Y = origPaddleY;

	currBallRefresh = origBallRefresh;
}

static void check_for_goal()
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

static void update_ball()
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
		currBallRefresh -= 0;
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
		currBallRefresh -= 0;
	}

	// update ball location based on X and Y velocities
	ballX += ballVX;
	ballY += ballVY;
}

static int is_game_over()
{
	if (p1Score == MAX_SCORE)
		return P1_WINS;
	if (p2Score == MAX_SCORE)
		return P2_WINS;
	return 0;
}

static void update_paddles()
{
	paddle1Y += paddle1Dir;
	if (paddle1Y <= 0)
		paddle1Y = 1;
	else if (paddle1Y + paddleHeight >= cols)
		paddle1Y = cols - 1 - paddleHeight;
	paddle2Y += paddle2Dir;
	if (paddle2Y <= 0)
		paddle2Y = 1;
	else if (paddle2Y + paddleHeight >= cols)
		paddle2Y = cols - 1 - paddleHeight;
}

void start_server_communication(int *sock)
{
	char host[] = "xinu00.cs.purdue.edu";
	struct sockaddr_in socin;
	struct hostent *phe;
	struct protoent *ppe;

	// open socket used to connect to inflection server
	memset(&socin, 0, sizeof socin);
	socin.sin_family = AF_INET;

	// map host name to IP address or map dotted decimal
	if (phe = gethostbyname(host)) {
		memcpy(&socin.sin_addr, phe->h_addr, phe->h_length);
	} else if ((socin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE) {
		// couldn't get host entry
		longjmp(jmpbuf, 1);
	}

	socin.sin_port = htons((unsigned short)TCPPORT);
	ppe = getprotobyname("tcp");

	// create the socket
	*sock = socket(PF_INET, SOCK_STREAM, ppe->p_proto);
	if (*sock < 0) {
		longjmp(jmpbuf, 1);
	}

	// connect the socket
	if (connect(*sock, (struct sockaddr*)&socin, sizeof socin) < 0) {
		longjmp(jmpbuf, 1);
	}
}

static void set_user(struct cmd *pcmd, const char *user)
{
	memset(pcmd->cid, ' ', UID_SIZ);
	memcpy(pcmd->cid, user, strlen(user));
}

static void set_passwd(struct cmd *pcmd, const char *passwd)
{
	memset(pcmd->cpass, ' ', PASS_SIZ);
	memcpy(pcmd->cpass, passwd, strlen(passwd));
}

static void set_svc_name(struct cmd *pcmd, const char *svc_name)
{
	memset(pcmd->csvc, ' ', SVC_SIZ);
	memcpy(pcmd->csvc, svc_name, strlen(svc_name));
}

static void set_extra_fields(struct cmd *pcmd)
{
	pcmd->cslash1 = '/';
	pcmd->cslash2 = '/';
	pcmd->dollar = '$';
}

static void start_pong_service(int *sock)
{
	char buffer[100];
	struct cmd *pcmd = (struct cmd*)buffer;
	pcmd->cmdtype = CMD_ACCESS;

	set_user(pcmd, USER);
	set_passwd(pcmd, PASSWD);
	set_svc_name(pcmd, SVC_NAME);
	set_extra_fields(pcmd);

	send(*sock, buffer, sizeof(struct cmd), 0);
	write(*sock, "Here", strlen("Here"));

	// read reply from server
	char replybuf[1024];
	int n = read(*sock, replybuf, sizeof replybuf);
	replybuf[n] = 0;

	if (!strcmp(replybuf, "NACK\n")) {
		start_server_communication(sock);
		// the service hasn't been started yet, so start it
		pcmd->cmdtype = CMD_REGISTER;
		set_user(pcmd, USER);
		set_passwd(pcmd, PASSWD);
		set_svc_name(pcmd, SVC_NAME);
		set_extra_fields(pcmd);

		send(*sock, buffer, sizeof(struct cmd), 0);

		// wait for "Here" response from other player before returning and starting game
		n = read(*sock, replybuf, sizeof replybuf);
	}
}

void pong_wifi(int rows, int cols)
{
	if (setjmp(jmpbuf))
		return;

	// open msg queue
	mqd_t mq;
	mq = mq_open(MQ_NAME, O_RDONLY | O_NONBLOCK);
	if (mq == -1) {
		puts("message queue not available");
		return;
	}

	extern struct bitmap_font font5x7;
	struct color textColor = {200, 0, 0};
	struct color buf[1024];
	struct color border = {60, 60, 0};

	// format score string
	char score[4];
	score[0] = p1Score + '0';
	score[1] = '-';
	score[2] = p2Score + '0';
	score[3] = 0;

	init_pong(rows, cols);

	memset(buf, 0, sizeof buf);
	bputs(font5x7, 0, score, 2, 8, rows, cols, buf, textColor);
	// print initial game state
	print_pong(paddle1X, paddle1Y, paddle2X, paddle2Y, paddleHeight, ballX, ballY, rows, cols, buf, border);

	int sock;
	start_server_communication(&sock);
	start_pong_service(&sock);

	// delay to allow players to get ready
	sleep(1);

	while (1) {
		read_from_controllers(mq, sock);
		if (!ballRefresh) {
			update_paddles();
			update_ball();
			check_for_goal();

			score[0] = p1Score + '0';
			score[1] = '-';
			score[2] = p2Score + '0';

			memset(buf, 0, sizeof buf);
			bputs(font5x7, 0, score, 2, 8, rows, cols, buf, textColor);
			print_pong(paddle1X, paddle1Y, paddle2X, paddle2Y, paddleHeight, ballX, ballY, rows, cols, buf, border);

			int winner = is_game_over();
			if (winner) {
				memset(buf, 0, sizeof buf);
				bputs(font5x7, 0, winner == P1_WINS ? "P1 Won" : "P2 Won", 2, 2, rows, cols, buf, textColor);
				printm(buf, rows, cols);
				print_pong(paddle1X, paddle1Y, paddle2X, paddle2Y, paddleHeight, ballX, ballY, rows, cols, buf, border);
				sleep(5);
				return;
			}
			ballRefresh = currBallRefresh;
		}
		ballRefresh--;
	}
}
