all: main.o
	chmod +x *
	@echo Compiled client and server

main.o:
	g++ shout.cpp -o shout
	g++ shoutd.cpp -o shoutd
clean: 
	rm -f shout shoutd
