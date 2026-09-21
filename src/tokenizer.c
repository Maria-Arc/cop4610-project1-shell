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


int main() {
	char line[200];
    char* args[150];
    const char* USER = getenv("USER");
    const char* MACHINE = getenv("MACHINE");
    char* pwd = getenv("PWD");

    //USER@MACHINE:PWD>
    printf("%s@%s:%s> ", USER, MACHINE, pwd );
    while (fgets(line, sizeof(line), stdin) != NULL) {
        int j = Tokenize(line, args);
        for (int i = 0; i < j; i++){
            printf("%d: \t %s\n", i, args[i]);
        }
        
        if (strcmp(args[0], "exit") == 0)
            return 0; 

         printf("%s@%s:%s> ", USER, MACHINE, pwd );
    }
  
}


