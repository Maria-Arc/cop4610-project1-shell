#include "expand.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  char line[500];
  char *args[100];
  const char *USER = getenv("USER");
  const char *MACHINE = getenv("MACHINE");
  char *pwd = getenv("PWD");
  int needToFree[100];
  int numTokens;

  // USER@MACHINE:PWD>
  printf("%s@%s:%s> ", USER, MACHINE, pwd);
  while (fgets(line, sizeof(line), stdin) != NULL) {

    numTokens = Tokenize(line, args);
    for (int i = 0; i < numTokens; i++) {
      printf("%d: \t %s\n", i, args[i]);
      needToFree[i] = 0;
    }

    TildeExpansion(args, numTokens, needToFree);
    Path(&args[0], needToFree);

    if (args[0] != NULL && strcmp(args[0], "exit") == 0)
      return 0;

    printf("%s@%s:%s> ", USER, MACHINE, pwd);
  }

  return 0;
}

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
