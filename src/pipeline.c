#include "pipeline.h"
#include <stdio.h>
#include <string.h>

// splits args on "|" into separate commands
int split_pipeline(char *args[], int numTokens, Cmd cmds[MAX_CMDS]) {
  int n = 0;
  int start = 0;

  for (int i = 0; i <= numTokens; i++) {
    if (i != numTokens && strcmp(args[i], "|") != 0)
      continue;

    int len = i - start;
    if (len == 0) {
      fprintf(stderr, "shell: syntax error near unexpected token '|'\n");
      return -1;
    }
    if (n == MAX_CMDS) {
      fprintf(stderr, "shell: too many pipes\n");
      return -1;
    }

    for (int k = 0; k < len; k++)
      cmds[n].argv[k] = args[start + k];
    cmds[n].argv[len] = NULL;

    n++;
    start = i + 1;
  }

  return n;
}