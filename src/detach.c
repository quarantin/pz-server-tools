#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pty.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int search(char *program, char *filepath, size_t filepathsz) {

	char *dir;
	struct stat info;
	char envpath[BUFSIZ];

	memset(&info, 0, sizeof(info));
	snprintf(filepath, filepathsz, "%s", program);
	if (!stat(filepath, &info))
		return 0;

	snprintf(envpath, sizeof(envpath), "%s", getenv("PATH"));
	if (!*envpath)
		return -1;

	dir = strtok(envpath, ":");
	while (dir) {

		memset(&info, 0, sizeof(info));
		snprintf(filepath, filepathsz, "%s/%s", dir, program);
		if (!stat(filepath, &info))
			return 0;

		dir = strtok(NULL, ":");
	}

	return -1;
}

#ifdef PZWRAP

void write_pid(pid_t pid) {

	int fd;
	char buf[32];

	fd = open("/var/cache/pzst/pzserver.pid", O_CREAT|O_WRONLY|O_TRUNC, 0644);
	if (fd == -1) {
		perror("open failed");
		return;
	}

	snprintf(buf, sizeof(buf), "%d", pid);
	write(fd, buf, strlen(buf));
	close(fd);
}

#endif

int main(int argc, char **argv, char **envp) {

	pid_t pid;
	int status;
	char exepath[BUFSIZ];

	if (argc < 2) {
		printf("Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (search(argv[1], exepath, sizeof(exepath))) {
		fprintf(stderr, "%s: %s: command not found\n", argv[0], argv[1]);
		exit(EXIT_FAILURE);
	}

#ifdef DETACH

	int master;
	pid = forkpty(&master, NULL, NULL, NULL);

#elif PZWRAP

	pid = fork();

#else

	#error Specify DETACH or PZWRAP

#endif

	if (!pid) {
		execve(exepath, &argv[1], envp);
		perror("execve failed");
		exit(EXIT_FAILURE);
		return 0;
	}

#ifdef PZWRAP
	else {
		write_pid(pid);
	}
#endif

	wait(&status);
	exit(!WIFEXITED(status) || WEXITSTATUS(status));
}
