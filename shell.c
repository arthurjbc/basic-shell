/*
next:
    pipe: pipe()
    I/O: open()
    signal working on child: signal()
    background process: father doesnt wait for child when & appears

*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void handleChdSignal(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void clear(){
    // initialize the process with clear command
    pid_t pid2 = fork();
    if (pid2 == 0){
        execlp("clear", "clear", NULL);
        exit(1);
        perror("clear failed");
    }
    if (pid2 > 0){
        int st;
        wait (&st);
    }
}

int main() {
    char comando[1000];
    
    clear();
    
    while (1) {
        char *arg[1000] = {};
        signal(SIGCHLD, handleChdSignal);
        int bg=0;
        char *cwd = getcwd(NULL, 0);
        printf("\033[33m%s@\033[0m\033[34m%s\033[0m$ ", getlogin(), cwd);
        free (cwd);
        int i=0;

        // tranforms the input into a array (necessary for execvp)
        if (fgets(comando, sizeof(comando), stdin)!=NULL){
            char *token = strtok(comando, " \n"); 
            while (token != NULL){
                arg[i] = token;
                i++;
                token = strtok(NULL, " \n"); 
            }
            arg[i] = NULL;
        }
        
        if (i>1 && strcmp(arg[i-1], "&")==0){  // handle background process request
            arg[i-1] = NULL; 
            bg = 1;
        }

        if (arg[0]==NULL){
            continue;
        }

        
        if (strcmp(arg[0], "exit") == 0){   // built-in command exit
            printf("exit\n");
            exit(0);
        }

        if (strcmp(arg[0], "cd")==0){   // built-in command cd
            if (arg[1]==NULL){
                chdir("/home");
            }else{
                if (chdir(arg[1])==-1){
                    perror("cd failed");
            }
            }

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
            if (bg){
                printf("[%d]Job running on background\n", pid);
            }else{
                int status;
                wait(&status);
            }
        }
    }
    
    return 0;
}