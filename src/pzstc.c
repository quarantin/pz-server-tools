#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "pzst.h"


int main(int argc, char **argv) {

	size_t size;
	ssize_t ssize;
	int err, sock;
	char buf[BUFSIZ];
	struct sockaddr_un sun;
	struct ioctl_command cmd;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s <pid> <command> [args]...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	size = argc - 3;
	for (int i = 2; i < argc; i++)
		size += strlen(argv[i]);

	if (size >= MAX_COMMAND_SIZE) {
		fprintf(stderr, "Error: command too big!\n");
		exit(EXIT_FAILURE);
	}

	memset(cmd.command, 0, MAX_COMMAND_SIZE);
	for (int i = 2; i < argc; i++) {
		strcat(cmd.command, argv[i]);
		strcat(cmd.command, " ");
	}

	cmd.command[size] = 0;
	cmd.pid = atoi(argv[1]);

	memset(&sun, 0, sizeof(sun));
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, SERVER_SOCKET);

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	err = connect(sock, (struct sockaddr *)&sun, SUN_LEN(&sun));
	if (err) {
		perror("connect failed");
		exit(EXIT_FAILURE);
	}

	ssize = send(sock, &cmd, 4 + strlen(cmd.command) + 1, 0);
	if (ssize < 0) {
		perror("send failed");
		exit(EXIT_FAILURE);
	}

	ssize = recv(sock, buf, sizeof(buf), 0);
	if (ssize < 1) {
		perror("recv failed");
		exit(EXIT_FAILURE);
	}

	buf[ssize - 1] = 0;

	if (ssize != 1) {
		fprintf(stderr, "%s\n", buf);
		exit(EXIT_FAILURE);
	}

	printf("OK\n");
	return 0;
}
