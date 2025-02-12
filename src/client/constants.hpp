#ifndef CLIENT_CONSTANTS_H
#define CLIENT_CONSTANTS_H

#define IP_FLAG "-n"
#define PORT_FLAG "-p"
#define SERVER_IP "localhost"
#define SERVER_PORT "58052"
#define CONNECTION_TIMEOUT 10
#define MAX_TRIES 8

#define BUFFER_SIZE 128

#define START "start"
#define TRY "try"
#define SHOW_TRIALS "show_trials"
#define ST "st"
#define SCOREBOARD "scoreboard"
#define SB "sb"
#define QUIT "quit"
#define EXIT "exit"
#define DEBUG "debug"

#define RDB "RDB"
#define RSS "RSS"
#define RST "RST"
#define RQT "RQT"
#define RSG "RSG"
#define RTR "RTR"

#define OK "OK"
#define NOK "NOK"
#define ERR "ERR"
#define DUP "DUP"
#define INV "INV"
#define ENT "ENT"
#define ETM "ETM"
#define ACT "ACT"
#define FIN "FIN"
#define EMPTY "EMPTY"

#define UDP 1
#define TCP 2

#define ERROR -1
#define INVALID_COMMAND_MSG "Invalid command"
#define UNPARSEABLE_MSG_SERVER "Unparseable message from server"
#define WRONG_COMMAND_MSG_SERVER "Wrong command from server"

#endif