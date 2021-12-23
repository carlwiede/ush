// Function declarations of builtins

int ush_cd(char **args);
int ush_help(char **args);
int ush_exit(char **args);

// Declaration of function names and their pointers
char *builtin_str[];
int (*builtin_func[]) (char **);