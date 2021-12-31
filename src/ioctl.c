#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#define BUF_SIZE 64
#define MAX_WAIT 60
#define MIN_UID 1000

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
	time_t start;

	start = time(NULL);

	while (1) {

		err = kill(pid, 0);
		if (err && errno == ESRCH)
			break;

		if (time(NULL) - start > MAX_WAIT) {
			printf("\nSomething went wrong! Process not responding after %d seconds\n", MAX_WAIT);
			exit(EXIT_FAILURE);
		}

		printf(".");
		fflush(stdout);
		usleep(200 * 1000);
	}

	printf("\n");
}

int check_target_pid(char *pidstr) {

	int fd;
	pid_t pid;
	struct stat info;
	char path[BUF_SIZE];

	pid = atoi(pidstr);
	if (pid < MIN_UID) {
		printf("This command is not allowed when targeting a system process!\n");
		exit(EXIT_FAILURE);
	}

	snprintf(path, sizeof(path), "/proc/%d/loginuid", pid);

	memset(&info, 0, sizeof(info));
	if (stat(path, &info) < 0) {
		printf("No process found with pid %d\n", pid);
		exit(EXIT_FAILURE);
	}

	if (!info.st_uid) {
		printf("This command is not allowed when targeting a process running as root!\n");
		exit(EXIT_FAILURE);
	}

	return pid;
}

void send_command(pid_t pid, int argc, char **argv) {

	int fd;
	char path[BUF_SIZE];

	snprintf(path, sizeof(path), "/proc/%d/fd/0", pid);

	fd = open(path, O_WRONLY);
	if (fd < 0) {
		perror("open failed");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < argc; i++) {

		for (int j = 0; j < strlen(argv[i]); j++)
			do_ioctl(fd, &argv[i][j]);

		do_ioctl(fd, " ");
	}

	do_ioctl(fd, "\n");

	close(fd);

	if (!strcmp(argv[0], "quit"))
		wait_pid(pid);
}

int main(int argc, char **argv) {

	pid_t pid;

	if (argc < 3) {
		printf("Usage: %s <pid> \"<command> [args...]\"\n\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	pid = check_target_pid(argv[1]);

	send_command(pid, argc - 2, &argv[2]);

	exit(EXIT_SUCCESS);
}
