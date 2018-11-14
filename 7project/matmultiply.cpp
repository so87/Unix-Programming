#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

void multS(int** a, int** b,int size);                   
void multM(int** a, int** b,int size);

int main(int argc, char *argv[]) {                    
    if (argc != 3) {
	fprintf(stderr, "usage: s|m matrice_size \n");
	exit(1);
    }
    
    string choice = argv[1];
    int size = atoi(argv[2]);
    srand(time(NULL));

    // allocate those matrices on the heap
    int** a = new int*[size];
    for(int i = 0; i < size; ++i){
      a[i] = new int[size];
      for(int j = 0; j < size; ++j)
        a[i][j] = rand() % 10;
    }

    int** b = new int*[size];
    for(int i = 0; i < size; ++i){
      b[i] = new int[size];
      for(int j = 0; j < size; ++j)
        b[i][j] = rand() % 10;
    }

    // run with a single or multiple thread
    cout << "__________" << endl;
    if(choice == "m"){
      multM(a, b, size);
    }
    else{ 
      multS(a, b, size);
    }
    cout << endl << "___________" << endl;


    // delete the heap
    for(int i = 0; i < size; ++i) {
      delete [] a[i];
    }
    delete [] a; 
    for(int i = 0; i < size; ++i) {
      delete [] b[i];
    }
    delete [] b;

    
    return 0;
}


// ensure to print out first/last element
void multS(int** A, int** B,int size) {
    int C[size][size];

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int num = 0;
            for (int k = 0; k < size; k++) {
                num += A[i][k] * B[k][j];
            }

            C[i][j] = num;
	    if(i <= 1 && j <= 1)
              std::cout << num << " ";
	    if(i == 2 && j == 2)
	      std::cout << "    ... ";

	    if(i+1==size && j+1 == size)
	      std::cout << endl << "        "<< num << " ";
        }
	if(i <= 1)
          std::cout << std::endl;
    }
}

// ensure to print out first/last element
void multM(int** A, int** B, int size) {
    int C[size][size];

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int num = 0;
            for (int k = 0; k < size; k++) {
                num += A[i][k] * B[k][j];
            }

            C[i][j] = num;
            if(i <= 1 && j <= 1)
              std::cout << num << " ";
            if(i == 2 && j == 2)
              std::cout << "    ... ";

            if(i+1==size && j+1 == size)
              std::cout << endl << "        "<< num << " ";
        }
        if(i <= 1)
          std::cout << std::endl;
    }
}
