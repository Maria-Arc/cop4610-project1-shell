#include "redirection.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int RedirectIO(char *args[]) {
  int inputIndex = -1;
  int outputIndex = -1;

  // finding the symbols
  for (int i = 0; args[i] != NULL; i++) {
    if (strcmp(args[i], "<") == 0)
      inputIndex = i;
    if (strcmp(args[i], ">") == 0)
      outputIndex = i;
  }

  if ((inputIndex != -1 && args[inputIndex + 1] == NULL) ||
      (outputIndex != -1 && args[outputIndex + 1] == NULL)) {
    fprintf(stderr, "shell: syntax error: missing file name for redirection\n");
    return -1;
  }

  // input is now handled BEFORE output
  if (inputIndex != -1) {
    // added the "must be a regular file" check the spec asks for
    struct stat st;
    if (stat(args[inputIndex + 1], &st) != 0) {
      fprintf(stderr, "shell: %s: No such file or directory\n",
              args[inputIndex + 1]);
      return -1;
    }
    if (!S_ISREG(st.st_mode)) {
      fprintf(stderr, "shell: %s: Not a regular file\n", args[inputIndex + 1]);
      return -1;
    }
    // O_RDONLY: never modify the input file
    int fd = open(args[inputIndex + 1], O_RDONLY);
    if (fd == -1) {
      perror("open");
      return -1;
    }
    if (dup2(fd, STDIN_FILENO) == -1) {
      perror("dup2");
      close(fd);
      return -1;
    }
    close(fd);
  }

  if (outputIndex != -1) {
    int fd = open(args[outputIndex + 1], O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRUSR | S_IWUSR);
    if (fd == -1) {
      perror("open");
      return -1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1) {
      perror("dup2");
      close(fd);
      return -1;
    }
    close(fd);
  }
  return 0;
}
