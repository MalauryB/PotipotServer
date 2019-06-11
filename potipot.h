#ifndef H_Potipot
#define H_Potipot
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "watering.h"

typedef struct State
{
	int code;
	char* message;
}State;

#endif
