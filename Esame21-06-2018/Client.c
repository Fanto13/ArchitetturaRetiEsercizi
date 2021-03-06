
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
//************INCLUDE PROTOBUF******************
#include "message.pb-c.h"
//**********************************************

#define DIM 4096
#define numero_argomenti 2


/*
 *
 *            while ((nread = read(sd, buff, DIM)) > 0) {//LEGGO DALLO STREAM
            if (write(1, buff, nread) < 0) {//SCRIVO SU STDOUTPUT
                perror("ERRORE SCRITTURA SU STODUT");
                exit(9);
            }
        }

        */


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
    char c;
    int nread;
    char buff[DIM];
    /*********************************FINE VARIBILI CREAZIONE CONNESSIONE********************************************/

    /*
     * VARIBILI UTILI
     */

    // void *buffer;
    //unsigned length;
    RispostaServer *risposta;
    RichiestaClient richiesta = RICHIESTA_CLIENT__INIT;


    char tempnomefile[DIM];
    char Stato[DIM];

    int stop = 0;

    /*
     * FINE VARIABILI UTILI
     */

    /*********************************GENERAZIONE CLIENT********************************************/
    /* Controllo argomenti */
    if (argc < numero_argomenti) {
        printf("Uso: rps <server> <porta> <soglia>...\n");
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
        printf("Inserire il nome del file da visualizzare:> ");
        scanf("%s", tempnomefile);
        while ((c = getchar()) != '\n' && c != EOF);

        richiesta.nomefile = tempnomefile;// IMPORTANTE PER COPIA

        proto_send_nodim_client(sd, &richiesta);
//************************************************************************************
        nread = read(sd, buff, sizeof(buff));//RICEVO
        if (nread < 0) {
            perror("PROTOBUF");
            exit(5);
        }
        fprintf(stderr, "ENTRO NELLA FUNZIONE\n");

        risposta = risposta_server__unpack(NULL, nread, buff);//DESERIALIZZO/ESTRAGGO
        fprintf(stderr, "ENTRO NELLA FUNZIONE\n");
        if (risposta == NULL) {
            perror("ERRORE DESERIALIZZAZIONE");
            exit(6);
        }
        //****************************************************************************
        fprintf(stderr, "FUORIFUNZIONE");
        printf("\n%d\n\n\n", risposta->dim);

        if (risposta->dim > 0)
            stop = 1;


    } while (stop != 1);

    fprintf(stderr, "\n\nESCO DAL LOOP\n\n");
    if (risposta->dim > atoi(argv[3])) {
        printf("SUPERA LA SOGLIA");

        sprintf(Stato, "ERROR");
        send_stringa_ben_formata(sd, Stato);


        close(sd);
        exit(11);

    } else {
        printf("DIM ACCETTABLE");
        sprintf(Stato, "OK");
        send_stringa_ben_formata(sd, Stato);


        read_from_stream(sd);


    }

    close(sd);
    return 0;
}

/*******************************************************FINE MODIFICA PER FARE COSE*********************************************************************************************/
