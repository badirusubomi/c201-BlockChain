/* Names: Olasubomi Badiru - 3101002 || Alex Lyndon 3111431 || Collette Patalinghog 3110938
 * Lab section: CMPT-201-X01L(1) and CMPT-201-X03L
 * assignment: TestAdd.txt
 * last worked on: March 17th
 * due date: April 5th
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../blockChain.h"
#include "../peepEditor.h"


int main(int argc, char **argv)
{ //argv[1] or BlockChainOut
  struct BlockChain *bc = malloc(sizeof(struct BlockChain));

  printf("\nTesting addTransactions \n");
  struct Transaction * modBuffer;
  int tr_no = readTrans(&modBuffer, argv[2]);    
  char * peep;
  addTransactions(bc, modBuffer, tr_no); // CALL ADD TRANSACTIONS 3 times 
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  peep = getPeep(bc);

  printTransactions(bc);                // Print all the transactions on the blockChain
  
                   // Get the peep
  int length = check_len(peep);
  printf("%d\n", length);         // Should be 69 characters long
  for (int q=0; q<length; q++)
  {
      printf("%c", peep[q]);
  }
  
  printf("\nFinished Testing addTransactions\n");
  return 0;
}
