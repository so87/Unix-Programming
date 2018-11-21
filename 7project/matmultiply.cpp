#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

void multSingleThread(int** a, int** b,int size);                   
void multMultipleThread(int** a, int** b,int size);

#define NUM_THREADS 4 // my computer has 4 cores which makes using 4 threads a good balance of performance

struct arg_struct {
  int** A;
  int** B;
  int size;
  int j;
  int i;
  int k;
  int answer = 0;
};

int main(int argc, char *argv[]) {                    
    if (argc != 3) {
	cerr << "usage: <s | m> <size>" << endl;
	exit(1);
    }
    
    string choice = argv[1];
    if ((choice != "s") && (choice !="m")){
	cerr << "error: choose s or m for the first argument" << endl;
	return 1;
    }
    int size = atoi(argv[2]);

    // matrix A
    int** a = new int*[size];
    for(int i = 0; i < size; ++i){
      a[i] = new int[size];
      for(int j = 0; j < size; ++j)
        a[i][j] = rand() % 10;
    }
    // matrix B
    int** b = new int*[size];
    for(int i = 0; i < size; ++i){
      b[i] = new int[size];
      for(int j = 0; j < size; ++j)
        b[i][j] = rand() % 10;
    }

    cout << "__________" << endl;
    if(choice == "m"){
      cout << "Running with multiple threads" << endl;
      multMultipleThread(a, b, size);
    }
    else if (choice == "s"){ 
      cout << "Running with one thread" << endl;
      multSingleThread(a, b, size);
    }
    cout << endl << "___________" << endl;


    // delete matrix A on heap
    for(int i = 0; i < size; ++i) {
      delete [] a[i];
    }
    delete [] a; 
    // delete matrix B on heap
    for(int i = 0; i < size; ++i) {
      delete [] b[i];
    }
    delete [] b;

    // close out those threads
    pthread_exit(NULL);    
    return 0;
}


// ensure to print out first/last element
void multSingleThread(int** A, int** B,int size) {

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int num = 0;
            for (int k = 0; k < size; k++) {
                num += A[i][k] * B[k][j];
            }

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

void *multiply(void* arguments){
  struct arg_struct *args = (arg_struct*) arguments;
  int k = args->k;
  int j = args->j;
  int i = args->i;
  args->answer += args->A[i][k] * args->B[k][j];
}


// ensure to print out first/last element
void multMultipleThread(int** A, int** B, int size) {
    pthread_t threads[NUM_THREADS];
    long t;
    struct arg_struct args;
    void *thread_result;
    args.size = size;
    args.A = A;
    args.B = B;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int num = 0;
	    args.answer = 0;
	    int k = 0;
	    args.i = i;
	    args.j = j;
	    for (int k = 0; k < size; k++) {
		args.k = k;
		int threadstatus[5];
		pthread_create (&threads[1], NULL, &multiply, &args);
		if(k+1 < size){
		  k++;
		  args.k = k;
		  pthread_create (&threads[2], NULL, &multiply, &args);
		  threadstatus[1] = 1;
		}
		if(k+2 < size){
                  k++;
                  args.k = k;
                  pthread_create (&threads[3], NULL, &multiply, &args);
                  threadstatus[2] = 1;
                }
		if(k+3 < size){
                  k++;
                  args.k = k;
                  pthread_create (&threads[4], NULL, &multiply, &args);
                  threadstatus[3] = 1;
                }
		pthread_join(threads[1], &thread_result);
		if(threadstatus[1])	
		  pthread_join(threads[2], &thread_result);
		if(threadstatus[2])
                  pthread_join(threads[3], &thread_result);
		if(threadstatus[3])
                  pthread_join(threads[4], &thread_result);

            }
	    num = args.answer;
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
