void handler(int s)
{
    int status;
    wait(&status);
}

/*
* Funzione:  proto_send_nodim_server 
* ----------------------------------
* 
*/
void proto_send_nodim_server(int sd, RispostaServer *risposta)
{
    void *buffer;
    unsigned length;
    length = risposta_server__get_packed_size(risposta);
    buffer = malloc(length);
    risposta_server__pack(risposta, buffer);

    // TODO: Debug
    // fprintf(stderr, "ESEGUO\n");

    if ((write(sd, buffer, length)) < 0)
    {
        perror("WRITE ERROR");
        exit(6);
    }

    free(buffer);
    fflush(stdout);
}

/*
* Funzione:  read_stringa_ben_formata
* -----------------------------------
* > Riceve una stringa dal socket e ne verifica l'integrità.
* > Argomenti: filedescriptor di ascolto; stringa.
*/

void read_stringa_ben_formata(int sd, char *stato)
{
    int nread;

    // Ricezione
    nread = read(sd, stato, sizeof(stato));
    if (nread < 0)
    {
        perror("READ STATO");
        exit(5);
    }
}

/*
* Funzione:  write_on_socket
* --------------------------
* 
*/
void write_on_socket(int sd)
{
    close(1);
    close(2);
    dup(sd);
    dup(sd);
    close(sd);
}

/*
* Funzione:  proto_send_nodim_client
* ----------------------------------
* 
*/
void proto_send_nodim_client(int sd, RichiestaClient *richiesta)
{

    void *buffer;
    unsigned length;
    length = richiesta_client__get_packed_size(richiesta);
    buffer = malloc(length);
    richiesta_client__pack(richiesta, buffer);

    // TODO: Debug
    // fprintf(stderr, "ESEGUO\n");

    if ((write(sd, buffer, length)) < 0)
    {
        perror("Errore in proto_send_nodim_client: WRITE ERROR");
        exit(6);
    }

    free(buffer);
    fflush(stdout);
}

/*
* Funzione:  proto_receive_nodim
* -----------------------------
* > Argomenti: filedescriptor di ascolto; stringa.
*/
RispostaServer *proto_receive_nodim(int sd, RispostaServer *risposta)
{
    char buff[DIM];
    int nread;

    // TODO: Debug
    // fprintf(stderr, "ENTRO NELLA FUNZIONE\n");

    // Ricezione
    nread = read(sd, buff, sizeof(buff));
    if (nread < 0)
    {
        perror("Errore in proto_send_nodim_client: PROTOBUF");
        exit(5);
    }
    
    // TODO: Debug
    // fprintf(stderr, "ENTRO NELLA FUNZIONE\n");

    // Deserializzazione
    risposta = risposta_server__unpack(NULL, nread, buff);

    // TODO: Debug
    // fprintf(stderr, "ENTRO NELLA FUNZIONE\n");

    if (risposta == NULL)
    {
        perror("Errore proto_receive_nodim: ERRORE DESERIALIZZAZIONE");
        exit(6);
    }

    // TODO: Debug
    // fprintf(stderr, "\n\nDENTRO FUNCTION VALE: %d\n\n", risposta->dim);

    return risposta;
}

/*
* Funzione:  send_stringa_ben_formata
* -----------------------------------
* > Invia una stringa sul socket e verifica che l'invio
    abbia successo.
* > Argomenti: filedescriptor di ascolto; stringa.
*/
void send_stringa_ben_formata(int sd, char *string)
{
    if ((write(sd, string, sizeof(string))) < 0)
    {
        perror("Errore in send_stringa_ben_formata: WRITE ERROR");
        exit(6);
    }
}

/*
* Funzione:  read_from_stream
* ---------------------------
* > Legge il contenuto dello stream e lo trascrive
*   sullo stdout.
* > Argomenti: filedescriptor di ascolto.
*/

void read_from_stream(int sd)
{
    char buff[DIM];
    int nread;
    while ((nread = read(sd, buff, DIM)) > 0)
    { // Leggo dallo stream
        if (write(1, buff, nread) < 0)
        { // Scrivo su stdout
            perror("Errore in read_from_stream: STDOUT ERROR");
            exit(9);
        }
    }
}
