/* Names: Olasubomi Badiru - 3101002 || Alex Lyndon 3111431 || Collette Patalinghog 3110938
 * Lab section: CMPT-201-X01L(1) and CMPT-201-X03L
 * assignment: A3Plan.txt
 * last worked on: March 17th
 * due date: April 5th
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../blockChain.h"
#include "../peepEditor.h"


int main(int argc, char **argv){ //argv[1] or BlockChainOut
  struct BlockChain *bc;  
  printf("\ntesting readBlockChain() \n");
  bc = readBlockChain(argv[1]);
  printTransactions(bc);
  destroyBlockChain(bc);
  printf("\nFinished Testing read\n");
  return 0;
}
