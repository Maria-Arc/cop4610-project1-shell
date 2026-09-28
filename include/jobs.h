#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>
#include "shell.h"

// adds a new background job, prints "[job] pid"
int add_job(pid_t pids[], int n, const char *cmdline);

// checks for finished jobs without blocking, prints "done" lines
void poll_jobs(void);

// prints all jobs still running
void print_jobs(void);

// waits for every job to finish (used by exit)
void wait_all_jobs(void);

#endif
