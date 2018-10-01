#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gdbm.h>
#include <unistd.h>
#include <stdlib.h>
#include <curses.h>
#include <algorithm>
#include <vector>

#include <cstring>
#include <iostream>
#include <string>

using namespace std;

// function delclarations
string intro();
void printMiddle(string message, int row);
string getConsoleInput();
void printHighScores();
void increaseQ();
char generateVH();
char generateSign();
int generateNumb();
void quiz(char direction, char sign, int num1, int num2);
void updateScores(string name);

// structure: speed, occurrence, right answers
struct questions {int speed=100000; double occurrence=1; int correct=0;};

// db
struct hsRec{string name; int score;};


// defines
#define NAME_LEN 256
struct questions questionRate;
int correctAnswers = 1;

int main()
{
    // seed rand for generatoring random questions later
    srand(time(NULL));

    // get their user name
    string userName;
    userName = intro();  

    // play the game
    while(correctAnswers){
      // give them a question
      int num1 = generateNumb(); 
      int num2 = generateNumb();
      char sign = generateSign();
      char direction = generateVH(); 
      quiz(direction, sign, num1, num2);

      // make the game harder 
      if(correctAnswers)
        increaseQ(); 
    }
    // update high scores if someone was beat 
    updateScores(userName); 

    // print out scores
    printHighScores();
    return 0;
}


void printMiddle(string message,int row)
{
    // print to the middle of the screen
    move(row,(COLS-strlen(message.c_str())-1)/2);
    printw(message.c_str());
    return;
}

string getConsoleInput()
{
    string name;
    int i,c;
    for(i = 0; i < NAME_LEN; i++) {
        // get a character from standard input
        c = getch();
        if (c == '\n') break;
        name += c;
    }
    return name; 
}

// print out intro and get their user name
string intro(){
    // clean out terminal and make buffer for messages 
    initscr();
    cbreak();
    refresh();

    // get user name from user
    printMiddle("--------------------------------------------------",0);
    attron(A_BOLD|A_BLINK);
    printMiddle("                     Math Games                   ",1);
    attroff(A_BOLD| A_BLINK);
    printMiddle("          by: simon owens    for: simon owens     ",2);
    printMiddle("                graphics: simon owens             ",3);
    printMiddle("                                                  ",4);
    printMiddle("'If you don't like math then $#@* you'-ricky bobby",5);
    printMiddle("--------------------------------------------------",6);
    printMiddle("Please write your user name:",10);
    attron(A_BOLD| A_BLINK);
    printMiddle("User name: ",12);
    attroff(A_BOLD| A_BLINK);
    cbreak();
 
    // get input from console
    string name = getConsoleInput();

    // remove screen stuff
    printMiddle("starting game.....", 15);
    refresh();sleep(2);
    clear();

    return name;
}


// generate random 'V' or 'H' for vertical or horizontal
char generateVH()
{
  char VH;
  int flip = rand()%2;
  if (flip)
    VH = 'V';
  else
    VH = 'H'; 

  return VH;
}

// generate '+' '-' '*' for the different operators. devide is too hard
char generateSign()
{
  char type;
  int flip = rand()%4;
  if (!flip)
    type = '+';
  else if (flip == 1)
    type = '-';
  else if (flip == 2)
    type = '*';
  else
    type = '+';

  return type;
}

// generate a random number -20 to 20 that is even
int generateNumb()
{
  int num;
  num = -20 + (rand()%(20-0+1));
  return num;
}


// move the question across the screen and wait for correct answer
void quiz(char direction, char sign, int num1, int num2)
{
    // prompt for answer and move it across screen
    // move it based on direction
    // move it so fast
    // when gets to edge set incorrect and break
    string message = "Question: "+to_string(num1)+" "+sign+" "+to_string(num2)+" = ?";
   
    int done = 0;
    int size = 0;
    int row = 0;
    // move across the screen until it hits a limit
    int randomCol = 40 + (rand() % static_cast<int>(70 - 40 + 1));
    int randomRow = 50 + (rand() % static_cast<int>(70 - 50 + 1));
    clear();refresh();
    while(!done){
      clear();
      // move across the screen horizontal
      if (direction == 'H'){
        move(randomRow-message.length(), size);
        printw(message.c_str());
	refresh();
	usleep(questionRate.speed);
	size++;
	if(size == COLS){
	  done = 1;
	  clear();refresh();
	}
      } 
      // move across the screen vertical
      if(direction == 'V'){
        move(size,randomCol-message.length());
        printw(message.c_str());
	cout << randomCol << endl;
	refresh();
	usleep(questionRate.speed);
        size++;
        if(size == LINES){
          done = 1;
          clear();refresh();
        }
      }
    }
    attroff(A_BOLD| A_BLINK);
    cbreak();
    printMiddle("What was the answer: ",1);
    refresh();
    size_t sz;
    string stringAns = getConsoleInput();
    if (stringAns.length() > 3){
	clear();
	refresh();
        printMiddle("warning that is way out of bounds... stoping game",5);
        correctAnswers = 0;
        refresh();sleep(3);clear();
        return; 
    }
    int userAnswer = stoi(stringAns,&sz);

    // calculate right answer
    int answer;
    if (sign == '+')
      	answer = num1 + num2;
    else if (sign == '-')
	answer = num1 - num2;
    else
	answer = num1 * num2;

    //check if their answer was right and set "correctAnswers"
    if (userAnswer != answer){
	clear();
        refresh();
        printMiddle("wrong answer!",5);
	printMiddle("the right answer was "+to_string(answer)+" ",6);
	printMiddle("you entered: "+to_string(userAnswer)+" ",7);
	printMiddle("You entered: "+to_string(questionRate.correct)+" correct answers!",8);
        correctAnswers = 0;
        refresh();sleep(5);clear();
	correctAnswers  = 0;
    }
    clear();
}

// because c++ can't conver ttypes and because DBM is terrible
void add_to_scores(GDBM_FILE db, string nameS, string scoreS){
    datum key, data; // Key and value (date and event).
    char *name = new char[nameS.length() + 1];
    strcpy(name, nameS.c_str());
    char *score = new char[scoreS.length() +1];
    strcpy(score, scoreS.c_str());

    // add the user and their score to the database
    key.dptr = name;
    key.dsize = strlen(name);
    data.dptr = score;
    data.dsize = strlen(score);
    // Storing!
    gdbm_store(db, key, data, GDBM_REPLACE);
    delete [] name;
}

// open the file and write the updated high score. make sure to sort
void updateScores(string name)
{
  // check if they made the top score
  // open the file
    GDBM_FILE file;
    datum current_key, current_data;
    // check to see if this is the first time someone has played
     if (FILE *file = fopen("highscores.dat", "r")) {
	fclose(file);
     }
    // make an initial computer user for the database 
     else{
       	GDBM_FILE fileG; 	
     	fileG = gdbm_open("highscores.dat", 512, GDBM_WRCREAT, 0666, 0);
        add_to_scores(fileG, "Computer", "1");
	gdbm_close(fileG);
     }

    file = gdbm_open("highscores.dat", 512, GDBM_WRCREAT, 0666, 0);

    // get the places. the data file is already sorted
    add_to_scores(file, name, to_string(questionRate.correct));

    // close file
    gdbm_close(file);
}

// increase the movespeed and rate of equation coming out
void increaseQ()
{
   questionRate.speed = questionRate.speed -10000;
   questionRate.occurrence = questionRate.occurrence +1.5; 
   questionRate.correct = questionRate.correct +1;
}

bool compareArray(const hsRec &a, const hsRec &b)
{
    return a.score < b.score;
}

// print out high score
void printHighScores()
{
    // open the file
    GDBM_FILE file;
    datum current_key, current_data;
    file = gdbm_open("highscores.dat", 512, GDBM_WRCREAT, 0666, 0);

    // get the places. the data file is already sorted
    vector<struct hsRec> db;

    // key should be first entry
    int count = 0;
    for(current_key = gdbm_firstkey(file);
        current_key.dptr != NULL;
        current_key = gdbm_nextkey(file, current_key))
    {
        // Fetch the data associated with that key.
        current_data = gdbm_fetch(file, current_key);
        if(current_data.dptr != NULL){
            db.push_back(current_data.dptr,atoi(current_key.dptr));
	    count++;
	}
    }
    // not working. comparision is not defined
     
    // find the 3 highest scores
    sort(db.begin(), db.end(), compareArray); 
    

    // print out to the screen
    printMiddle("High Scores!",0);
    printMiddle("-----------------------------------",1);
    printMiddle("First  : "+db[0].name+" with "+to_string(db[0].score)+ " points!",2);
    printMiddle("Second : "+db[1].name+" with "+to_string(db[1].score)+" points!",3);
    printMiddle("Third  : "+db[2].name+" with "+to_string(db[2].score)+ " points!",4);
    printMiddle("-----------------------------------",7);
    printMiddle("Play again if you aren't scared....",15);

    // clean out screen
    echo();
    nocbreak();
    refresh();sleep(10);
    endwin();


    // close the file
    gdbm_close(file);

}
