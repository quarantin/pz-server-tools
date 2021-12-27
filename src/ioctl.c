#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

void do_ioctl(int fd, char *data) {

	int err;

	err = ioctl(fd, TIOCSTI, data);
	if (err) {
		perror("ioctl failed");
		exit(EXIT_FAILURE);
	}
}

void wait_pid(pid_t pid) {

	int err;

	while (1) {

		err = kill(pid, 0);
		if (err && errno == ESRCH)
			break;

		printf(".");
		fflush(stdout);
		usleep(200 * 1000);
	}

	printf("\n");
}

int main(int argc, char **argv) {

	int fd;
	pid_t pid;
	char path[64];

	if (argc < 3) {
		printf("Usage: %s <pid> \"<command> [args...]\"\n\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	pid = atoi(argv[1]);
	snprintf(path, sizeof(path), "/proc/%d/fd/0", pid);

	fd = open(path, O_WRONLY);
	if (fd < 0) {
		perror("open failed");
		exit(EXIT_FAILURE);
	}

	for (int i = 2; i < argc; i++) {

		for (int j = 0; j < strlen(argv[i]); j++)
			do_ioctl(fd, &argv[i][j]);

		do_ioctl(fd, " ");
	}

	do_ioctl(fd, "\n");

	close(fd);

	if (!strcmp(argv[2], "quit"))
		wait_pid(pid);

	exit(EXIT_SUCCESS);
}
