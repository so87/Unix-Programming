#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char * argv[])
{
  string userInput = argv[2];
  
  // acquire the semaphore
  sem_t *sem_p;
  char const *sem_name = argv[1];
  sem_p = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);
  sem_wait(sem_p);

  // capitolize everything
  locale loc;
  for (size_t i=0; i <userInput.length(); ++i)
    userInput[i] = toupper(userInput[i],loc); 

  cout << userInput << endl;

  // release the semaphore
  sem_post(sem_p);
  sem_close(sem_p);
  sem_unlink(sem_name); 

  return 0;
}
