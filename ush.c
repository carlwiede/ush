#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define USH_RL_BUFSIZE 1024 // That's a lot of characters
#define USH_TOK_BUFSIZE 64 // No more tokens than this please
#define USH_TOK_DELIM " \t\r\n\a" // Delimiters

int ush_cd(char **args);
int ush_help(char **args);
int ush_exit(char **args);

int ush_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child
        if (execvp(args[0], args) == -1) {
            perror("ush"); // Oof
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Forking error
        perror("ush");
    } else {
        // Parent
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    // Great success
    return 1;
}

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

int ush_execute(char **args)
{
    int i;

    if (args[0] == NULL) {
        // Empty command
        return 1;
    }

    // Pattern match args to available built-ins
    for (i = 0; i < ush_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return ush_launch(args);
}

// Parsing function very similar to reading function
char **ush_split_line(char *line)
{
    int bufsize = USH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    // Sanity check
    if(!tokens) {
        fprintf(stderr, "ush: token allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Get first token
    token = strtok(line, USH_TOK_DELIM);

    // Iterate for rest of the tokens
    while (token != NULL) {
        tokens[position] = token;
        position++;

        // Dynamic token amount
        if (position >= bufsize) {
            bufsize += USH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            // Insanity check
            if (!tokens) {
                fprintf(stderr, "ush: token reallocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        // Get next token
        token = strtok(NULL, USH_TOK_DELIM);
    }
    // Null-terminate
    tokens[position] = NULL;
    return tokens;
}

// Old school line reading because who needs getline()
char *ush_read_line(void)
{
    int bufsize = USH_RL_BUFSIZE, position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    // Hold the phone
    if (!buffer) {
        fprintf(stderr, "ush: line allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read character
        c = getchar();

        // EOF control
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        // Increase buffer size and reallocate if more memory is required
        if (position >= bufsize) {
            bufsize += USH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);

            // Hold the phone again
            if (!buffer) {
                fprintf(stderr, "ush: line reallocation error \n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

// Read, parse, execute
void ush_loop(void)
{
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = ush_read_line();
        args = ush_split_line(line);
        status = ush_execute(args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char **argv)
{

    // Initialize
    

    // Interpret
    ush_loop();

    // Terminate
    return EXIT_SUCCESS;
}