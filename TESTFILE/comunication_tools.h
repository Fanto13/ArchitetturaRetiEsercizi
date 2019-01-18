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

void proto_send_nodim(int sd, Com1 *risposta);

Com1 *proto_recive_nodim(int sd);

void read_stringa_ben_formata(int sd, char *stato);

void send_stringa_ben_formata(int sd, char *string);


void write_on_socket(int sd);

void read_from_stream(int sd);
