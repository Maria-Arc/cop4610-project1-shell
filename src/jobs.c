#include "jobs.h"
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

typedef struct {
  int job_num;
  pid_t report_pid;      // pid we print to the user (last command in pipe)
  pid_t pids[MAX_CMDS];  // all pids in this job
  int reaped[MAX_CMDS];  // has each pid been waited on yet
  int num_pids;
  char cmdline[CMDLINE_LEN];
  int active;
} Job;

static Job job_table[MAX_JOBS];
static int next_job_num = 1; // job numbers just keep going up, never reused

int add_job(pid_t pids[], int n, const char *cmdline) {
  int slot = -1;
  for (int i = 0; i < MAX_JOBS; i++) {
    if (!job_table[i].active) {
      slot = i;
      break;
    }
  }
  if (slot == -1) {
    fprintf(stderr, "shell: too many background jobs\n");
    return -1;
  }

  Job *j = &job_table[slot];
  j->job_num = next_job_num++;
  j->num_pids = n;
  for (int i = 0; i < n; i++) {
    j->pids[i] = pids[i];
    j->reaped[i] = 0;
  }
  j->report_pid = pids[n - 1];
  strncpy(j->cmdline, cmdline, CMDLINE_LEN - 1);
  j->cmdline[CMDLINE_LEN - 1] = '\0';
  j->active = 1;

  printf("[%d] %d\n", j->job_num, j->report_pid);
  fflush(stdout);
  return j->job_num;
}

void poll_jobs(void) {
  for (int i = 0; i < MAX_JOBS; i++) {
    if (!job_table[i].active)
      continue;
    Job *j = &job_table[i];

    int all_done = 1;
    for (int k = 0; k < j->num_pids; k++) {
      if (j->reaped[k])
        continue;
      int status;
      if (waitpid(j->pids[k], &status, WNOHANG) > 0)
        j->reaped[k] = 1;
      else
        all_done = 0;
    }

    if (all_done) {
      printf("[%d]  + %d done %s\n", j->job_num, j->report_pid, j->cmdline);
      fflush(stdout);
      j->active = 0;
    }
  }
}

void print_jobs(void) {
  int any = 0;
  for (int i = 0; i < MAX_JOBS; i++) {
    if (!job_table[i].active)
      continue;
    printf("[%d]  + %d running %s\n", job_table[i].job_num,
           job_table[i].report_pid, job_table[i].cmdline);
    any = 1;
  }
  if (!any)
    printf("No active jobs.\n");
}

void wait_all_jobs(void) {
  for (int i = 0; i < MAX_JOBS; i++) {
    if (!job_table[i].active)
      continue;
    Job *j = &job_table[i];

    for (int k = 0; k < j->num_pids; k++) {
      if (!j->reaped[k]) {
        int status;
        waitpid(j->pids[k], &status, 0); // block here, exit has to wait
        j->reaped[k] = 1;
      }
    }

    printf("[%d]  + %d done %s\n", j->job_num, j->report_pid, j->cmdline);
    fflush(stdout);
    j->active = 0;
  }
}