#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// fixed a Path() bug, commands with / were wrongly reported "not found"
int Path(char **command, int changed[]) {
  if (strchr(*command, '/'))
    return 0;

  char *ogPath = getenv("PATH");
  if (ogPath == NULL)
    return -1;
  char *directories =
      (char *)malloc(strlen(ogPath) + 1); // malloc to not modify actual path
  strcpy(directories, ogPath);
  char *directoryList[100];
  char dircopy[500];
  int numDir = 0;
  struct stat st;

  char *directoriesCopy = strtok(directories, ":");
  while (directoriesCopy != NULL) {
    // changes path return to a list of directories
    directoryList[numDir] = directoriesCopy;
    numDir++;
    directoriesCopy = strtok(NULL, ":");
  }

  for (int i = 0; i < numDir; i++) {
    strcpy(dircopy, directoryList[i]);
    strcat(dircopy, "/");
    strcat(dircopy, *command);
    // if file has execute permission, and not a directory
    if (access(dircopy, X_OK) == 0 && stat(dircopy, &st) == 0 &&
        S_ISREG(st.st_mode)) {
      if (changed[0] > 0)
        free(*command);

      *command = (char *)malloc(sizeof(char) * (strlen(dircopy) + 1));
      strcpy(*command, dircopy);
      changed[0] = 1;
      break;
    }
  }
  if (changed[0] == 0)
    // MERGE: added the missing "\n" and send it to stderr.
    fprintf(stderr, "%s: Command not found.\n", *command);
  free(directories);
  return changed[0];
}

int TildeExpansion(char *args[], int num, int changed[]) {
  int change = 0;
  for (int i = 0; i < num; i++) {
    if (changed[i])
      continue;
    if (strncmp("~/", args[i], 2) == 0) // need to add whatever comes after /
    {
      char *home = getenv("HOME");
      char *arg = &args[i][1];
      if (home != NULL) {
        args[i] =
            (char *)malloc(sizeof(char) * (strlen(home) + strlen(arg)) + 1);
        if (args[i] == NULL)
          return -1; // need to add smth in main to check (?)
        strcpy(args[i], home);
        strcat(args[i], arg);
        changed[i] = 1;
        change++;
      }
    } else if (strcmp("~", args[i]) == 0) // should be a ~ by itself no need to
    {                                     // malloc just pt straight to env var
      char *home = getenv("HOME");
      if (home != NULL)
        args[i] = home;
    }
  }
  return change;
}
