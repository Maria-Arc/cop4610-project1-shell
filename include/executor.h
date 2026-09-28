#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "pipeline.h"

// Runs cmds[0..n) as a pipeline (n == 1 is just a single command).
void run_pipeline(Cmd cmds[MAX_CMDS], int n, int background, const char *cmdline);

#endif
