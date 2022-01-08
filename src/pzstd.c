#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>

#include <errno.h>
#include <grp.h>
#include <fcntl.h>
#include <time.h>

#include <netdb.h>
#include <netinet/in.h>

#include <unistd.h>

#include "pzst.h"

#define MAX_WAIT 30

#define PZST_GROUP "pzst"
#define PROC_NAME "ProjectZomboid"


int init_socket() {

	int err, sock;
	struct group *group;
	struct sockaddr_un sun;

	memset(&sun, 0, sizeof(sun));
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path, SERVER_SOCKET);

	group = getgrnam(PZST_GROUP);
	if (!group) {
		perror("getgrname failed");
		exit(EXIT_FAILURE);
	}

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	err = unlink(SERVER_SOCKET);
	if (err && errno != ENOENT) {
		perror("unlink failed");
		exit(EXIT_FAILURE);
	}

	err = bind(sock, (struct sockaddr *)&sun, sizeof(sun));
	if (err) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	err = chmod(SERVER_SOCKET, 0666);
	if (err) {
		perror("chmod failed");
		exit(EXIT_FAILURE);
	}

	err = chown(SERVER_SOCKET, 0, group->gr_gid);
	if (err) {
		perror("chown failed");
		exit(EXIT_FAILURE);
	}

	err = listen(sock, SERVER_QUEUE);
	if (err) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	return sock;
}

int pz_ioctl(int fd, char c, char *strerr, size_t strerrsz) {

	int err;

	err = ioctl(fd, TIOCSTI, &c);
	if (err)
		snprintf(strerr, strerrsz, "ERROR: ioctl failed: %s", strerror(errno));

	return err;
}

int check_permission(pid_t pid, char *strerr, size_t strerrsz) {

	int fd;
	ssize_t size;
	char buf[32];

	snprintf(buf, sizeof(buf), "/proc/%d/comm", pid);

	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		snprintf(strerr, strerrsz, "ERROR: Open failed: %s", strerror(errno));
		return -1;
	}

	size = read(fd, buf, sizeof(buf));
	if (size < 0) {
		snprintf(strerr, strerrsz, "ERROR: Read failed: %s", strerror(errno));
		return -1;
	}

	buf[size - 1] = 0;

	if (strncmp(buf, PROC_NAME, strlen(PROC_NAME))) {
		snprintf(strerr, strerrsz, "ERROR: Permission denied: Not a pzserver pid");
		return -1;
	}

	close(fd);
	return 0;
}

int wait_pz_pid(pid_t pz_pid, char *strerr, size_t strerrsz) {

	int err;
	time_t start;

	start = time(NULL);

	while (1) {

		err = kill(pz_pid, 0);
		if (err && errno == ESRCH)
			break;

		if (time(NULL) - start > MAX_WAIT) {
			snprintf(strerr, strerrsz, "Something went wrong! Process not responding after %d seconds", MAX_WAIT);
			return -1;
		}

		usleep(200 * 1000);
	}

	return 0;
}

int send_command(struct ioctl_command *cmd, char *strerr, size_t strerrsz) {

	int fd;
	char path[32];

	if (check_permission(cmd->pid, strerr, strerrsz))
		return -1;

	snprintf(path, sizeof(path), "/proc/%d/fd/0", cmd->pid);

	fd = open(path, O_WRONLY);
	if (fd < 0) {
		snprintf(strerr, strerrsz,"ERROR: open failed: %s", strerror(errno));
		return -1;
	}

	for (int i = 0; i < (int)strlen(cmd->command); i++)
		if (pz_ioctl(fd, cmd->command[i], strerr, strerrsz))
			return -1;

	if (pz_ioctl(fd, '\n', strerr, strerrsz))
		return -1;

	close(fd);

	if (!strcmp(cmd->command, "quit"))
		return wait_pz_pid(cmd->pid, strerr, strerrsz);

	return 0;
}

void server_accept(int sock) {

	int csock;
	ssize_t size;
	char strerr[64];
	struct ioctl_command cmd;

	csock = accept(sock, NULL, NULL);
	if (csock < 0) {
		perror("accept failed");
		exit(EXIT_FAILURE);
	}

	size = recv(csock, &cmd, sizeof(cmd), 0);
	if (size < 0) {
		perror("recv failed");
		return;
	}

	if (!size)
		return;

	cmd.command[MAX_COMMAND_SIZE] = 0;

	strerr[0] = 0;

	send_command(&cmd, strerr, sizeof(strerr));

	size = send(csock, strerr, strlen(strerr) + 1, 0);
	if (size != (int)strlen(strerr) + 1) {
		perror("send failed");
		return;
	}
}

void server_loop(int sock) {

	pid_t pid;

	while (1) {

		pid = fork();
		if (!pid) {
			server_accept(sock);
			return;
		}

		if (pid < 0) {
			perror("fork failed");
			exit(EXIT_FAILURE);
		}

		waitpid(pid, NULL, 0);
	}
}

int main() {
	server_loop(init_socket());
	return 0;
}
