#include <stdio.h>
#include <stdlib.h>

#define USH_RL_BUFSIZE 1024 // That's a lot of characters

int ush_execute(char **args)
{
    
}

char **ush_split_line(char *line)
{

}

// Old school line reading because who needs getline()
char *ush_read_line(void)
{
    int bufsize = USH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    // Hold the phone
    if (!buffer) {
        fprintf(stderr, "ush: allocation error\n");
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
                fprintf(stderr, "ush: reallocation error \n");
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