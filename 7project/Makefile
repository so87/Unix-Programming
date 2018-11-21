all: main.o
	chmod +x *
	@echo Compiled matmultiply
main.o:
	g++ matmultiply.cpp -lpthread -o matmultiply
clean: 
	rm -f shout shoutd
	rm -f shout.exe shoutd.exe
