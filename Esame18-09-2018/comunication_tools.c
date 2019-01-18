void handler(int s) {
    int status;
    wait(&status);
}


void proto_send_nodim_server(int sd, RispostaServer *risposta) {


    void *buffer;
    unsigned length;
    length = risposta_server__get_packed_size(risposta);
    buffer = malloc(length);
    risposta_server__pack(risposta, buffer);


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

void proto_send_nodim_client(int sd, RichiestaClient *richiesta) {


    void *buffer;
    unsigned length;
    length = richiesta_client__get_packed_size(richiesta);
    buffer = malloc(length);
    richiesta_client__pack(richiesta, buffer);


    fprintf(stderr, "ESEGUO\n");

    if ((write(sd, buffer, length)) < 0) {
        perror("WRITE ERROR");
        exit(6);
    }

    free(buffer);
    fflush(stdout);

}


//void proto_recive_nodim(int sd, RispostaServer *risposta) {
//    char buff[DIM];
//    int nread;
//    fprintf(stderr, "ENTRO NELLA FUNZIONE\n");
//    nread = read(sd, buff, sizeof(buff));//RICEVO
//    if (nread < 0) {
//        perror("PROTOBUF");
//        exit(5);
//    }
//    fprintf(stderr, "ENTRO NELLA FUNZIONE\n");
//
//    risposta = risposta_server__unpack(NULL, nread, buff);//DESERIALIZZO/ESTRAGGO
//    fprintf(stderr, "ENTRO NELLA FUNZIONE\n");
//    if (risposta == NULL) {
//        perror("ERRORE DESERIALIZZAZIONE");
//        exit(6);
//    }
//
//    fprintf(stderr, "\n\nDENTRO FUNCTION VALE: %d\n\n", risposta->dim);
//    risposta->dim=risposta->dim;
//
//}


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
