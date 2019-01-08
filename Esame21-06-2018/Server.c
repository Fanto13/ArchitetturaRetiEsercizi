
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

#include <sys/stat.h>
//************INCLUDE PROTOBUF******************
#include "message.pb-c.h"
//**********************************************

#define N 4096
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
    char buff[N];
    int nread;
    /********************************************FINE VARIABILI CREAZIONE CONNESSIONE*************************************************************/
    /*
     *
     * DICHIARAZIONE VARIABILI UTILI
     */


    RichiestaClient *richiesta;

    RispostaServer risposta = RISPOSTA_SERVER__INIT;
    void *buffer;
    unsigned length;
    struct stat st;

    char filename[N];


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

            do {
                nread = read(ns, buff, sizeof(buff));//RICEVO
                if (nread < 0) {
                    perror("PROTOBUF");
                    exit(5);
                }

                richiesta = richiesta_client__unpack(NULL, nread, buff);//DESERIALIZZO/ESTRAGGO
                if (richiesta == NULL) {
                    perror("ERRORE DESERIALIZZAZIONE");
                    exit(6);
                }

                printf("%s", richiesta->nomefile);

                if (stat(richiesta->nomefile, &st) == 0)
                    risposta.dim = st.st_size;
                else
                    risposta.dim = -1;

                length = risposta_server__get_packed_size(&risposta);
                buffer = malloc(length);
                risposta_server__pack(&risposta, buffer);

                fprintf(stderr, "ESEGUO");

                if ((write(ns, buffer, length)) < 0) {
                    perror("WRITE ERROR");
                    exit(6);
                }

                free(buffer);
                fflush(stdout);
            } while (risposta.dim == -1);


            nread = read(ns, buff, sizeof(buff));//RICEVO
            if (nread < 0) {
                perror("READ STATO");
                exit(5);
            }

            if (strcmp(buff, "ERROR") == 0) {
                printf("OUCH");
                close(ns);
                exit(1);
            } else if (strcmp(buff, "OK") == 0) {
                printf("YAY");

                close(1);
                close(2);
                dup(ns);
                dup(ns);
                close(ns);

                execlp("cat", "cat", richiesta->nomefile, (char *) 0);
            }



/*******************************************************FINE MODIFICA PER FARE COSE*********************************************************************************************/
            close(ns);
            exit(0);
        } else {
            /* padre */
            close(ns);
        }
    }
    return 0;
}
