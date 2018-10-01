// Example program illustrating gdbm database routines.
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gdbm.h>

#include <iostream>
#include "employee.h"

using namespace std;

int main()
{
   GDBM_FILE dbf;
   // datum contains 'char *dptr' and 'int dsize' members (see gdbm info)
   datum key, datain;

   struct keystr keydata;
   struct salrec salary;
   struct emprec employee;

   // open data base file
   dbf = gdbm_open("datafile.dat", 0, GDBM_READER, 0, 0);

   // get the key to the first record in the db
   key = gdbm_firstkey(dbf);
   // dptr is NULL if all keys have been retrieved
   while (key.dptr != NULL) {
      keydata = *(struct keystr *)key.dptr;
      // fetch data corresponding to this key
      datain = gdbm_fetch(dbf, key);
      if(keydata.type == EMPREC) {
         // copy the data into the employee variable
         employee = *((struct emprec *)(datain.dptr));
         free(datain.dptr);    // free mem allocated by gdbm
         cout << "Read employee record: " << endl;
         cout << employee.fname << " " << employee.lname << endl;
         cout << employee.phone << endl;
      }
      else if (keydata.type == SALREC) {
         // copy the data into the salary variable
         salary = *((struct salrec *)(datain.dptr));
         free(datain.dptr);    // free mem allocated by gdbm
         cout << "Read salary record: " << endl;
         cout << salary.salary << endl;
      }
      // get the key of the next record and repeat
      key = gdbm_nextkey(dbf, key);
   }

   gdbm_close(dbf);

   return 0;
}
