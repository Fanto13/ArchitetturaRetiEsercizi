
//
// Created by giacomo on 02/12/18.

/*
 * protoc-c --c_out=. nomefile.proto
 * gcc -c message.pb-c.c
 * gcc -o bolettino_neve Client.c message.pb-c.c -L/usr/lib -lprotobuf-c
 *
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

#define DIM 4096


//************************PER SPEDIRE**************************************************


//************************PER RICEVERE*****************************************************



int main(int argc, char **argv) {
    /*********************************SEMPRE********************************************/
    int err;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *ptr;
    char *host_remoto;
    char *servizio_remoto;
    int sd;
    /*********************************FINE SEMPRE********************************************/
    /*********************************DELL'ESERCIZIO********************************************/
    /*
     * VARIBILI UTILI
     */
    char buff[DIM];
    char end_or_month[DIM];
    int year;
    char c;
    int nread;
/*********************************FINE DELL'ESERCIZIO********************************************/
    /*********************************GENERAZIONE CLIENT********************************************/
    /* Controllo argomenti */
    if (argc < 2) {
        printf("Uso: client <server> <porta> ...\n");
        exit(1);
    }
    /****************************************************************************************/

    /* Costruzione dell'indirizzo */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    /* Risoluzione dell'host */
    host_remoto = argv[1];

    if ((err = getaddrinfo(host_remoto, argv[2], &hints, &res)) != 0) {
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
            printf("connect riuscita\n");
            break;
        }

        close(sd);
    }
    /* Verifica sul risultato restituito da getaddrinfo */
    if (ptr == NULL) {
        fprintf(stderr, "Errore risoluzione nome: nessun indirizzo corrispondente trovato\n");
        exit(3);
    }

    /* Liberiamo la memoria allocata da getaddrinfo() */
    freeaddrinfo(res);
/*********************************FINE GENERAZIONE CLIENT********************************************/
/*
 *
 * CODE CHE IL CLIENT DEVE FARE
 *
 * EXIT DA 4 IN POI
 *
 */

/*
 * scanf mese (Stringa) scanf anno (Intero)
 * write senza terminatore x 2
 * read  senza terminatore
 */

    printf("Insert a month:> ");
    scanf("%c", end_or_month);

    while ((c = getchar()) != '\n' && c != EOF);//Pulisco standard input

    while (strcmp(end_or_month, "fine") != 0) {

        printf("Insert a year:> ");
        scanf("%d", &year);

        if (write(sd, end_or_month, strlen(end_or_month)) < 0) {//Invio stringa SENZA TERMINATORE
            perror("INVIO MESE");
            exit(4);
        }

        if (write(sd, &year, sizeof(year)) < 0) {//Invio stringa SENZA TERMINATORE
            perror("INVIO YEAR");
            exit(5);
        }

        //RICEZIONE STREAM
        memset(buff, 0, sizeof(buff));
        while ((nread = read(sd, buff, DIM)) > 0) {//LEGGO DALLO STREAM
            if (write(1, buff, nread) < 0) {//SCRIVO SU STDOUTPUT
                perror("ERRORE SCRITTURA SU STODUT");
                exit(9);
            }
        }

        printf("Insert a month:> ");
        scanf("%c", end_or_month);

        while ((c = getchar()) != '\n' && c != EOF);//Pulisco standard input



    }

    if (write(sd, end_or_month, strlen(end_or_month)) < 0) {//Invio stringa SENZA TERMINATORE
        perror("INVIO FINE");
        exit(10);
    }


    close(sd);
    return 0;
}

/*******************************************************FINE MODIFICA PER FARE COSE*********************************************************************************************/
