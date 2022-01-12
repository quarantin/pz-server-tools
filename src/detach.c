#include <stdio.h>
#include <stdlib.h>

#include <pty.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char **argv, char **envp) {

	pid_t pid;
	int master, status;

	if (argc < 2) {
		printf("Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	pid = forkpty(&master, NULL, NULL, NULL);
	if (!pid) {
		execve(argv[1], &argv[1], envp);
		perror("execve failed");
		exit(EXIT_FAILURE);
		return 0;
	}

	wait(&status);
	exit(!WIFEXITED(status) || WEXITSTATUS(status));
}
