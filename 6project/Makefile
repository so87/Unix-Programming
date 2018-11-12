all: main.o
	chmod +x *
	@echo Compiled with pre c++-11 bc of cserver
	@echo Compiled client and server

main.o:
	g++ shout.cpp -o shout
	g++ shoutd.cpp -o shoutd
clean: 
	rm -f shout shoutd
	rm -f shout.exe shoutd.exe
