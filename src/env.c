#include "env.h"
#include <stdlib.h>
#include <string.h>

int EnvExpansion(char *args[], int num, int changed[]) {
  int change = 0;
  for (int i = 0; i < num; i++) {
    if (args[i][0] == '$' && args[i][1] != '\0') {
      char *value = getenv(args[i] + 1);
      if (value != NULL) {
        char *copy = malloc(strlen(value) + 1);
        if (copy == NULL)
          return -1;
        strcpy(copy, value);
        args[i] = copy;
        changed[i] = 1;
        change++;
      } else {
        char *copy = malloc(1);
        if (copy == NULL)
          return -1;
        copy[0] = '\0';
        args[i] = copy;
        changed[i] = 1;
        change++;
      }
    }
  }
  return change;
}
