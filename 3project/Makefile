all: math-game.o
	chmod +x *
	@echo Compiled all projects

math-game.o: math-game.cpp
	@echo Making math-game
	g++ math-game.cpp -lcurses -lgdbm -o math-game
