/* Names: Olasubomi Badiru - 3101002 || Alex Lyndon 3111431 || Collette Patalinghog 3110938
 * Lab section: CMPT-201-X01L(1) and CMPT-201-X03L
 * assignment: A3
 * last worked: April 5th
 * due date: April 5th
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../blockChain.h"
#include "../peepEditor.h"


int main(int argc, char **argv){
  struct BlockChain *bc;
  char *peep;
  printf("\ntesting writeBlockChain() \n");
  //argv[1] is blockChain binary file
  bc = readBlockChain(argv[1]); 
  // first reads from sample blockChain file
  peep = getPeep(bc);
  printPeep(peep);
  free(peep);
  printf("Printing Transactions read using readBlockChain()\n");
  printTransactions(bc); //prints the transactions that were read
  
  struct Transaction *modBuffer;
 
  int tr_no = readTrans(&modBuffer,argv[2]); //argv[2 is transactions binary file]
  printf("\n\nAdding new transactions to BlockChain from %s (file containing %d  transactions)", argv[2], tr_no);
  printf("\nBlockChain should have one extra block\n\n\n");
  addTransactions(bc, modBuffer, tr_no); 
  free(modBuffer);
    
  printTransactions(bc); //print the transactions and compare with previous transactions that we added
  peep = getPeep(bc);
  printf("\nPeep after writing new transactions to block:");
  printPeep(peep);
  free(peep);
  destroyBlockChain(bc); 
  printf("\nFinished Testing write\n");
  return 0;
}
