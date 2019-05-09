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

#define N 4096
#define numero_argomenti 2

// Gestore del segnale SIGCHLD
void handler(int s)
{
    int status;
    wait(&status);
}

int main(int argc, char **argv)
{

    /*
    * Sezione:  VARIABILI 
    * -------------------
    * > variabili per la creazione della connessione
    * > variabili utili per l'esecuzione del programma 
    */
    struct addrinfo hints, *res; //Servono sempre
    int err, sd, ns, pid;
    int on = 1; //Servono sempre
    int nread;
    char buff[N];

    //int var;

    /*
    * Sezione:  CONNESSIONE
    * ---------------------
    * > Dopo il controllo sugli argomenti, viene creata
    *   la connessione.
    */

    // Controllo argomenti
    if (argc < numero_argomenti)
    {
        printf("Uso: ./server <porta> \n");
        exit(1);
    }

    // Segnale per non dover aspettare i figli
    signal(SIGCHLD, handler);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Controllo errori nel getaddrinf
    if ((err = getaddrinfo(NULL, argv[1], &hints, &res)) != 0)
    {
        fprintf(stderr, "Errore setup indirizzo bind: %s\n", gai_strerror(err));
        exit(2);
    }

    // Controllo errori nel socket
    if ((sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
    {
        perror("Errore in socket");
        exit(3);
    }

    // Dichiaro on=1 ad inizio codice
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("setsockopt");
        exit(4);
    }

    // Verifico l'integrità del bind
    if (bind(sd, res->ai_addr, res->ai_addrlen) < 0)
    {
        perror("Errore in bind");
        exit(5);
    }

    freeaddrinfo(res);

    if (listen(sd, SOMAXCONN) < 0)
    {
        perror("listen");
        exit(6);
    }

    /*
    * Sezione:  CICLO INFINITO DI ASCOLTO CLIENT
    * ------------------------------------------
    * > sd: filedescriptor di ascolto per le connessioni in entrata
    * > ns: filedescriptor specifico per una singola connessione, uso
    *   sempre ns perche' viene passato al figlio figlio tramite fork
    * 
    * > Il figlio chiude subito sd e alla fine chiude ns; il padre
    *   chiude sempre ns come prima cosa e non chiude mai sd.
    */

    /* Attendo i client... */
    for (;;)
    {
        printf("Server in ascolto...\n");
        //Accetto le connessioni
        if ((ns = accept(sd, NULL, NULL)) < 0)
        {
            // Ignoro gli errori di tipo interruzione da segnale
            if (errno == EINTR)
                continue;
            // Gestisco tutte le altre tipologie di errore
            perror("accept");
            exit(7);
        }

        // Generazione di un figlio
        if ((pid = fork()) < 0)
        {
            perror("fork");
            exit(8);
        }
        else if (pid == 0)
        {
            // Figlio
            close(sd);

            /*
            * Sezione:  LOGICA 
            * ----------------
            * > consegna: lorem ipsum dolor sit amet 
            */

            close(ns);
            exit(0);
        }
        else
        {
            // Padre
            close(ns);
        }
    }
    return 0;
}
