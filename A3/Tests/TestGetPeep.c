/* Names: Olasubomi Badiru - 3101002 || Alex Lyndon 3111431 || Collette Patalinghog 3110938
 * Lab section: CMPT-201-X01L(1) and CMPT-201-X03L
 * assignment: TestGetPeep.c
 * last worked on: March 17th
 * due date: April 5th
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../blockChain.h"
#include "../peepEditor.h"


int main(int argc, char **argv){ //argv[1] or BlockChainOut

  char *peep;
  struct BlockChain *bc;
  bc = malloc(sizeof(struct BlockChain)*1);
  bc = readBlockChain(argv[1]);
  
  /*  struct Transaction *modBuffer;

  int tr_no = readTrans(&modBuffer, argv[2]);

  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  printTransactions(bc);
  writeBlockChain(argv[1],bc);
  */
  printTransactions(bc);
  peep = getPeep(bc);

  printf("\nPeep:- %s\n",peep);
  return 0;
}
