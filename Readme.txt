CSCI3120 A Simple Shell Program – Readme
================================
By  : Sanghani Jay N.
B00 : B00937799

Description:
-------------
This program implements a simple shell that reads user commands, supports a history of up to 10 commands,
and can re-execute commands using special syntax such as:
  - "!!"  : Re-execute the most recent command.
  - "!N"  : Re-execute the Nth command from the history (where N is a digit 1-10).
Additionally, the "history" command prints the list of stored commands (with their PIDs).
The shell executes commands using a forked child process and execvp(), and it exits when the
user types "exit".

Compilation:
-------------
You can compile the program:

    **Using the provided Makefile:**

   *A Makefile is included*

   To compile the program, simply execute:
       make OR gcc main.c (either of them)

Execution:
----------
After successful compilation, run the program by executing:
    make run OR ./shell (either of them)

To clean up (remove the executable), run:
    make clean

Upon running, the shell will display the prompt:
       CSCI3120>

You can then enter any system command. The following are special commands supported by the shell:

   - exit: Terminates the shell.
   - history: Displays a list of the last 10 commands along with their corresponding process IDs.
   - !!: Executes the most recent command from the history.
   - !N: Executes the Nth command from the history (e.g., !3 executes the 3rd most recent command).

Thank you!