enum rec_type { EMPREC, SALREC };

// keys must be unique
// we construct a unique key by using both the employee id and the
// record type in the key.
struct keystr { int id; rec_type type; };

struct salrec { int id; int salary; };

struct emprec { int  id; char lname[25]; char fname[25]; char phone[25]; } ;

