all: client server proto

proto: message.pb-c.c
		gcc -c message.pb-c.c

comunication_tools.o: comunication_tools.c comunication_tools.h
		gcc -g -c comunication_tools.c

client: Client.c message.pb-c.c comunication_tools.o
		gcc -o client Client.c comunication_tools.o message.pb-c.c -L/usr/lib -lprotobuf-c

server: Server.c message.pb-c.c comunication_tools.o
		gcc -o server Server.c comunication_tools.o message.pb-c.c -L/usr/lib -lprotobuf-c

