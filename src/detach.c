#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pty.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
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

int endswith(char *string, char *pattern) {

	size_t slen = strlen(string);
	size_t plen = strlen(pattern);

	return slen >= plen && !strcmp(string + slen - plen, pattern);
}

int main(int argc, char **argv, char **envp) {

	pid_t pid;
	int master, status;
	char exepath[BUFSIZ];

	if (argc < 2) {
		printf("Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (search(argv[1], exepath, sizeof(exepath))) {
		fprintf(stderr, "%s: %s: command not found\n", argv[0], argv[1]);
		exit(EXIT_FAILURE);
	}

	if (endswith(argv[0], "detach"))
		pid = forkpty(&master, NULL, NULL, NULL);

	else
		pid = fork();

	if (!pid) {
		execve(exepath, &argv[1], envp);
		perror("execve failed");
		exit(EXIT_FAILURE);
		return 0;
	}

	wait(&status);
	exit(!WIFEXITED(status) || WEXITSTATUS(status));
}
