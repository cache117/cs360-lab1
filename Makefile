all: server download
download: download.c
	g++ -o download download.cpp
client: client.c
	g++ -o client client.cpp
server: server.c
	g++ -o server server.cpp
clean: 
	rm client server download
