// main() has been REMOVED from here and replaced by the real shell loop in the new src/main.c
#include "shell.h"
#include <stdio.h>
#include <string.h>

// replaces spaces with \0 then points args[i] to start of word
// args[0] is a ptr that points to 0th word in line and so on
int Tokenize(char *line, char *args[]) {
  int n = 0;
  char *token = strtok(line, " \n");
  while (token != NULL) {
    args[n] = token;
    n++;
    token = strtok(NULL, " \n");
  }
  args[n] = NULL;
  return n;
}
