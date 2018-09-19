all: count_opens.o lsl.o kitten.o
	chmod +x *
	@echo Compiled all projects

count_opens.o: count-opens.cpp
	@echo Making count-opens
	g++ count-opens.cpp -o count-opens

lsl.o: lsl.cpp
	@echo Making lsl
	g++ lsl.cpp -o lsl

kitten.o: kitten.cpp
	@echo Making kitten
	g++ kitten.cpp -o kitten
