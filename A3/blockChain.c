// The maximum number of transactions allowed in one block.
#define MXTXNUM 64
#define PEEPLEN 256
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>
#include "blockChain.h"
#include "peepEditor.h"

/*
DATA STRUCTURES

Structure to Represent a modification [2 bytes]. (modification)
position- Position in the peep that the event occured: Maximum position of 256
haracter- The character that was modified  - ASCII codes 32 to 127
event_type- Type of event that has occured.
  0 for Insert
    1 for Delete
NOTE: Bitfields must be used to condense data in memory and when written


Structure to Represent a Transaction [8 bytes] (transaction) 
timestamp- Time that the event occurred (EPOCH time since January 1-st 2021). [6 bytes]
modification- The modification part of the transaction.                          [2 bytes]

Structure to Represent a Block  [32 bytes for the  head + up to 8*64 bytes for transactions] (block)
previousHash- Hash of the Previous block head. 0 if first          [7 bytes]
timeStamp- Time that the hashing of the block occured           [6 bytes]
blockNum - Incremental number of the block.                     [4 bytes]
blockSize- Size of transaction elements in the block            [1 byte]
blockHash- The Merkle tree hash of the transactions             [7 bytes]
                                
                                
headHash- Hash of the head elements above                      [7 bytes]
      
transactions- Transactions contained in the block (maximum of 64 transactions)

NOTE: a hash of length 7 bytes is obtained by taking the last 7 bytes of the sha256 hash 
      The truncation is to be applied only as the final step. 
      All intermediary hashes are full sha256 hashes

FUNCTIONS

writeBlockChain - writes a block chain to a file

readBlockChain - reads a block chain from a file 

verifyBlockChain - verifies the integrity of the block chain

addTransactions - adds a new session of transactions at the end of the block chain starting with a new block

printTransactions - prints all transactions contained in the blockchain

getPeep - produces the current peep by replaying all the transactions contained in the block chain

getPeepAtTime - produces the peep at a certain time stamp by replaying all the transactions in the block chain up to (and including) the time stamp

*/






/*   To Print The digest in hex form */
void printDigest(unsigned char *digest){
  for(unsigned char *i = digest; *i != '\0'; i++){
    printf("%X",*i);
  }
  printf("\n");
  return;
}



/* parameters: name of file, pointer to a BlockChain
   return: int to indicate successful write */
int writeBlockChain (const char * filename, struct BlockChain *bc){
  //  int i = 0, j = 0, no_entries = 0;
  FILE *fOut = fopen(filename,"wb");
  /*  
  for(i = 0; i < bc->size; i++){
    for(j = 0; j < 64; j++){   //can modify to write a block at once
      fwrite(&(bc->chain[i].transactions[j]), sizeof(struct Transaction), 1,fOut );
      no_entries++;
    }
  }
  */
  fwrite(bc->chain, sizeof(struct Block), bc->size, fOut);
  fclose(fOut);
  return bc->size;
}


/* parameters: name of file
   return:     pointer to a BlockChain */
struct BlockChain *readBlockChain (const char * filename){
  int i = 0, no_entries = 0;
  FILE *fIn = fopen(filename,"rb");
  int block_no = 0;
  fseek(fIn, 0L, SEEK_END);
  block_no = ftell(fIn)/sizeof(struct Block);
  fseek(fIn, 0L, SEEK_SET);
  
  struct BlockChain *bc = malloc(sizeof(struct BlockChain) * 1);
  bc->chain = malloc(sizeof(struct Block) * block_no);  //assumes blockchain isempty
  bc->size = block_no;   //currently one block in our blockchain
 
  for(i = 0; !(feof(fIn)) && !(ferror(fIn)) && i < block_no;i++){
      fread(&(bc->chain[i]), sizeof(struct Block), 1, fIn);
      no_entries++;
  }
 
  return bc;
}


char * getMerkleRoot(struct Transaction * trx, unsigned int n)
{
	int oddFlag = !((n % 2) == 0); // oddFlag is 1 when n is odd, and 0 when n is even
	char ** transactionHashes;	
	if (oddFlag)
	{
		transactionHashes = malloc(sizeof(char *) * (n+1)); // Allocate space for (n+1) char pointers
	}
	else 
	{
		transactionHashes = malloc(sizeof(char *) * n); // Room for n char pointers
	}

// 	EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
	for (int i=0; i < n; i++) // Loop once for each transaction
	{
		transactionHashes[i] = hashOneTransaction(trx[i]);
		//strncpy(*(transactionHashes[i]), digest, 32); // Copy the 256 bits 
	}

	if (oddFlag)
	{
		transactionHashes[n] = transactionHashes[n-1]; // Copy the last hash
		n++; // We added an extra hash
	}

	return recursiveMerkle(transactionHashes, n);

}

char * recursiveMerkle(char ** hashes, int n) // Takes a pointer to a series of char pointers (which point to hashes) and int n 
{
	// Base case:
	if (n == 1)
		return hashes[0]; // Return the char pointer that is stored in hashes[0]

	char ** newHashes = malloc(sizeof(char *) * (n/2)); // Allocate room for n/2 character pointers

	// N should only be even when it is passed here

	unsigned int digest_len = EVP_MD_size(EVP_sha256());
		
	EVP_MD_CTX * mdctx;
	mdctx = EVP_MD_CTX_create();


	for (int i=0; i < n-1; i+=2) // loop once for each hash 
	{
		char * digest = (char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()));

		EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
		EVP_DigestUpdate(mdctx, hashes[i], sizeof(hashes[i]));
		EVP_DigestUpdate(mdctx, hashes[i+1], sizeof(hashes[i+1]));
		EVP_DigestFinal_ex(mdctx, (unsigned char *)digest, &digest_len);
		newHashes[i/2] = malloc(sizeof(char) * 32);
		strncpy(newHashes[i/2], digest, 32);
		//EVP_MD_CTX_reset(mdctx);
	}
	EVP_MD_CTX_destroy(mdctx);
	// At this point there should be n/2 hashes in newHashes
	// and we call recursive merkle
	return recursiveMerkle(newHashes, n/2);
}


/* parameters: pointer to a BlockChain
   return:     int to indicate successful verification */
   
int verifyBlockChain (struct BlockChain * bc){
	struct Block * current;  
	int i;	
	char * generatedHash;
	char * truncatedRootHash;


	for (i=0; i<bc->size; i++)
	{
		current = &(bc->chain[i]);
	
	
	// Generate Merkle Root Hash from all the transactions
		struct Transaction * trx = (struct Transaction *) &(current->transactions);
	
		generatedHash = getMerkleRoot(trx, current->blockSize);
		truncatedRootHash = malloc(7 * sizeof(char));

		strncpy(truncatedRootHash, (generatedHash+25), 7); // Copy last 7 bytes from full hash 

		if (memcmp(truncatedRootHash, current->blockHash, 7))
		{
			return 1; // They did not match! 
		}		
		
		if (i < bc->size-1) // if we're not on the last block
		{
			int different = memcmp(current->headHash, bc->chain[i+1].previousHash, 7);
			if (different)
				return 1; // Current head hash did not match previousHash of next Block !!
		}
		
	}	

	free(truncatedRootHash);

	return 0;
}

/* parameters: pointer to a BlockChain, pointer to a Transaction,
               int to indicate number of transactions
	       return:     none */
 void addTransactions (struct BlockChain * bc, struct Transaction * trx, int n){
	struct Block newBlock;
	unsigned int digest_len=EVP_MD_size(EVP_sha256());
	unsigned int * lengthPointer = &(digest_len);
    char *digest = (char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()));

        EVP_MD_CTX *mdctx;
        mdctx = EVP_MD_CTX_create();
	
	EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
        
	if (bc->size == 0) // The block chain is empty
	{
		for (int i=0; i<7; i++)
			newBlock.headHash[i] = '0';	
	}
	else if (bc->size > 0)
	{
		strncpy(newBlock.previousHash, bc->chain[bc->size-1].headHash, 7); // Copy the 7 characters of the 
									  // previous block's head hask into previousHash
	}
	
	time_t stamp = time(NULL) - 1609459200 - 31536000;
	newBlock.timeStamp = stamp;


	int blockChainSize = bc->size;	
	newBlock.blockNum = blockChainSize;
	newBlock.blockSize = n;
	
	
	char * root = getMerkleRoot(trx, n); // Will return the full 64 character hash

	// HERE GET THE LAST 7 BYTES OF ROOT AND SET newBlock.blockHash = root[-7:-1]
	
	strncpy(newBlock.blockHash, (root+25), 7); // copy the last 7 bytes

	/*
	printf("The blockHash stored was: \n");
	for (int q=0; q<7; q++)
	{
		printf("%02x\t", newBlock.blockHash[q]);
	}*/

	EVP_DigestUpdate(mdctx, &stamp, sizeof(time_t));
	EVP_DigestUpdate(mdctx, newBlock.previousHash, sizeof(newBlock.previousHash));
	EVP_DigestUpdate(mdctx, &blockChainSize, 4);
	EVP_DigestUpdate(mdctx, &(newBlock.blockSize), sizeof(newBlock.blockSize));
	EVP_DigestUpdate(mdctx, newBlock.blockHash, sizeof(newBlock.blockHash));

	EVP_DigestFinal(mdctx, (unsigned char *)digest, lengthPointer);
	
	strncpy(newBlock.headHash, (digest+25), 7); //Copy last 7 bytes of head hash
	/*
	printf("\nThe headHash being stored is: \n");
	for (int q=0; q<7; q++)
	{
		printf("%02x\t", newBlock.headHash[q]);
	}
	*/
	EVP_MD_CTX_destroy(mdctx);

	for (int i=0; i<n; i++)
	{
		newBlock.transactions[i] = trx[i]; // Copy the transactions from trx into the new Block
	}


	if (bc->size == 0)
	{
		bc->chain = malloc(sizeof(struct Block));
		bc->size = bc->size + 1;
		bc->chain[0] = newBlock;
	}
	else if (bc->size > 0)
	{
		bc->chain = realloc(bc->chain, sizeof(struct Block) * (bc->size + 1));
		bc->size++;
		bc->chain[bc->size-1] = newBlock;
	}
   return;
 }
 

/* parameters: pointer to a BlockChain
   return:     none */
 void printTransactions (struct BlockChain * bc){
   int i,j;
   for (i=0; i< bc->size; i++)
   {
     printf("\nBlock[%d] with %d transactions:-\n", i, bc->chain[i].blockSize);
	for (j=0; j<(bc->chain[i].blockSize); j++)
	{
		printIndividualTransaction(bc->chain[i].transactions[j]);
	}
   }

   return;
 }



/* helper function for getPeep and getPeepAtTime */
void insertP(struct Modification mod, char * peep)
{
  unsigned short pos = mod.position, chr = mod.character;

  int peeplen = check_len(peep);

  if (pos == (peeplen+1))
  {
	  peep[pos-1] = chr;
	  peep[pos] = '\0';
  }
  else if (pos > 0 && pos <= peeplen)
  {
  	char * temp = malloc(sizeof(char) * (peeplen - pos));
	// algorithm taken from insert function in peepEditor.c
 	strncpy(temp, &peep[pos - 1], (peeplen - pos) + 1);
  	peep[pos - 1] = chr;
  	strncpy(&peep[pos], temp, (peeplen - pos) + 1);
	//free(temp);
  }
  else
  {
	  printf("Something went wrong.\n");  
  }

}


/* helper function for getPeep and getPeepAtTime */
void deleteP(struct Modification mod, char * peep)
{
  unsigned short pos = mod.position;
  
  int peeplen = check_len(peep);
  
  if (pos > peeplen)
{
	printf("Tried to delete from %d\n", pos);
	printf("Something wrong here\n");
	return;
}
  char * temp = malloc(sizeof(char) * (peeplen - pos));

  // algorithm taken from delete function in peepEditor.c
  strncpy(temp, &peep[pos], peeplen - pos);
  strncpy(&peep[pos - 1], temp, peeplen - pos);
  peep[peeplen - 1]= '\0';
  free(temp);
}





/* parameters: pointer to a BlockChain
   return:     string (peep) */
char * getPeep (struct BlockChain * bc)
{
  int i = 0, j = 0;
  char * peep = malloc(256);
  memset(peep, '\0', 256);
  //printf("%d\n", check_len(peep));

  while (i < bc->size) // while not at end of block chain
    {
      for (j = 0; j < bc->chain[i].blockSize; j++) // for each transaction
	{
	  // if transaction event is an insert
	  if (bc->chain[i].transactions[j].mod.event_type == 0 && check_len(peep)<256)
	    {insertP(bc->chain[i].transactions[j].mod, peep);}
	  // if transaction event is a delete
	  else if (bc->chain[i].transactions[j].mod.event_type == 1)
	    {deleteP(bc->chain[i].transactions[j].mod, peep);}
	}
      i++;
    }

  return peep;
}


/* parameters: pointer to a BlockChain, timestamp
   return:     string (peep) */
char * getPeepAtTime (struct BlockChain * bc, time_t time)
{
  int i = 0, j = 0;
  char * peep = malloc(256);

  while (i < bc->size) // while not at end of block chain
    {
      // j is less than the number of transactions and the transaction's timestamp is less than
      // the given time
      for (j = 0; j < bc->chain[i].blockSize &&  bc->chain[i].transactions[j].timestamp <= time; j++)
	{
	  // if transaction event is an insert
	  if (bc->chain[i].transactions[j].mod.event_type == 0)
	    {insertP(bc->chain[i].transactions[j].mod, peep);}
	  // if transaction event is a delete
	  else
	    {deleteP(bc->chain[i].transactions[j].mod, peep);}
	}
      i++;
    }
  return peep; //Don't forget to free peep after displaying in main()
}



void printIndividualTransaction(struct Transaction t)
{
	if (t.mod.event_type) // event type is 1 so it was a deletion
	{
		printf("Letter %c was deleted at time: %lld from position %d\n", t.mod.character, (long long) t.timestamp, t.mod.position);
	}
	else 
	{
		printf("Letter %c was inserted at time: %lld to position %d\n", t.mod.character, (long long) t.timestamp, t.mod.position);
	}
	
	return;
}

char * hashOneTransaction(struct Transaction t)
{
	char * toReturn = malloc(32); // 32 bytes for return value
	struct Transaction * q = &t;
	EVP_MD_CTX * mdctx;
	mdctx = EVP_MD_CTX_create();
	unsigned int digest_len = EVP_MD_size(EVP_sha256());
	char * digest = (char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()));	
	
	EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
	EVP_DigestUpdate(mdctx, q, sizeof(struct Transaction));
	EVP_DigestFinal_ex(mdctx, (unsigned char *)digest, &digest_len);

	strncpy(toReturn, digest, 32); // Copy 32 bytes over

	return toReturn;
}



void destroyBlockChain (struct BlockChain * bc){
  free(bc->chain);
  free(bc);
  return;
}

