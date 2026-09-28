#include "redirection.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int RedirectIO(char *args[])
{
    int inputIndex=-1;
    int outputIndex=-1;
   
    //finding symbols
    for(int i=0; args[i]!= NULL; i++)
    {
        if(strcmp(args[i],"<")==0)
        {
            inputIndex=i;
        }
        if(strcmp(args[i],">")==0)
        {
            outputIndex=i;
        }
    }
    //checking that output files given
    if(outputIndex!=-1 && args[outputIndex +1]==NULL)
    {
        return -1;
    }
    //redirect output to output filez
    if (outputIndex!=-1)
    {
        int fd = open(args[outputIndex +1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
        if(fd ==-1)
        {
            perror("open");
            return -1;
        }
        if(dup2(fd, STDOUT_FILENO)==-1)
        {
            perror("dup2");
            close(fd);
            return -1;
        }
        close(fd);
    }
    //check for input file
    if(inputIndex!=-1 && args[inputIndex +1]==NULL)
    {
        return -1;
    }
    //redirect inppt
    if(inputIndex!=-1)
    {
        int fd = open(args[inputIndex +1], O_RDONLY);
        if(fd ==-1)
        {
            perror("open");
            return -1;
        }
        if(dup2(fd, STDIN_FILENO)==-1)
        {
            perror("dup2");
            close(fd);
            return -1;
        }
        close(fd);

    }
    return 0;
}