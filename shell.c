#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    char comando[1000];
    
    while (1) {
        printf("Comando: ");
        if (fgets(comando, sizeof(comando), stdin)) {
            printf("%s", comando);
        }
        comando[strcspn(comando, "\n")] = 0; // Tira a newLine pra colocar somente o comando no execlp()
        if (strcmp(comando, "exit") == 0){
            exit(0);
        }
        pid_t pid = fork();
        if (pid == 0) {
            execlp(comando, comando, NULL);
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