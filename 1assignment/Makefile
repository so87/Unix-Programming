make-lib:
	@echo Making object files for library
	g++ -c mysin.cpp -o mysin.o
	g++ -c mycos.cpp -o mycos.o
	g++ -c mytan.cpp -o mytan.o 
	@echo 
	@echo Adding object files to library
	ar rv libmytrig.a mysin.o mycos.o mytan.o
	ranlib libmytrig.a
	@echo 
	@echo Compiling main program
	g++ -c main.cpp -o main.o
	@echo 
	@echo Linking main program to libmytrig.a
	g++ main.o -o driver -L. -lmytrig
