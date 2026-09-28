#include "builtins.h"
#include "jobs.h"
#include "shell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char history[HIST_SIZE][CMDLINE_LEN];
static int hist_len = 0;   // how many slots are filled (0..HIST_SIZE)
static int hist_count = 0; // total valid commands ever entered

int is_builtin(char *args[]) {
  if (args[0] == NULL)
    return 0;
  return strcmp(args[0], "exit") == 0 || strcmp(args[0], "cd") == 0 ||
         strcmp(args[0], "jobs") == 0;
}

void record_history(const char *cmdline) {
  if (hist_len < HIST_SIZE) {
    strncpy(history[hist_len], cmdline, CMDLINE_LEN - 1);
    history[hist_len][CMDLINE_LEN - 1] = '\0';
    hist_len++;
  } else {
    // slide everything down one slot, newest goes on the end
    for (int i = 0; i < HIST_SIZE - 1; i++)
      strcpy(history[i], history[i + 1]);
    strncpy(history[HIST_SIZE - 1], cmdline, CMDLINE_LEN - 1);
    history[HIST_SIZE - 1][CMDLINE_LEN - 1] = '\0';
  }
  hist_count++;
}

static void builtin_cd(char *args[], int argc) {
  if (argc == 1) {
    char *home = getenv("HOME");
    if (home == NULL) {
      fprintf(stderr, "cd: HOME not set\n");
      return;
    }
    if (chdir(home) != 0)
      perror("cd");
    return;
  }
  if (argc > 2) {
    fprintf(stderr, "cd: too many arguments\n");
    return;
  }
  if (chdir(args[1]) != 0) {
    if (errno == ENOTDIR)
      fprintf(stderr, "cd: %s: Not a directory\n", args[1]);
    else if (errno == ENOENT)
      fprintf(stderr, "cd: %s: No such file or directory\n", args[1]);
    else
      perror("cd");
  }
}

static void builtin_exit(const char *cmdline) {
  record_history(cmdline); // exit counts as a command too

  wait_all_jobs(); // don't quit while background stuff is still running

  if (hist_count == 0) {
    printf("No valid commands were entered.\n");
  } else {
    printf("Last (%d) valid command%s:\n", hist_len, hist_len == 1 ? "" : "s");
    for (int i = 0; i < hist_len; i++)
      printf("[%d]: %s\n", i + 1, history[i]);
  }

  exit(0);
}

void run_builtin(char *args[], int argc, const char *cmdline) {
  if (strcmp(args[0], "exit") == 0) {
    builtin_exit(cmdline); // never returns
  } else if (strcmp(args[0], "cd") == 0) {
    builtin_cd(args, argc);
    record_history(cmdline);
  } else if (strcmp(args[0], "jobs") == 0) {
    print_jobs();
    record_history(cmdline);
  }
}