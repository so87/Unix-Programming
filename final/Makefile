all: client_server
	chmod +x *
	@echo Compiled Rock,paper,scissors!
client_server: client.cpp server.cpp
	g++ client.cpp -o client
	g++ server.cpp -o server
clean: 
	rm -f client server
	rm -f client.exe server.exe
