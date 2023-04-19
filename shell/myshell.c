#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int cd(char *path) {
    if (chdir(path) != 0) {
        perror("cd failed");
        return -1;
    }
    return 0;
}

void add_directories_to_path(int size, char *dirs[]) {
    // for every directory in dirs array add it to the path
    for (int i = 1; i < size; i++) {
        char *dir = dirs[i];
        char *path = getenv("PATH");
        char *new_path = malloc(strlen(path) + strlen(dir) + 2);
        sprintf(new_path, "%s:%s", dir, path);
        setenv("PATH", new_path, 1);
        free(new_path);
        printf("dir: %s\n", dir);
        printf("path: %s\n", getenv("PATH"));
    }
}

void run(char *args[100], char *history[100], int *history_index) {
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        // child process
        // add the pid and the command to the history
        execvp(args[0], args);
        exit(0);
    } else if (pid > 0) {
        // parent process
        history[*history_index] = malloc(102);
        sprintf(history[*history_index], "%d %s", pid, args[0]);
        *history_index = *history_index + 1;
        wait(NULL);
    } else {
        // error
        perror("fork failed");
    }
}

void get_args(char *cmd_args, char *args[100]) {
    int i = 0;
    char *token = strtok(cmd_args, " ");
    while (1) {
        if (token == NULL) {
            args[i] = NULL;
            break;
        }
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
}

void history_print(int size, char *history[100], int history_index) {
    // print the history
    for (int i = 0; i < history_index; i++) {
        if (history[i] != NULL) {
            printf("%s\n", history[i]);
        }
    }
//    printf("%s\n", history[history_index]);
}


// this main function is working as a shell
int main(int argc, char *argv[]) {
    // commands history array shared between all processes
    char *history[100];
    int history_index = 0;
    add_directories_to_path(argc, argv); // add directories to the path
    char *cmd_args = malloc(102);

    while (1) {
        printf("$ ");
        fflush(stdout);
        fgets(cmd_args, 102, stdin);
        cmd_args[strcspn(cmd_args, "\n")] = 0; // remove newline character from the end of the string
        if (strcmp(cmd_args, "") == 0) {
            continue;
        }
        if (strcmp(cmd_args, "exit") == 0) {
            free(cmd_args);
            exit(0);
        }

        // split the rest of the string into args
        char *args[100];
        get_args(cmd_args, args);

        if (strcmp(args[0], "cd") == 0) {
            if (cd(args[1]) == 0) {
                // add the pid and the command to the history
                history[history_index] = malloc(102);
                sprintf(history[history_index], "%d %s", getpid(), cmd_args);
                history_index++;
            }
            continue;
        }

        if (strcmp(args[0], "history") == 0) {
            // add the pid and the command to the history
            history[history_index] = malloc(102);
            sprintf(history[history_index], "%d %s", getpid(), cmd_args);
            history_index++;
            history_print(100, history, history_index);
            continue;
        }

        run(args, history, &history_index);

    }
}
