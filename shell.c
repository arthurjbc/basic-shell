#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    char comando[1000];
    char *arg[1000];

    // initialize the process with clear command
    pid_t pid2 = fork();
    if (pid2 == 0){
        execlp("clear", "clear", NULL);
        exit(1);
        perror("deu ruim");
    }
    if (pid2 > 0){
        int st;
        wait (&st);
    }

    while (1) {

        printf("\033[34m%s\033[0m$ ", getcwd(NULL, 0));
        int i=0;

        // tranforms the input into a array (necessary for execvp)
        if (fgets(comando, sizeof(comando), stdin)!=NULL){
            char *token = strtok(comando, " ");
            while (token != NULL){
                arg[i] = token;
                i++;
                token = strtok(NULL, " ");
            }
            arg[i] = NULL;
        }

        arg[i-1][strcspn(arg[i-1], "\n")] = 0; // take the newLine off
        
        if (strcmp(comando, "exit") == 0){   //built-in command exit
            exit(0);
        }

        if (strcmp(arg[0], "cd")==0){   //built-in command cd
            chdir(arg[1]);
            continue;
        }

        // create a new process responsible for the command inserted
        pid_t pid = fork();
        if (pid == 0) {
            execvp(arg[0], arg);
            perror("exec failed");
            exit(1);
        }
        else if (pid > 0) {
            int status;
            wait(&status);
        }
    }
    
    return 0;
}