
/* Names: Olasubomi Badiru - 3101002 || Alex Lyndon 3111431 || Collette Patalinghog 3110938
 * Lab section: CMPT-201-X01L(1) and CMPT-201-X03L
 * assignment: peep.c
 * last worked on: April 5th
 * due date: April 5th
*/


#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "blockChain.h"
#include "peepEditor.h"

/*
- alex will write editor() and insert() helper function
- Subomi will write delete() helper function
- Collette will write displayMenu() and displayPeep()
*/


//possible clobal:
//char peep[256];


int main(int argc, char **argv){

  int c, tr_no;
  
  //int cflag = 0;
  //tflag = 0, vflag = 0, hflag = 0, eflag = 0, pflag = 0;
  //each flag is set to 1 if respective option encountered

  char *cfile;//to save argument following i option
  //char *ifile;//to save argument following o option
  struct Transaction *modBuffer = malloc(sizeof(struct Transaction) * 10);
  struct BlockChain *bc = malloc(sizeof(struct BlockChain)*1);;
  struct Transaction *modBuffer2;
  char *peep;   //declared outside getopt while loop to avoid redclaration
  /* 
    peep -c [fileName] – to create (by invoking the editor) a new peep
    peep -e [fileName] – to edit an existing peep
    peep -v [fileName] – to verify the integrity of an existing peep
    peep -h [fileName] – to print the transaction history of an existing peep
    peep -p [fileName] -t [timestamp] –to reproduce a peep at a given timestamp
    peep -p [fileName] –to reproduce the current peep 
  */
  while ((c=getopt(argc,argv,":c:e:v:h:p:t")) !=-1){
    //colon after option indicates that option should be followed by argument
    //colon before first option to distinguish between missing argument and wrong option
    switch (c)
      {
      case 'c':     //creates a blcok chain; calls editor; adds transactions to blockChain; prnits Transaction
	//	cflag=1;
	printf("\nCreating Peep and Ledger::\n");
	memset(peep, '\0',256); 
	cfile=optarg;	
	tr_no = editor(peep,modBuffer); //editor() write the transactions to a file after session
	//free(modBuffer);
	tr_no = readTrans(&modBuffer2, "transactionOut"); //no need to malloc; memory allocation occurs in readtrans()
	if(tr_no > 0){
	  printf("%d transactions were made in the editing session\n", tr_no);
	  addTransactions(bc, modBuffer2, tr_no);
	  writeBlockChain(cfile,bc);
	  printTransactions(bc);
	}
	
	break;
      case 'e':   // I believe this optiions reads from a blockChain from a file and adds the transactions
	          // from the editor to the blockChain
	//assumes peep already exists
	printf("\nEditing begins: \n");
	//eflag = 1;
	cfile = optarg;
	bc = readBlockChain(cfile);
	peep = getPeep(bc);
	tr_no = editor(peep, modBuffer);
	//free(modBuffer);
	tr_no = readTrans(&modBuffer2,"transactionOut");
        addTransactions(bc, modBuffer2, tr_no);
	writeBlockChain(cfile, bc);
	break;
	
      case 'v': //missing argument
	//vflag = 1;
	cfile = optarg;
	bc = readBlockChain(cfile);
	if (!(verifyBlockChain(bc))){
	  printf("\nBlockChain is vald and has been Verified.\n");
	}
	else printf("\nBlockChain is invalid. Reevaluate...\n");
	break;
	
      case 'h':
	//hflag=1;
	cfile = optarg;
	bc = readBlockChain(cfile);
	printf("\nTransactions in BlockChain:-\n");
	printTransactions(bc);
	break;
	
      case 'p':
	cfile = optarg;
	bc = readBlockChain(cfile);

	c = getopt(argc,argv,":t:");
	
	if(c == 't'){
	  cfile = optarg;
	  //tflag = 1;
	  long int  timestamp = (long int) atoi(optarg);
	  peep = getPeepAtTime(bc, timestamp);
	  
	  printf("\nPeep at time(%ld):",timestamp);
	  printPeep(peep); //print the peep

	  //How will this work, will the function return a pointer to dynamic memory??
	  free(peep);
	}
	else{
	  peep = getPeep(bc);
	  printTransactions(bc);

	  printf("\nPeep: ");
	  
	  printPeep(peep);
	  free(peep);
	  }
	//pflag=1;
	break;
      case ':': //missing argument
	fprintf(stderr, "%s: option '-%c' requires an argument\n",argv[0], optopt);
	//printUsage();
	//	exit(ARG_ERROR);
	break;
      case '?': //incorrect option
	fprintf(stderr, "%s: option '-%c' is invalid: ignored\n",argv[0], optopt);
	//printUsage();
	//exit(ARG_ERROR);
	break;

      }


  }
  

  return 0;

}
