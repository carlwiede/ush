#include <stdio.h>

#include "commands.h"

// List of builtin commands 
// and their corresponding functions

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &ush_cd,
    &ush_help,
    &ush_exit
};

int ush_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// Builtin function implementations
int ush_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "ush: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("ush");
        }
    }
    return 1;
}

int ush_help (char **args)
{
    int i;
    printf("Carl Wiede's USH\n");
    printf("Type the program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for(i = 0; i < ush_num_builtins(); i++) {
        printf("    %s\n", builtin_str[i]);
    }

    return 1;
}

int ush_exit(char **args)
{
    return 0;
}