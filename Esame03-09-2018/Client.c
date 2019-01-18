
//
// Created by giacomo on 02/12/18.
//


/*
 * protoc-c --c_out=. message.proto
 * gcc -c message.pb-c.c
 * gcc -o client Client.c message.pb-c.c -L/usr/lib -lprotobuf-c
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "message.pb-c.h"

#define DIM 4096
#define numero_argomenti 3

int main(int argc, char **argv) {
    /*********************************VARIABILI CREAZIONE CONNESSIONE********************************************/
    int err;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *ptr;
    char *host_remoto;
    char *servizio_remoto;
    int sd;
    int connessione_numero;
    int nread;
    /*********************************FINE VARIBILI CREAZIONE CONNESSIONE********************************************/

    /*
     * VARIBILI UTILI
     */

    Com4 spedito = COM4__INIT;

    //MI SERVONO VARIABILI DI APPOGGIO DA METTERE NELLA STRUTTURA PROTO

    char nomefile[DIM];
    char testoaggiuntivo[DIM];
    char directory[DIM];
    int option;


    int stop = 0;

    //varibile pulitura input
    char c;
    char Stato[DIM];
    char buff[DIM];
    /*
     * FINE VARIABILI UTILI
     */

    /*********************************GENERAZIONE CLIENT********************************************/
    /* Controllo argomenti */
    if (argc < numero_argomenti) {
        printf("Uso: client <server> <porta> <soglia>...\n");
        exit(1);
    }
    /* Fine controllo numero_argomenti*/
    //################################################CREO CONNESSIONE##########################################################################
    /* Costruzione dell'indirizzo */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    /* Risoluzione dell'host */
    host_remoto = argv[1];
    servizio_remoto = argv[2];
    if ((err = getaddrinfo(host_remoto, servizio_remoto, &hints, &res)) != 0) {
        fprintf(stderr, "Errore risoluzione nome: %s\n", gai_strerror(err));
        exit(2);
    }
    for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
        /*se socket fallisce salto direttamente alla prossima iterazione */
        if ((sd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) < 0) {
            fprintf(stderr, "creazione socket fallita\n");
            continue;
        }
        /*se connect funziona esco dal ciclo */
        if (connect(sd, ptr->ai_addr, ptr->ai_addrlen) == 0) {
            printf("connect riuscita al tentativo %d\n", connessione_numero);
            break;
        }
        connessione_numero++;
        close(sd);
    }
    /* Verifica sul risultato restituito da getaddrinfo */
    if (ptr == NULL) {
        fprintf(stderr, "Errore risoluzione nome: nessun indirizzo corrispondente trovato\n");
        exit(3);
    }

    /* Liberiamo la memoria allocata da getaddrinfo() */
    freeaddrinfo(res);
    //##########################################FINE CREAZIONE CONNESSIONE#####################################################################

/*********************************FINE GENERAZIONE CLIENT********************************************/
/*
 *
 * CODE CHE IL CLIENT DEVE FARE
 *
 */

    do {
        printf("INSERIRE COMANDO:> ");
        scanf("%d", &option);


        switch (option) {
            case 1:
                printf("INSERIRE NOME_FILE:> ");
                scanf("%s", nomefile);
                while ((c = getchar()) != '\n' && c != EOF);//Pulisco standard input

                printf("INSERIRE TESTO DA AGGIUNGERE:> ");
                scanf("%s", testoaggiuntivo);
                while ((c = getchar()) != '\n' && c != EOF);//Pulisco standard input
                spedito.testoaggiuntivo = testoaggiuntivo;
                break;

            case 2:
                printf("INSERIRE NOME_FILE DA ELIMINARE:> ");
                scanf("%s", nomefile);
                while ((c = getchar()) != '\n' && c != EOF);//Pulisco standard input
                break;
            case 3:
                printf("INSERIRE NOME_DIRECTORY:> ");
                scanf("%s", directory);
                while ((c = getchar()) != '\n' && c != EOF);//Pulisco standard input
                break;

            case -1:


                printf("INSERIRE NOME_FILE:> ");
                scanf("%s", nomefile);
                while ((c = getchar()) != '\n' && c != EOF);//Pulisco standard input
                stop = 1;
                break;
        }
        spedito.nomefile = nomefile;
        spedito.command = option;
        spedito.directory = directory;


        void *buffer;
        unsigned length;
        length = com4__get_packed_size(&spedito);
        buffer = malloc(length);
        com4__pack(&spedito, buffer);


        fprintf(stderr, "ESEGUO\n");

        if ((write(sd, buffer, length)) < 0) {
            perror("WRITE ERROR");
            exit(6);
        }

        free(buffer);
        fflush(stdout);


        switch (option) {
            case 1:
                nread = read(sd, Stato, sizeof(Stato));//RICEVO
                if (nread < 0) {
                    perror("PROTOBUF");
                    exit(5);
                }
                break;
            case 2:
                nread = read(sd, Stato, sizeof(Stato));//RICEVO
                if (nread < 0) {
                    perror("PROTOBUF");
                    exit(5);
                }
                break;
            case 3:
                nread = read(sd, Stato, sizeof(Stato));//RICEVO
                if (nread < 0) {
                    perror("PROTOBUF");
                    exit(5);
                }

                break;
            case -1:
                while ((nread = read(sd, buff, DIM)) > 0) {//LEGGO DALLO STREAM
                    if (write(1, buff, nread) < 0) {//SCRIVO SU STDOUTPUT
                        perror("ERRORE SCRITTURA SU STODUT");
                        exit(9);
                    }
                }
                close(sd);
                exit(0);
                break;

        }
        printf("Il sever dice: %s\n", Stato);


    } while (!stop);


    close(sd);
    return 0;
}

/*******************************************************FINE MODIFICA PER FARE COSE*********************************************************************************************/
