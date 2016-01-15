all: client server download
download: download.c
	gcc -o download download.c
client: client.c
	gcc -o client client.c
server: server.c
	gcc -o server server.c
clean: 
	rm client server download
