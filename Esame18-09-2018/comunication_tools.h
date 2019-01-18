//
// Created by giacomo on 09/01/19.
//

#define DIM 4096

void handler(int s);

void proto_send_nodim_server(int sd, RispostaServer *risposta);

void read_stringa_ben_formata(int sd, char *stato);

void write_on_socket(int sd);

void proto_send_nodim_client(int sd, RichiestaClient *richiesta);

void send_stringa_ben_formata(int sd, char *string);

void read_from_stream(int sd);


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

