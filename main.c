#include <ctype.h>
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
    pid_t pidArr[MAX_History_Length] = {0};
    int index =0;

    while(should_run) {
        int n = 1;
        printf("CSCI3120>");
        fflush(stdout);

        fgets(input, MAX_LINE, stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) {
            printf("Error: No Command entered!\n");
            continue;
        }
        if (strcmp(input, "exit")== 0) {
            should_run = 0;
            continue;
        }

        if (strcmp(input, "history")== 0) {
            printf("%-6s %-6s %-6s\n", "ID", "PID", "Command");
            int curr = (index - 1 + MAX_History_Length) % MAX_History_Length;
            for (int i=0; i<MAX_History_Length; i++) {
                if (historyArr[curr] == NULL) {
                    break;
                }
                printf("%-6d %-6d %-6s\n", i+1, pidArr[curr] , historyArr[curr]);
                curr = (curr -1 + MAX_History_Length) % MAX_History_Length;
                // Its basically curr - 1, and then to remove the negative
                // values and to wrap around I did + 10 ) % 10.
            }
            continue;
        }

        if (strncmp(input, "!!", 2) == 0) {
            if (input[2] != '\0') {
                printf("Error: Invalid command!\n");
                continue;
            }
            char *lastCommand = historyArr[(index - 1 + MAX_History_Length) % MAX_History_Length];
            if (lastCommand == NULL) {
                printf("Error: No command in history!\n");
                continue;
            }
                strcpy(input, lastCommand);
        }

        if (input[0] == '!' && isdigit(input[1])) {
            int N = atoi(&input[1]);
            if (N<1 || N>MAX_History_Length) {
                printf("Error: Invalid command!\n");
                continue;
            }
            char *NthCommand = historyArr[((index - 1) - (N - 1) + MAX_History_Length) % MAX_History_Length];
            if (historyArr[(index - 1 + MAX_History_Length) % MAX_History_Length] == NULL) {
                printf("Error: No command in history!\n");
                continue;
            }
            if (NthCommand == NULL) {
                printf("Error: Such a command is NOT in history!\n");
                continue;
            }
            strcpy(input, NthCommand);
        }

        if (historyArr[index] != NULL) {
            free(historyArr[index]);
        }
        historyArr[index] = strdup(input); // strdup() to create a string copy
        char* token;
        token = strtok(input, " ");

        args[0] = token;
        while (token != NULL) {
            token = strtok(NULL, " ");
            args[n] = token;
            n++;
        }

        pid_t pid = fork();
        if (pid < 0) {
            printf("Error: Child process creation failed :(\n");
            exit(1);
        }
        if (pid == 0) {
            if (execvp(args[0], args) == -1) {
                printf("Error: %s\n", strerror(errno));
                exit(1);
            }
        }
        else {
            int status;
            waitpid(pid, &status, 0);
            pidArr[index] = pid;
            index = (index + 1) % MAX_History_Length;
        }
    }

    // Free all allocated memory before exiting
    for (int i = 0; i < MAX_History_Length; i++) {
        if (historyArr[i] != NULL) {
            free(historyArr[i]);
        }
    }
    return 0;
}
