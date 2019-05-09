#define DIM 4096

void handler(int s);

void proto_send_nodim_server(int sd, RispostaServer *risposta);

void read_stringa_ben_formata(int sd, char *stato);

void write_on_socket(int sd);

void proto_send_nodim_client(int sd, RichiestaClient *richiesta);

void send_stringa_ben_formata(int sd, char *string);

void read_from_stream(int sd);

Rispostaserver *proto_receive_nodim(int sd, RispostaServer *risposta);
