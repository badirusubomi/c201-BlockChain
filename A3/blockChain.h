# ifndef BLOCKCHAIN
# define BLOCKCHAIN
// The maximum number of transactions allowed in one block.
#define MXTXNUM 64
#define PEEPLEN 256
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/evp.h>
//#include "peepEditor.h"

/*

DATA STRUCTURES

Structure to Represent a modification [2 bytes]. (modification)
position- Position in the peep that the event occured: Maximum position of 256
character- The character that was modified  - ASCII codes 32 to 127
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
blockNum- Incremental number of the block.                     [4 bytes]
blockSize- Size of transaction elements in the block            [1 byte]
blockHash- The Merkle tree hash of the transactions             [7 bytes]
                                
                                
headHash- Hash of the head elements above                      [7 bytes]
      
transactions- Transactions contained in the block (maximum of 64 transactions)

NOTE: a hash of length 7 bytes is obtained by taking the last 7 bytes of the sha256 hash 
      The truncation is to be applied only as the final step. 
      All intermediary hashes are full sha256 hashes

*/

struct Modification{
  unsigned short position: 8;
  unsigned short character: 7;
  unsigned short event_type: 1;
} Modification;

struct Transaction{
  time_t timestamp;
  struct Modification mod;
} Transaction;

struct Block{
  time_t timeStamp;
  char previousHash[7];
  unsigned int blockNum: 4;
  unsigned char blockSize;
  char blockHash[7];
  char headHash[7];
  struct Transaction transactions[64];
} Block;

struct BlockChain{
  int size;
  struct Block * chain;
  
} BlockChain;

/*FUNCTIONS

writeBlockChain - writes a block chain to a file

readBlockChain - reads a block chain from a file 

verifyBlockChain - verifies the integrity of the block chain

addTransactions - adds a new session of transactions at the end of the block chain starting with a new block

printTransactions - prints all transactions contained in the blockchain

getPeep - produces the current peep by replaying all the transactions contained in the block chain

getPeepAtTime - produces the peep at a certain time stamp by replaying all the transactions in the block chain up to (and including) the time stamp

*/

/* parameters: name of file, pointer to a BlockChain
   return: int to indicate successful write */
int writeBlockChain (const char * filename, struct BlockChain *bc);

/* parameters: name of file
   return:     pointer to a BlockChain */
struct BlockChain *readBlockChain (const char * filename);

/* parameters: pointer to a BlockChain
   return:     int to indicate successful verification */
int verifyBlockChain (struct BlockChain * bc);

/* parameters: pointer to a BlockChain, pointer to a Transaction,
               int to indicate number of transactions
           return:     none
   Loops through transactions in trx and creates a block to add them to chain
           */
void addTransactions (struct BlockChain * bc, struct Transaction * trx, int n);

/* parameters: pointer to a BlockChain
   return:     none */
void printTransactions (struct BlockChain * bc);

/* parameters: pointer to a BlockChain
   return:     string (peep) */
char * getPeep (struct BlockChain * bc);

/* parameters: pointer to a BlockChain, timestamp
   return:     string (peep) */
char * getPeepAtTime (struct BlockChain * bc, time_t time);

// Recursively generated n/2 hashes until there is a single hash
char * getMerkleRoot(struct Transaction * trx, unsigned int n);

/*  Recursive function to generate merkle

Parameters int n - number of hashes in hashes array
*/
char * recursiveMerkle(char ** hashes, int n);

/*  
print a single transation

parameter: struct transaction
no side effects
*/
void printIndividualTransaction(struct Transaction t);

/*  */
char * hashOneTransaction(struct Transaction t);

/* Frees all memories

Parameters BlockChain struct poitner
 */
void destroyBlockChain (struct BlockChain * bc);

/* helper function for getPeep and getPeepAtTime */
void deleteP(struct Modification mod, char * peep);

/* helper function for getPeep and getPeepAtTime */
void insertP(struct Modification mod, char * peep);

# endif
