#ifndef PIPELINE_H
#define PIPELINE_H

#include "shell.h"

// One command in a pipeline, one command if no |
// argv points at the same tokens as the main args[] array
typedef struct {
  char *argv[MAX_TOKENS + 1];
} Cmd;

// Splits tokens on "|" up to MAX_CMDS commands.
int split_pipeline(char *args[], int numTokens, Cmd cmds[MAX_CMDS]);

#endif
