#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <stdlib.h>
#include "blockChain.h"
#include "peepEditor.h"

/*
- alex will write editor() and insert() helper function
- Subomi will write delete() helper function
- Collette will write displayMenu() and displayPeep()
*/

// Displays the 4 menu options and returns the chosen value
// Will continue polling for input until a valid option is chosen


int displayMenu(void)
{
  int choice;

  printf("Choose an Option\n");
  printf("1. View Characters\n2. Insert\n3. Delete\n4. Save and Exit\n");

    do
      {
	scanf("%d", &choice);

      } while (choice > 4 || choice < 1);  // Keep getting input until a valid choice

    return choice;
}

void displayPeep (char * peep)
{
  int count = 1; // Start from position 1

  while (*peep != '\0' && count < 257) // Keep printing until we reach NULL character or go beyond the end of the peep space
    {
      printf("%d:%c\t", count, *peep);
      count++; peep++;
    }
  putchar('\n'); // Newline
}

/*
        Check the length of the peep (not including null character)
*/
int check_len(char *peep){
  int i = 0;
  for (i = 0; i < 256; i++){
    if(peep[i] == '\0'){
      break;
    }

  }
  return i;
}



/*
         
*/
void printPeep(char *peep){
  int peepSize = check_len(peep);
  for(int i = 0; i < peepSize; i++){
    printf("%c",peep[i]);
  }
  printf("\n");
  return;
}



/*
Takes pointer to transaction array, filename to read from, and number of transactions
to read to file; be read,
NOTE: this function assumes modBuffer is allocated memory, and tries to free it

*/
void saveTrans(struct Transaction *modBuffer, char *fName, unsigned int tr_no){
  FILE *fOut = fopen(fName, "wb");
  fwrite(modBuffer, sizeof(struct Transaction), tr_no, fOut);
  //free(modBuffer); // needed??
  fclose(fOut);
  return;
}


/*
Takes double pointer to transaction array, and filename to read from; allocate memory for transactions
to be read, and return the pointer to the allocated memory
*/
int readTrans(struct Transaction **modBuffer, char *fName){
  FILE *fIn = fopen(fName, "rb");
  int tr_no = 0;
  tr_no = fseek(fIn, 0L, SEEK_END);
  tr_no = ftell(fIn)/sizeof(struct Transaction);
  fseek(fIn, 0L, SEEK_SET);
  *modBuffer = malloc(sizeof(struct Transaction) * tr_no);
  fread(*modBuffer, sizeof(struct Transaction), tr_no, fIn);
  fclose(fIn);

  return tr_no;
}

// Takes pointer to modification struct that will be populated by this function
// and pointer to the peep
// event_type should always be 0 in returned struct
// Returns 0 if successful or other if not
// Peep is updated too
int insert(struct Modification * mod, char * peep)
{

  int currentLength = check_len(peep); // current length is the number of actual characters in the peep
  // So peep[currentLength] points to the last character
  int validPosition = currentLength + 1;// And thus currentLength + 1 is the last valid position the user could enter
  int userPosition = 0;// Initialize to zero


  printf("Position to insert:");// Prompt to enter position
  scanf("%d", &userPosition);// Get a position from the user

  // If validPosition is 266 or larger, then the peep is full
  // if userPosition > validPosition, they entered a position that is too big
  // If user Position is 0 or negative it is invalid


  if (validPosition > 256 || userPosition > validPosition || userPosition < 1)
    {
      return 1;
    }


  char userCharacter = 0;

  while (userCharacter < 32 || userCharacter > 127)  // Loop until they enter a valid printable character (32-127 ASCII)
    {
      printf("Character to insert:");
      fflush(stdin);
      scanf(" %c", &userCharacter);
    }


  if (userPosition == validPosition)  // If they try to insert at the end, do it and set next char to null
    {
      peep[userPosition-1] = userCharacter;
      peep[userPosition] = '\0';
    }

  else if (userPosition > 0 && userPosition <= currentLength)
    {
      // Logic here to move everything at and beyond userPosition 1 space to the right and insert the new character
      char * temp = malloc(sizeof(char) * (currentLength - userPosition));
      strncpy(temp, &peep[userPosition-1], (currentLength-userPosition)+1);
      peep[userPosition-1] = userCharacter;
      strncpy(&peep[userPosition], temp, (currentLength-userPosition)+1);

      free(temp);
    }

  mod->position = userPosition;
  mod->character = userCharacter;
  mod->event_type = 0;

  return 0;
}


// Return 0 on successful deletion or other if not
// Details of deletion will be in mod
// Peep is updated too
int delete(struct Modification * mod, char * peep){
  // char ins_char = mod->character;

  int pos;
  int size = check_len(peep);
  printf("Position to delete:");          // Prompt to enter position
  scanf("%d", &pos);             // Get a position from the user
  if (pos > size){
    return 1;
  }

  //int e_type = mod->event_type;
  char *temp = malloc(sizeof(char) * (size - pos));
  char del_char;
  del_char = peep[pos-1];
  strncpy(temp,&peep[pos],size-pos);
  strncpy(&peep[pos-1], temp,size-pos);
  peep[size-1]= '\0';
  //memmove(&peep[pos-1],temp,sizeof(char) * (strlen(temp)));
  mod->position = pos;
  mod->character = del_char;
  mod->event_type = 1;
  free(temp);
  return 0;
}



/* parameters:   pointer to modBuffer, string (peep)
   return:       int to indicate number of transactions
   purpose:      displays the menu to the user, gets their choice
                 and executes it; if either insert() or delete()
		 is called, the number of transactions is
		 increased; modBuffer is reallocated to
		 store the new Transaction
   side effects: saveTrans() is called to write the transactions
                 to a file
*/
unsigned int editor(char * peep, struct Transaction * modBuffer)
{
  int transactions = 0;
  int menuChoice;
  int capacity = 10; // the main function initially allocates for
                     // 10 transactions
  int size = 0;
  
  menuChoice = displayMenu();

  struct Modification mod;
  struct Transaction currentTrans;

  while (menuChoice > 0 && menuChoice < 4)
    {
      //if(transactions == 64){return transactions;}

      switch (menuChoice)
	{
	case 1:
	  displayPeep(peep);
	  break;
	case 2:
	  // Call insert on the peep, if the operation is successful
	  // then increment the number of transactions
	  if (!(insert(&mod,peep)) )
	    {
	      transactions++; size++;
	      if (size == capacity)
		{
		  modBuffer = realloc(modBuffer, (size * 2) *  sizeof(struct Transaction));
		  capacity = size * 2;
		}
	      currentTrans.timestamp = time(NULL)  - 1609459200 - 31536000; // timestamp of current transaction
	      currentTrans.mod = mod; // modification part of current transaction
	      modBuffer[transactions-1] = currentTrans; // Add transaction pointer to modBuffer
	    }
	  break;
	case 3:
	  // Call delete on the peep, if the operation is successful
	  // then increment the number of transactions
	  if (!(delete(&mod, peep)))
	    {
	      transactions++; size++;
	      if (size == capacity)
		{
		  modBuffer = realloc(modBuffer, (size * 2) * sizeof(struct Transaction));
		  capacity = size * 2;
		}
	      modBuffer = realloc(modBuffer, (transactions + 1) * sizeof(struct Transaction));
	      currentTrans.timestamp = time(NULL) - 1609459200 - 31536000 ; // timestamp of the current transaction
	      currentTrans.mod = mod; // modification part of transaction
	      modBuffer[transactions-1] = currentTrans; // Add transaction pointer to modBuffer
	    }
	  break;
	}
      menuChoice = displayMenu();
    }
  saveTrans(modBuffer, "transactionOut", transactions);
  free(modBuffer);

  return transactions;
}


/*            Write transactions to a file
int main(void)
{
  char * peep = malloc(256); // Make space for 256 bytes
  memset(peep, '\0', 256); // Set all array elements to the null character
  struct Transaction * modBuffer = malloc(64 * sizeof(struct Transaction));
  int num = editor(peep, modBuffer);

  free(peep);
  //free(modBuffer);
  printf("number of transaction: %d\n", num);
}
*/
/*
int main(int argc, char **argv){  //Reads Transactions from a file and displays them 
  printf("\nReading transactions from file: transactionOut\n");
  struct Transaction *modBuffer =   modBuffer = readTrans(modBuffer, "transactionOut");
  for(int i = 0;i < 2 ;i++){ //argv[2] == no. of transactions in file
    printf("\nInserting %d\n", i+1);
    printf("char = %c; operation = %d; position = %d\n",modBuffer[i].mod.character, modBuffer[i].mod.event_type, modBuffer[i].mod.position);
  }
  
  return 0;
}
*/
