all: master.o
	chmod +x *
	@echo Compiled all projects

master.o: master.cpp upper.cpp reverse.cpp
	@echo Making the programs
	g++ master.cpp -o master
	g++ upper.cpp -o upper
	g++ reverse.cpp -o reverse
