#include "src/Cmd.h"
#include <stdio.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("\n> ");
        if (!fgets(input, MAX_INPUT_SIZE, stdin)) break;  // EOF or error
        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) continue;

        
        char *cmd = strtok(input, " ");
        char *arg = strtok(NULL, " ");

        if (strcmp(cmd, "exit") == 0) {
            cmd_exit(0);
        } else if (strcmp(cmd, "ls") == 0) {
            cmd_ls(arg);
        } else if (strcmp(cmd, "cd") == 0) {
            if (arg)
                cmd_cd(arg);
            else
                printf("cd: missing argument\n");
        } else if (strcmp(cmd, "pwd") == 0) {
            cmd_pwd();
        } else if (strcmp(cmd, "cat") == 0) {
            if (arg)
                cmd_cat(arg);
            else
                printf("cat: missing argument\n");
        } else {
            printf("Unknown command: %s\n", cmd);
        }
    }

    return 0;
}
