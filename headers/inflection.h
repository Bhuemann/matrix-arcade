#define UID_SIZ 10
#define PASS_SIZ 8
#define SVC_SIZ 10
#define SVC_BLANK "          "

struct cmd {
	char cmdtype;
	char cid[UID_SIZ];
	char cslash1;
	char cpass[PASS_SIZ];
	char cslash2;
	char csvc[SVC_SIZ];
	char dollar;
};

#define TCPPORT 48481

#define CMD_SIZ sizeof(struct cmd)

#define h_addr h_addr_list[0] // for backward compatibility

#define CMD_ACCESS 'A'
#define CMD_CLEAR 'C'
#define CMD_REGISTER 'R'

#define USER "pong"
#define PASSWD "pong"
#define SVC_NAME "pong"
