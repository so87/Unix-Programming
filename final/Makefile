CFLAGS  = `pkg-config --cflags --libs gtk+-3.0`

all: client_server
	chmod +x *
	@echo Compiled Rock,paper,scissors!
client_server: player.cpp referee.cpp gui-player.cpp
	g++ player.cpp -o player
	g++ gui-player.cpp -o gui-player ${CFLAGS}
	g++ referee.cpp -o referee
clean: 
	rm -f player referee gui-player
	rm -f player.exe referee.exe player-gui.exe
