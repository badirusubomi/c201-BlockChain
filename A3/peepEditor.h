# ifndef PEEPEDITOR
# define PEEPEDITOR
# define PEEPLEN 256
# define MXTXNUM 64

# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <string.h>
 

/*
struct Modification{
  unsigned short position: 8;
  unsigned short character: 7;
  unsigned short event_type: 1;
} Modification;

struct Transaction{
  time_t timestamp;
  struct Modification mod;
} Transaction;
*/

/* edits the peep, saves transactions in modBuffer, and returns the number of transactions written in th ebuffer*/
unsigned int editor(char * peep, struct Transaction * modBuffer );

// Takes pointer to modification struct that will be populated by this function
// and pointer to the peep
// event_type should always be 0 in returned struct
// Returns 0 if successful or other if not
// Peep is updated too

int insert(struct Modification *, char *);

// Displays the 4 menu options and returns the chosen value
// Will continue polling for input until a valid option is chosen

int displayMenu(void);


// Displays the contents of the peep
// in the format specified by A3 Presentation
void displayPeep(char *);


// saves tr_no number of  transactions contained in modBuffer to a file - fName
// frees modBuffer
void saveTrans(struct Transaction *modBuffer, char *fName, unsigned int tr_no);

// accepts double pointer to Transaction array
// reads transactions from fName
//returns number of transactions read
int readTrans(struct Transaction **modBuffer, char *fName);

// Return 0 on successful deletion or other if not
// Details of deletion will be in mod
// Peep is updated too
int delete(struct Modification * mod, char * peep);


//
// print chars contained in peep
void printPeep(char *peep);

// Check Len funtion
// Returns the size of the peep
int check_len(char *peep);

# endif
