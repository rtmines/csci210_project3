#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int main() {

    char line[256];

    while (1) {

		fprintf(stderr,"rsh>");

		if (fgets(line,256,stdin)==NULL) continue;

		if (strcmp(line,"\n")==0) continue;

		line[strlen(line)-1]='\0';

		char* command = strtok(line, " ");

		if (strcmp(command, "exit") == 0) {
			return 0;
		}
		if (strcmp(command, "help") == 0) {
			fprintf(stdout, "The allowed commands are:\n");
			fprintf(stdout, "1: cp\n");
			fprintf(stdout, "2: touch\n");
			fprintf(stdout, "3: mkdir\n");
			fprintf(stdout, "4: ls\n");
			fprintf(stdout, "5: pwd\n");
			fprintf(stdout, "6: cat\n");
			fprintf(stdout, "7: grep\n");
			fprintf(stdout, "8: chmod\n");
			fprintf(stdout, "9: diff\n");
			fprintf(stdout, "10: cd\n");
			fprintf(stdout, "11: exit\n");
			fprintf(stdout, "12: help\n");
			continue;
		}

		int valid = 0;
		for (int i = 0; i < 10; i++) {
			if (strcmp(command, allowed[i]) == 0) {
				valid = 1;
			}
		}

		if (!valid) {
			fprintf(stdout, "NOT ALLOWED!\n");
			continue;
		}

		if (strcmp(command, "cd") == 0) {
			char* arg = strtok(NULL, " ");
			char* second = strtok(NULL, " ");

			if (second != NULL) {
				fprintf(stdout, "-rsh: cd: too many arguments\n");
				continue;
			}

			chdir(arg);
			continue;
		}

		char *argv[20];
		argv[0] = command;

		for (int i=1; i<20; i++) {
			char* arg = strtok(NULL, " ");
			argv[i] = arg;
			if (arg == NULL) {
				break;
			}
		}


		pid_t pid;
		int status;
		posix_spawnattr_t attr;
	
		// Initialize spawn attributes
		posix_spawnattr_init(&attr);
	
		// Set flags if needed, for example, to specify the scheduling policy
		// posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSCHEDULER);
	
		// Spawn a new process
		if (posix_spawnp(&pid, command, NULL, &attr, argv, environ) != 0) {
			perror("spawn failed");
			exit(EXIT_FAILURE);
		}
	
		// Wait for the spawned process to terminate
		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid failed");
			exit(EXIT_FAILURE);
		}
	
		// Destroy spawn attributes
		posix_spawnattr_destroy(&attr);


		// TODO
		// Add code to spawn processes for the first 9 commands
		// And add code to execute cd, exit, help commands
		// Use the example provided in myspawn.c

    }
    return 0;
}
