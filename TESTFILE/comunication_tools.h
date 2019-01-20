//
// Created by giacomo on 09/01/19.
//
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

#include "message.pb-c.h"
#include "struct.h"


/*
FATTI:
proto send/recive dim       OK
proto send/recive no dim    OK
send/recive ben formata     OK
send/recive ack             OK
read/write on stream        OK
----------------------------------
DA TESTARE:
Pipe da e verso             TEST
----------------------------------
DA FARE:
STRUCT
Stringa lunghezza variabile
*/

int ricevi(int sd, char *buf, int n);

void pipe_to_upper_level(int pipedesc, int sd );
void pipe_from_lower_level(int pipedesc);

void proto_send_nodim(int sd, Com1 *risposta);
Com1 *proto_receive_nodim(int sd);

void read_stringa_ben_formata(int sd, char *string);
void send_stringa_ben_formata(int sd, char *string);

void proto_send_dim(int sd, Com1 *risposta);
Com1 *proto_receive_dim(int sd);

void send_with_ack(int sd, char *string1, char *string2);
void receive_with_ack(int sd, char *string1, char *string2);

void write_on_socket(int sd);
void read_from_stream(int sd);

void send_struct(int sd, Struttura struct);
Struttura *recive_struct(int sd);
