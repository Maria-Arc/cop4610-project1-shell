#include "external.h"
#include "redirection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// the fork/exec half ExecuteCommand, pulled out so the background code (Part 8) can start a command without waiting for it.
pid_t SpawnCommand(char *args[]) {
  char *commandArgs[200];
  int commandIndex = 0;
  // arg list without the "<" / ">" symbols and their file names
  for (int i = 0; args[i] != NULL; i++) {
    if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0) {
      i++; // skip the symbol AND the file name after it
    } else {
      commandArgs[commandIndex] = args[i];
      commandIndex++;
    }
  }
  commandArgs[commandIndex] = NULL;

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return -1;
  }

  if (pid == 0) {
    if (RedirectIO(args) == -1)
      _exit(1);
    // replace child process with the command
    execv(commandArgs[0], commandArgs);
    perror("execv");
    _exit(1);
  }
  return pid;
}

int ExecuteCommand(char *args[]) {
  pid_t pid = SpawnCommand(args);
  if (pid < 0)
    return -1;

  // waiting for the child process to finish before we continue
  int status;
  if (waitpid(pid, &status, 0) == -1) {
    perror("waitpid");
    return -1;
  }
  return 0;
}
