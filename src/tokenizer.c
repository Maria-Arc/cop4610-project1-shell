#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// replaces spaces with \0 then points args[i] to start of word
// args[0] is a ptr that points to 0th word in line and so on
int Tokenize(char* line, char* args[])
{
    int n = 0;
    char* token = strtok(line, " \n");
    while (token != NULL){
        args[n] = token;
        n++;
        token = strtok(NULL, " \n");
    }
    args[n] = NULL;
    return n;
}

int TildeExpansion(char* args[], int num, int changed[])
{
    int change = 0;
    for (int i = 0; i < num; i++){
        if (strncmp("~/", args[i], 2) == 0)   //add whatever after /
        {
            char* home = getenv("HOME");
            char * arg = &args[i][1];
            if (home != NULL){
                args[i] = (char*)malloc(sizeof(char) * (strlen(home) + strlen(arg)) + 1);
                if (args[i] == NULL)
                    return -1;
                strcpy(args[i], home);
                strcat(args[i], arg);
                changed[i] = 1;
                change++;
            }
        }
        else if (strncmp("~", args[i], 2) == 0 )  // should be a ~ by itself
        {
            char* home = getenv("HOME");
            if (home != NULL)
                args[i] = home;
        }
    }
    return change;
}


int main() {
	char line[200];
    char* args[150];
    const char* USER = getenv("USER");
    const char* MACHINE = getenv("MACHINE");
    char* pwd = getenv("PWD");
    int needToFree[150];
    int numTokens;

    //USER@MACHINE:PWD>
    printf("%s@%s:%s> ", USER, MACHINE, pwd );
    while (fgets(line, sizeof(line), stdin) != NULL) {

        numTokens = Tokenize(line, args);
        for (int i = 0; i < numTokens; i++){
            printf("%d: \t %s\n", i, args[i]);
            needToFree[i] = 0;
        }

        if (TildeExpansion(args, numTokens, needToFree) > 0){
             for (int j = 0; j < numTokens; j++){
                printf("CHANGED TO: %s\n", args[j]);
                if (needToFree[j] == 1) 
                {
                    free(args[j]);
                    args[j] = NULL;
                }
                 
             }
        }

        if (args[0] != NULL && strcmp(args[0], "exit") == 0)
            return 0; 

         printf("%s@%s:%s> ", USER, MACHINE, pwd );
    }
  
    return 0;
}


