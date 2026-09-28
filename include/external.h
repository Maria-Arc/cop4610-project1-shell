#ifndef EXTERNAL_H
#define EXTERNAL_H

#include <sys/types.h>

pid_t SpawnCommand(char *args[]);
int ExecuteCommand(char *args[]);

#endif
