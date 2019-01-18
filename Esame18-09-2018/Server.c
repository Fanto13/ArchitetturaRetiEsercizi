//
// Created by giacomo on 02/12/18.
//

/*
 * protoc-c --c_out=. message.proto
 * gcc -c message.pb-c.c
 * gcc -o server Server.c message.pb-c.c -L/usr/lib -lprotobuf-c
 */



#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "struct.h"


#define DIM 4096
#define maxdim 31
#define numero_argomenti 2

/* Gestore del segnale SIGCHLD */
void handler(int s) {
    int status;
    wait(&status);
}


int main(int argc, char **argv) {
    /******************************************VARIBILI CREAZIONE CONNESSIONE***************************************************************/
    struct addrinfo hints, *res;//Servono sempre
    int err, sd, ns, pid;
    int on = 1;//Servono sempre
    int nread;
    char buff[DIM];

    /********************************************FINE VARIABILI CREAZIONE CONNESSIONE*************************************************************/
    /*
     *
     * DICHIARAZIONE VARIABILI UTILI
     */
    Datidiricerca package;
    char path[DIM];
    char State[maxdim];
    int pid2;
    int piped[2];
    char ntostring[maxdim];
    /*
    * FINE DICHIARAZIONE VARIABILI UTILI
    */
    /***********************************************GENERAZIONE SERVER*************************************************************/
    /* Controllo argomenti */
    if (argc < numero_argomenti) {
        printf("Uso: ./server <porta> \n");
        exit(1);
    }
    /*Fine controllo argomenti*/
    //Segnale per non dover aspettare i figli
    signal(SIGCHLD, handler);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((err = getaddrinfo(NULL, argv[1], &hints, &res)) != 0) {
        fprintf(stderr, "Errore setup indirizzo bind: %s\n", gai_strerror(err));
        exit(2);
    }

    if ((sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
        perror("Errore in socket");
        exit(3);
    }
//Dichiaro on=1 ad inizio codice
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        perror("setsockopt");
        exit(4);
    }

    if (bind(sd, res->ai_addr, res->ai_addrlen) < 0) {
        perror("Errore in bind");
        exit(5);
    }

    freeaddrinfo(res);

    if (listen(sd, SOMAXCONN) < 0) {
        perror("listen");
        exit(6);
    }


/*
*sd filedescriptor di ascolto per le connessioni in entrata
*ns filedescriptor specifico per una singola connessione, uso sempre ns perchè viene passato al figlio figlio tramite fork
*Il figlio chiude subito sd e alla fine chiude ns
*Il padre chiude sempre ns come prima cosa e non chiude mai sd
*/
    /*******************************************FINE GENERAZIONE SERVER**************************************************************/
//######################################CICLO INFINITO DI ASCOLTO CLIENT##############################################################

    /* Attendo i client... */
    for (;;) {
        printf("Server in ascolto...\n");
        //Accetto le connessioni
        if ((ns = accept(sd, NULL, NULL)) < 0) {
            /* Ignoro gli errori di tipo interruzione da segnale. */
            if (errno == EINTR)
                continue;
            /* Gestisco tutte le altre tipologie di errore. */
            perror("accept");
            exit(7);
        }

        /* Generazione di un figlio */
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(8);
        } else if (pid == 0) {
            /* figlio */
            close(sd);
/*******************************************************MODIFICARE PER FARE COSE*********************************************************************************************/
/*
 *
 * FIGLIO E CODICE DA ESEGUIRE
 *
 *
 */

            if (pipe(piped) < 0) {
                perror("ERRORE PIPE");
                exit(10);
            }

            nread = read(ns, &package, sizeof(package));//RICEVO
            if (nread < 0) {
                perror("READ STATO");
                exit(5);
            }

            printf("\nAnno: %d\tN: %d\tRegione: %s\n", package.anno, package.N, package.regione);

            sprintf(path, "./%d", package.anno);

            if (chdir(path) != 0) {

                sprintf(State, "%s\n", "ERROR");

                if ((write(ns, State, sizeof(State))) < 0) {
                    perror("WRITE ERROR");
                    exit(6);
                }

                close(ns);
                exit(0);

            }

            sprintf(State, "%s\n", "OK");

            if ((write(ns, State, sizeof(State))) < 0) {
                perror("WRITE ERROR");
                exit(6);
            }

            nread = read(ns, State, sizeof(State));//RICEVO
            if (nread < 0) {
                perror("READ STATO");
                exit(5);
            }

            if (strcmp(State, "AVVIA\n") != 0) {
                close(ns);
                perror("Bad State Descriptor...");
                exit(56);

            }
            if ((pid2 = fork()) < 0) {
                perror("fork");
                exit(8);
            } else if (pid2 == 0) {
                close(piped[0]);
                close(ns);
                close(1);
                close(2);
                dup(piped[1]);
                dup(piped[1]);
                close(piped[1]);
                sprintf(path, "./%s.txt", package.regione);

                execlp("sort", "sort", "-n", path, (char *) NULL);
                perror("SORT FAILED");
                exit(3);

            }

            close(piped[1]);
            close(0);
            dup(piped[0]);
            close(piped[0]);

            close(1);
            close(2);
            dup(ns);
            dup(ns);
            close(ns);

            sprintf(ntostring, "%d", package.N);

            execlp("head", "head", "-n", ntostring, (char *) NULL);
            perror("HEAD ERROR");
            exit(4);

/*******************************************************FINE MODIFICA PER FARE COSE*********************************************************************************************/
            //close(ns);
            // exit(0);
        } else {
            /* padre */
            close(ns);
        }
    }
    return 0;
}
