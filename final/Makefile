CFLAGS  = `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0`

all: client_server
	chmod +x *
	@echo Compiled Rock,paper,scissors!
client_server: player.cpp referee.cpp gui-player.c
	g++ player.cpp -o player
	gcc -rdynamic gui-player.c -o gui-player ${CFLAGS}
	g++ referee.cpp -o referee
clean: 
	rm -f player referee gui-player
	rm -f player.exe referee.exe player-gui.exe
