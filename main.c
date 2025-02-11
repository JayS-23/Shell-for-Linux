#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_LINE 80 /* The maximum length command */
#define MAX_History_Length 10 // Maximum length for history array

int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* an array of character strings */
    int should_run = 1; /* flag to determine when to exit program */
    char input[80];
    char *historyArr[MAX_History_Length] = {NULL};
    pid_t pidArr[MAX_History_Length] = {NULL};
    int index =0;

    while(should_run) {
        int n = 1;
        printf("CSCI3120>");
        fflush(stdout);
        fgets(input, 80, stdin);
        historyArr[index] = input;

        char* token;
        token = strtok(input, " \n");
        if (token == NULL) {
            printf("Loop breaking, message from pid: %d", getpid());
            break;
        }
        args[0] = token;
        while (token != NULL) {
            token = strtok(NULL, " \n");
            args[n] = token;
            n++;
        }
        printf("[");
        for (int i=0; i<n-1; i++) {
            printf("\"%s\", ", args[i]);
        }
        printf("\"%s\"]\n", args[n-1]);

        pid_t pid = fork();
        if (pid < 0) {
            printf("Child process creation failed :(\n");
            exit(1);
        }
        if (pid == 0) {
            printf("Child process created successfully with PID: %d\n",getpid() );
            if (strcmp(args[0], "history") == 0) {
                printf("%-6s %-6s %-6s\n", "ID", "PID", "Command");
                int i = index-1;
                for (int i=0, curr = index-1; i<MAX_History_Length; i++) {
                    printf("%-6d %-6s %-6s\n", i, pidArr[curr] , historyArr[curr]);

                    curr = (curr + 9) % 10; // Its basically curr - 1, and then to remove the negative values
                                            // and to wrap around I took %10 after adding 10 to it.
                }

            }
            else if (execvp(args[0], args) == -1) {
                printf("Error: %s\n", strerror(errno));
                exit(1);
            }
        }
        else {
            printf("Child process created successfully with Chile PID: %d\n",pid );
            int status;
            waitpid(pid, &status, 0);
            pidArr[index] = pid;
            index = (index + 1) % MAX_History_Length;
        }
    }
    return 0;
}