#ifndef PZST_H
#define PZST_H

#include <sys/types.h>

#define MAX_COMMAND_SIZE 128

#define SERVER_PORT 666
#define SERVER_QUEUE 10
#define SERVER_SOCKET "/var/cache/pzst/pzst.sock"

struct ioctl_command {
	pid_t pid;
	char command[MAX_COMMAND_SIZE];
};

#endif
