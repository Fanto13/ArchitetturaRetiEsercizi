all: client server proto

proto: message.pb-c.c
		gcc -c message.pb-c.c

client: Client.c message.pb-c.c
		gcc -o client Client.c message.pb-c.c -L/usr/lib -lprotobuf-c

server: Server.c message.pb-c.c
		gcc -o server Server.c message.pb-c.c -L/usr/lib -lprotobuf-c
