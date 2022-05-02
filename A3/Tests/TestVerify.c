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


int main(int argc, char **argv){ //argv[1] or BlockChainOut
  struct BlockChain *bc = malloc(sizeof(struct BlockChain));
  int result;
  printf("\nTesting verifyBlockChain \n");
  struct Transaction * modBuffer;
  int tr_no = readTrans(&modBuffer, argv[1]);
  //char * peep;
  addTransactions(bc, modBuffer, tr_no); // CALL ADD TRANSACTIONS 3 times 
  addTransactions(bc, modBuffer, tr_no);
  addTransactions(bc, modBuffer, tr_no);
  
  result = verifyBlockChain(bc);

  if (!(result))
    printf("BlockChain was successfully verified.\n");
  else  
    printf("BlockChain integrity compromised.\n");

  bc->chain[0].transactions[0].mod.character='X'; // Modify the first transaction in the first block
  bc->chain[6].transactions[11].mod.character='Y'; // Modify block 7 transaction 12 
  // The two lines above will corrupt the block chain and we check with verify again below
    result = verifyBlockChain(bc);

  if (!(result))
    printf("BlockChain was successfully verified.\n");
  else  
    printf("BlockChain integrity compromised.\n");

  bc->chain[0].transactions[0].mod.character='a'; // Change it back
  bc->chain[6].transactions[11].mod.character='j'; // change it back

  result = verifyBlockChain(bc);

  if (!(result))
    printf("BlockChain was successfully verified.\n");
  else  
    printf("BlockChain integrity compromised.\n");


  printf("\nFinished Testing verify\n");
  return 0;
}
