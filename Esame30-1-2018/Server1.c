//
// Created by giacomo on 02/12/18.
//

//*****************************************************************
#include <sys/stat.h>
//*****************************************************************
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

#define N 80
#define DIM 2048


/* Gestore del segnale SIGCHLD */
void handler(int s) {
    int status;
    wait(&status);
}


int main(int argc, char **argv) {
    /******************************************SEMPRE***************************************************************/
    struct addrinfo hints, *res;//Servono sempre
    int err, sd, ns, pid, pid2;//Servono sempre
    int on;
    //int dowhile = 1;
    /********************************************FINE SEMPRE*************************************************************/
    /**********************************************DELL'ESERCIZIO***********************************************************/
    /*
     *
     * DICHIARAZIONE VARIABILI UTILI
     *
     *
     */
    char buff[DIM];
    int nread;
//*************************************************************
    char end_or_month[DIM];
    int year;
    char yearstring[DIM];
    int piped[2];
    char path[DIM];
//*************************************************************


    /**********************************************FINE DELL'ESERCIZIO**********************************************************/
    /***********************************************GENERAZIONE SERVER*************************************************************/
    /* Controllo argomenti */
    if (argc < 2) {
        printf("Uso: ./server <porta> \n");
        exit(1);
    }

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

    on = 1;
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

    /*******************************************FINE GENERAZIONE SERVER**************************************************************/


    /* Attendo i client... */
    for (;;) {
        printf("Server in ascolto...\n");

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
 * EXIT DA 9 IN POI
 *
 *
 */         int w;

            if (pipe(piped) < 0) {
                perror("ERRORE PIPE");
                exit(10);
            }
            memset(&end_or_month, 0, sizeof(end_or_month));
            if ((w = read(ns, end_or_month, DIM - 1)) < 0) {
                perror("ERRORE RICEZIONE");
                exit(9);
            }


            if (strcmp(end_or_month, "fine") != 0) {
                if ((w = read(ns, &year, sizeof(year))) < 0) {
                    perror("ERRORE RICEZIONE");
                    exit(10);
                }

                sprintf(yearstring, "%d", year);



                /* Generazione di un figlio */
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

                    sprintf(path, "./file/vendite/%s/%s.txt", yearstring, end_or_month);

                    execlp("sort", "sort", "-n", "-r", path, (char *) 0);
                    perror("ERRORE SORT");
                    exit(9);

                } else {

                    close(piped[1]);


                    //chiudo input perchè ricevo da pipe
                    close(0);
                    dup(piped[0]);//ricevo dalla pipe e scrivo in stdinput
                    close(piped[0]);

                    close(1);
                    dup(ns);//Scrivo sullo stream
                    close(2);//TOTALEMNTE OPZIONALE IN QUESTO SPECIFICO CASO
                    dup(ns);
                    close(ns);
                    exit(0);


                }


            } else {
                close(ns);
                exit(0);//IMPORTANTE!!!!!!!
            }









//            float counter = 0;
//            int reply = 0;
//            int w = 0, stop = 1;
//
//
//            memset(&ricevuto, 0, sizeof(ricevuto));
//            if ((w = read(ns, &ricevuto, sizeof(ricevuto))) < 0) {
//                perror("ERRORE RICEZIONE");
//                exit(9);
//            }
//            // printf("%f ---  %c", ricevuto.operando, ricevuto.operatore);
//
//            while (ricevuto.operatore != 'z') {
//
//                switch (ricevuto.operatore) {
//                    case '+':
//                        counter += ricevuto.operando;
//                        msg.stato = "OK";
//                        break;
//                    case '-':
//                        counter -= ricevuto.operando;
//                        msg.stato = "OK";
//
//                        break;
//                    case '*':
//                        counter *= ricevuto.operando;
//                        msg.stato = "OK";
//                        break;
//                    case '/':
//                        if (ricevuto.operando == 0) {
//                            msg.stato = "NotANumber";
//                        } else {
//                            counter /= ricevuto.operando;
//                            msg.stato = "OK";
//                        }
//                        break;
//                    default:
//                        msg.stato = "UNKNOWN";
//                        break;
//                }
//
//                msg.risultato_parziale = counter;
//
//                //strcpy(msg.stato, "OK");
//
//                length = response__get_packed_size(&msg);
//                buffer = malloc(length);
//                response__pack(&msg, buffer);
//
//                fprintf(stderr, "ESEGUO");
//
//                if ((write(ns, buffer, length)) < 0) {
//                    perror("WRITE ERROR");
//                    exit(6);
//                }
//                // printf("\n\n%d\n\n",w);
//
//                free(buffer);
//                //fflush(stdout);
//
//                memset(&ricevuto, 0, sizeof(ricevuto));
//                if ((w = read(ns, &ricevuto, sizeof(ricevuto))) < 0) {
//                    perror("ERRORE RICEZIONE");
//                    exit(9);
//                }
//
//            }



/*******************************************************FINE MODIFICA PER FARE COSE*********************************************************************************************/



        } else {
            /* padre */
            close(ns);
        }
    }

    return 0;
}
