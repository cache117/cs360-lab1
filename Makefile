all: server download
download: download.cpp
	g++ -o download download.cpp
client: client.cpp
	g++ -o client client.cpp
server: server.cpp
	g++ -o server server.cpp
clean: 
	rm client server download
