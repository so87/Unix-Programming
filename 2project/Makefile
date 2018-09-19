all: count_opens.o lsl.o kitten.o
	chmod +x *
	@echo Compiled all projects

count_opens.o: count-opensSrc/count-opens.cpp
	@echo Making count-opens
	g++ count-opensSrc/count-opens.cpp -o count-opens

lsl.o: lslSrc/lsl.cpp
	@echo Making lsl
	g++ lslSrc/lsl.cpp -o lsl

kitten.o: kittenSrc/kitten.cpp
	@echo Making kitten
	g++ kittenSrc/kitten.cpp -o kitten
