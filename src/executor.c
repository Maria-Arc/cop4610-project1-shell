#include "executor.h"
#include "expand.h"
#include "external.h"
#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Frees every argv[0] that Path() malloc'd for stages [0, upto).
static void free_resolved(Cmd cmds[], const int path_alloc[], int upto) {
  for (int i = 0; i < upto; i++)
    if (path_alloc[i])
      free(cmds[i].argv[0]);
}

void run_pipeline(Cmd cmds[MAX_CMDS], int n, int background, const char *cmdline) {
  int path_alloc[MAX_CMDS] = {0};
  for (int i = 0; i < n; i++) {
    if (strchr(cmds[i].argv[0], '/') != NULL)
      continue; // has a slash: use as-is, no search needed
    int changed[1] = {0};
    int r = Path(&cmds[i].argv[0], changed);
    if (r <= 0) {
      // Path() already printed "<cmd>: Command not found." when r == 0.
      if (r < 0)
        fprintf(stderr, "shell: PATH is not set\n");
      free_resolved(cmds, path_alloc, i);
      return; // nothing forked yet, so nothing to clean up
    }
    path_alloc[i] = 1;
  }

  if (n == 1) {
    if (background) {
      // Background: start it but don't wait (Part 8 hands the pid to jobs.c)
      pid_t pid = SpawnCommand(cmds[0].argv);
      free_resolved(cmds, path_alloc, 1); // child has its own copy after fork
      if (pid > 0)
        add_job(&pid, 1, cmdline);
    } else {
      ExecuteCommand(cmds[0].argv);
      free_resolved(cmds, path_alloc, 1);
    }
    return;
  }

  int pipefd[MAX_CMDS - 1][2];
  for (int i = 0; i < n - 1; i++) {
    if (pipe(pipefd[i]) == -1) {
      perror("pipe");
      for (int j = 0; j < i; j++) { // close pipes opened so far
        close(pipefd[j][0]);
        close(pipefd[j][1]);
      }
      free_resolved(cmds, path_alloc, n);
      return;
    }
  }

  pid_t pids[MAX_CMDS];
  int forked = 0; // how many stages got a child

  for (int i = 0; i < n; i++) {
    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      break; // fall through to cleanup; already-started stages are reaped below
    }

    if (pid == 0) {
      if (i > 0)
        dup2(pipefd[i - 1][0], STDIN_FILENO);
      if (i < n - 1)
        dup2(pipefd[i][1], STDOUT_FILENO);
      for (int j = 0; j < n - 1; j++) {
        close(pipefd[j][0]);
        close(pipefd[j][1]);
      }

      execv(cmds[i].argv[0], cmds[i].argv);
      perror(cmds[i].argv[0]);
      _exit(127);
    }

    pids[forked++] = pid;
  }

  for (int j = 0; j < n - 1; j++) {
    close(pipefd[j][0]);
    close(pipefd[j][1]);
  }

  free_resolved(cmds, path_alloc, n);

  if (forked < n) {
    for (int i = 0; i < forked; i++)
      waitpid(pids[i], NULL, 0);
    return;
  }

  // Part 8: foreground vs. background 
  if (background) {
    add_job(pids, n, cmdline);
  } else {
    int status;
    for (int i = 0; i < n; i++)
      waitpid(pids[i], &status, 0);
  }
}
