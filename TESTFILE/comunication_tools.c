#include "message.pb-c.h"
#include "comunication_tools.h"


void proto_send_nodim(int sd, Com1 *risposta) {


    void *buffer;
    unsigned length;
    length = com1__get_packed_size(risposta);
    buffer = malloc(length);
    com1__pack(risposta, buffer);


    fprintf(stderr, "ESEGUO\n");

    if ((write(sd, buffer, length)) < 0) {
        perror("WRITE ERROR");
        exit(6);
    }

    free(buffer);
    fflush(stdout);

}


void read_stringa_ben_formata(int sd, char *stato) {

    int nread;

    nread = read(sd, stato, sizeof(stato));//RICEVO
    if (nread < 0) {
        perror("READ STATO");
        exit(5);
    }


}

void write_on_socket(int sd) {
    close(1);
    close(2);
    dup(sd);
    dup(sd);
    close(sd);
}


Com1 *proto_recive_nodim(int sd) {
    char buff[DIM];
    int nread;
    Com1 *risposta;
    fprintf(stderr, "ENTRO NELLA FUNZIONE\n");
    nread = read(sd, buff, sizeof(buff));//RICEVO
    if (nread < 0) {
        perror("PROTOBUF");
        exit(5);
    }
    fprintf(stderr, "ENTRO NELLA FUNZIONE\n");

    risposta = com1__unpack(NULL, nread, buff);//DESERIALIZZO/ESTRAGGO
    fprintf(stderr, "ENTRO NELLA FUNZIONE\n");
    if (risposta == NULL) {
        perror("ERRORE DESERIALIZZAZIONE");
        exit(6);
    }

    return risposta;

}


void send_stringa_ben_formata(int sd, char *string) {

    if ((write(sd, string, sizeof(string))) < 0) {
        perror("WRITE ERROR");
        exit(6);
    }
}

void read_from_stream(int sd) {
    char buff[DIM];
    int nread;
    while ((nread = read(sd, buff, DIM)) > 0) {//LEGGO DALLO STREAM
        if (write(1, buff, nread) < 0) {//SCRIVO SU STDOUTPUT
            perror("ERRORE SCRITTURA SU STODUT");
            exit(9);
        }
    }
}
