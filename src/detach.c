#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pty.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int endswith(char *string, char *pattern) {

	size_t slen = strlen(string);
	size_t plen = strlen(pattern);

	return slen >= plen && !strcmp(string + slen - plen, pattern);
}

int main(int argc, char **argv, char **envp) {

	pid_t pid;
	int master, status;

	if (argc < 2) {
		printf("Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (endswith(argv[0], "detach"))
		pid = forkpty(&master, NULL, NULL, NULL);

	else
		pid = fork();

	if (!pid) {
		execve(argv[1], &argv[1], envp);
		perror("execve failed");
		exit(EXIT_FAILURE);
		return 0;
	}

	wait(&status);
	exit(!WIFEXITED(status) || WEXITSTATUS(status));
}
