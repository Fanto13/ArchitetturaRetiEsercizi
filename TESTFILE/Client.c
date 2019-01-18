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
#include "comunication_tools.h"

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
    Com1 pack = COM1__INIT;
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

    pack.command = 1;
    proto_send_nodim(sd, &pack);


    close(sd);
    return 0;
}

/*******************************************************FINE MODIFICA PER FARE COSE*********************************************************************************************/
