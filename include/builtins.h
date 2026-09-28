#ifndef BUILTINS_H
#define BUILTINS_H

// true if args[0] is exit, cd, or jobs
int is_builtin(char *args[]);

// runs the matching builtin. exit() happens inside here for "exit"
void run_builtin(char *args[], int argc, const char *cmdline);

// adds a command to the last-3-commands history
void record_history(const char *cmdline);

#endif