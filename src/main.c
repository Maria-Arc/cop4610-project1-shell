// This is the new main loop (previously main() lived, in debug form, in tokenizer.c
#include "builtins.h"
#include "env.h"
#include "executor.h"
#include "expand.h"
#include "jobs.h"
#include "pipeline.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void print_prompt(void) {
  const char *user = getenv("USER");
  const char *machine = getenv("MACHINE");
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) == NULL)
    strcpy(cwd, "?");
  printf("%s@%s:%s> ", user ? user : "?", machine ? machine : "?", cwd);
  fflush(stdout);
}

// Strips a trailing '\n' from a line in place
static void strip_newline(char *line) {
  size_t len = strlen(line);
  if (len > 0 && line[len - 1] == '\n')
    line[len - 1] = '\0';
}

// Lets the userhit Enter on a blank line without it being treated as a command
static int is_blank(const char *line) {
  for (const char *p = line; *p != '\0'; p++)
    if (*p != ' ' && *p != '\t')
      return 0;
  return 1;
}

int main(void) {
  char line[MAX_LINE];
  char raw[MAX_LINE];
  char *args[MAX_TOKENS + 1];
  int needToFree[MAX_TOKENS]; // needToFree[i]: was args[i] malloc'd by TildeExpansion()?

  while (1) {
    poll_jobs();
    print_prompt();

    if (fgets(line, sizeof(line), stdin) == NULL) {
      printf("\n");
      break; // EOF (e.g. Ctrl-D): fall through to the same cleanup exit() does
    }
    strip_newline(line);
    if (is_blank(line))
      continue; // don't tokenize, execute, or record an empty line

    strcpy(raw, line); // snapshot BEFORE Tokenize() destroys the spaces

    int numTokens = Tokenize(line, args);
    if (numTokens == 0)
      continue;

    int background = 0;
    if (strcmp(args[numTokens - 1], "&") == 0) {
      background = 1;
      numTokens--;
      args[numTokens] = NULL; // re-terminate argv now that "&" is gone
    }
    if (numTokens == 0)
      continue; // the entire line was just "&" -- nothing to run

    // --- Part 2/3: expansion applies to every token, before dispatch ---
    for (int i = 0; i < numTokens; i++)
      needToFree[i] = 0;
    EnvExpansion(args, numTokens, needToFree);
    TildeExpansion(args, numTokens, needToFree);
    char cmdline[CMDLINE_LEN];
    strncpy(cmdline, raw, CMDLINE_LEN - 1);
    cmdline[CMDLINE_LEN - 1] = '\0';
    if (background) {
      char *amp = strrchr(cmdline, '&');
      if (amp != NULL) {
        *amp = '\0';
        // also eat any whitespace that was between the command and the "&"
        while (amp > cmdline && (amp[-1] == ' ' || amp[-1] == '\t'))
          *(--amp) = '\0';
      }
    }

    // Part 9 vs. Parts 5/6/7/8 dispatch 
    if (is_builtin(args)) {
      run_builtin(args, numTokens, cmdline); // exit() terminates inside here
    } else {
      Cmd cmds[MAX_CMDS];
      int n = split_pipeline(args, numTokens, cmds); // Part 7 split + Part 6 redirection extraction
      if (n > 0) {
        run_pipeline(cmds, n, background, cmdline); // Parts 5/6/7/8
        record_history(cmdline);                     // Part 9 history
      }
    }

    for (int i = 0; i < numTokens; i++)
      if (needToFree[i])
        free(args[i]);
  }

  wait_all_jobs();
  return 0;
}
