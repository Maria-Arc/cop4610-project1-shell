#include "external.h"
#include "redirection.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>




int ExecuteCommand(char *args[])
{
    char *commandArgs[200];
    int commandIndex=0;
    //arg list without symbl/file name
    for(int i=0; args[i]!=NULL; i++)
    {
        if (strcmp(args[i],"<")==0 || strcmp(args[i],">")==0)
            i++;
        else
        {
            commandArgs[commandIndex]=args[i];
            commandIndex++;
        }
    }
    commandArgs[commandIndex]=NULL;
    pid_t pid= fork();

    if (pid<0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        if(RedirectIO(args)==-1)
        {
            exit(1);
        }
        //replace child process w command
        execv(commandArgs[0], commandArgs);
        perror("execv");
        exit(1);
    }
    //waiting for child process to finish before we continue
    int status;

    if(waitpid(pid,&status,0)==-1)
    {
        perror("waitpid");
        return -1;
    }
    return 0;
}