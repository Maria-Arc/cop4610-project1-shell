#ifndef SHELL_H
#define SHELL_H

// Shared limits used across every module
#define MAX_LINE 250      // each command is less than 200 characters long
#define MAX_TOKENS 200    // generous upper bound on token count per line
#define MAX_CMDS 3        // at most 3 commands per line
#define MAX_JOBS 10       // no more than 10 background processes
#define HIST_SIZE 3        // exit prints "the last three valid commands"
#define CMDLINE_LEN 201    // command-line string storage

int Tokenize(char* line, char* args[]);

#endif
