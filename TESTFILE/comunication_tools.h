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

void pipe_to_upper_level(int pipedesc, int sd );

void pipe_from_lower_level(int pipedesc);

int ricevi(int sd, char *buf, int n);

void proto_send_nodim(int sd, Com1 *risposta);

Com1 *proto_receive_nodim(int sd);

void read_stringa_ben_formata(int sd, char *stato);

void send_stringa_ben_formata(int sd, char *string);

/*
 * Send non ben formata
 * recv non ben formata
 */
void proto_send_dim(int sd, Com1 *risposta);

Com1 *proto_receive_dim(int sd);

void send_with_ack(int sd, char *string1, char *string2);

void receive_with_ack(int sd, char *string1, char *string2);


void write_on_socket(int sd);

void read_from_stream(int sd);
