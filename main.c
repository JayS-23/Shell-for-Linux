#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_LINE 80 /* The maximum length command */
#define MAX_History_Length 10 // Maximum length for history array

/**
 * Prints command from the history Array in the order such as the Top most is used the latest. Maximum size is 10.
 */
void printHistory(char *historyArr[], pid_t pidArr[], int index) {
    printf("%-6s %-6s %-6s\n", "ID", "PID", "Command");
    int curr = (index - 1 + MAX_History_Length) % MAX_History_Length;
    for (int i=0; i<MAX_History_Length; i++) {
        if (historyArr[curr] == NULL) {
            break;
        }
        printf("%-6d %-6d %-6s\n", i+1, pidArr[curr] , historyArr[curr]);
        curr = (curr -1 + MAX_History_Length) % MAX_History_Length;
        // Its basically curr - 1, and then to remove the negative values and to wrap around I did + 10 ) % 10.
    }
}

/**
 * Executes the last added command in the history Array.
 */
int executeLatest(char *historyArr[], char input[], int index) {
    if (input[2] != '\0') {
        printf("Error: Invalid command!\n");
        return 1;
    }
    char *lastCommand = historyArr[(index - 1 + MAX_History_Length) % MAX_History_Length];
    if (lastCommand == NULL) {
        printf("Error: No command in history!\n");
        return 2;
    }
    strcpy(input, lastCommand);
    return 0;
}

/**
 * Executes the Nth command from the last added command in the history Array.
 */
int executeNth(char *historyArr[], char input[], int index) {
    int N = atoi(&input[1]);
    if (N<1 || N>MAX_History_Length) {
        printf("Error: Invalid command!\n");
        return 1;
    }
    char *NthCommand = historyArr[((index - 1) - (N - 1) + MAX_History_Length) % MAX_History_Length];
    if (historyArr[(index - 1 + MAX_History_Length) % MAX_History_Length] == NULL) {
        printf("Error: No command in history!\n");
        return 2;
    }
    if (NthCommand == NULL) {
        printf("Error: Such a command is NOT in history!\n");
        return 3;
    }
    strcpy(input, NthCommand);
    return 0;
}

/**
 * Splits the input string into tokens and store it in input[]
 */
void tokenizer(char input[], char* args[]) {
    int n=1;
    char* token = strtok(input, " ");
    args[0] = token;
    while(token != NULL) {
        token = strtok(NULL, " ");
        args[n] = token;
        n++;
    }
}

/**
 * Creats a child process by forking and executes the execvp() which will execute the
 * program further depending on the user inputs. Also, it updates the index before
 * returning it.
 */
int forkAndExecute(char* args[], pid_t pidArr[], int index) {
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
        wait(NULL);
        pidArr[index] = pid;
        return  (index + 1) % MAX_History_Length;
    }
}

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

// Reads input using fgets from standard input
        fgets(input, MAX_LINE, stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) {
            printf("Error: No Command entered!\n");
            continue;
        }
// Exits the program upon entering exit
        if (strcmp(input, "exit") == 0) {
            should_run = 0;
            continue;
        }
// For history
        if (strcmp(input, "history") == 0) {
            printHistory(historyArr, pidArr, index);
            continue;
        }
// For !!
        if (strncmp(input, "!!", 2) == 0) {
            int result = executeLatest(historyArr, input, index);
            if (result != 0) continue;
        }
// For !N
        if (input[0] == '!' && isdigit(input[1])) {
           int result = executeNth(historyArr, input, index);
            if (result != 0) continue;
        }
// Freeing memory before storing to avoid memory leaks
        if (historyArr[index] != NULL) {
            free(historyArr[index]);
        }
        historyArr[index] = strdup(input); // strdup() to create a string copy

// Tokenize the input received
        tokenizer(input, args);

// Fork and execute the command in the child process
        index = forkAndExecute(args, pidArr, index);
    }

// Free all allocated memory before exiting
    for (int i = 0; i < MAX_History_Length; i++) {
        if (historyArr[i] != NULL) {
            free(historyArr[i]);
        }
    }
    return 0;
}
