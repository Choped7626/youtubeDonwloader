#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

int TrozearCadena(char* cadena , char* tr[]){

    int i = 1;
    if ((tr[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((tr[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    i > 49 ? i = -1 : i;
    return i;
}

void ejec(pid_t pid , char* links[] , char* args[] , pid_t w , int wstatus){
    if((pid = fork()) == 0){
        if(execvp(links[0] , args) == -1){
            perror("Error al ejecutar");
            exit(EXIT_FAILURE);
        }
    }else{
        do {
            w = waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
            if (w == -1) {
                perror("Waitpid:");
                exit(EXIT_FAILURE);
            }
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
    }
}

int main() {
    char dir[256];
    char docu[256];

    printf("Introduzca el directorio donde quiere que se descarge la musica\n");
    scanf("%s" , dir);

    printf("Introduzca el documento de los enlaces (esto hay q automatizalo e cambialo solo polo link da playlist)\n");
    scanf("%s" , docu);

    chdir(dir);
    FILE *fptr;
    pid_t pid , w;
    int wstatus;
    char *args[256] = {NULL};
    int i, aux = 0;

    fptr = fopen(docu , "r");
    char* links[7];
    char buffer[46];

    while (fgets(buffer, 45, fptr)){
        char lol[256] = "youtube-dl --audio-format mp3 -x --output /home/choped/Music/saramalacara/%(title)s.%(ext)s ";
        strcat(lol , buffer);
        if(TrozearCadena(lol, links) != 7){
            printf("fallo");
            break;
        }
        for (i = 0; links[i] != NULL; ++i) {
            args[i + aux] = links[i];
            args[i + 1 + aux] = NULL;
        }
        ejec(pid , links , args , w , wstatus);
    }
    fclose(fptr);
}
