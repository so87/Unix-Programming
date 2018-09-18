all: count_opens.o lsl.o kitten.o
	@echo Compiled all projects

count_opens.o: count-opens/count-opens.cpp
	@echo Making count-opens
	g++ count-opens/count-opens.cpp -o count-opens

lsl.o: lsl/lsl.cpp
	@echo Making lsl
	g++ lsl/lsl.cpp -o lsl

kitten.o: kitten/kitten.cpp
	@echo Making kitten
	g++ kitten/kitten.cpp -o kitten
