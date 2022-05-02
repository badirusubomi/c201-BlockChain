# Names: Olasubomi Badiru - 3101002 || Alex Lyndon 3111431 || Collette Patalinghog 3110938
# * Lab section: CMPT-201-X01L(1) and CMPT-201-X03L
# * assignment: A3Plan.txt
# * last worked on: March 17th
# * due date: April 5th


CC=gcc
CFLAGS = -Wall -ansi -g -lm -std=c99



blockChain.o: blockChain.c
	$(CC) $(CFLAGS)  -lssl -lcrypto  $^ -c

peepEditor.o: peepEditor.c
	$(CC) $(CFLAGS) $^ -c

peep.o: peep.c peepEditor.h blockChain.h
	$(CC) $(CFLAGS) peep.c -c

peep: peep.o peepEditor.o blockChain.o
	$(CC) $(CFLAGS) -lssl -lcrypto $^ -o $@


TestWrite: TestWrite.o peepEditor.o blockChain.o
	$(CC) $(CFLAGS) -lssl -lcrypto $^ -o $@

TestWrite.o: ./Tests/TestWrite.c
	$(CC) $(CFLAGS) $^ -c

TestVerify: TestVerify.o peepEditor.o blockChain.o
	$(CC) $(CFLAGS) -lssl -lcrypto $^ -o $@

TestVerify.o: ./Tests/TestVerify.c
	$(CC) $(CFLAGS) $^ -c


TestRead: TestRead.o peepEditor.o blockChain.o
	$(CC) $(CFLAGS) -lssl -lcrypto $^ -o $@

TestRead.o: ./Tests/TestRead.c
	$(CC) $(CFLAGS) $^ -c

TestAdd: TestAdd.o peepEditor.o blockChain.o
	$(CC) $(CFLAGS) -lssl -lcrypto $^ -o $@

TestAdd.o: ./Tests/TestAdd.c
	$(CC) $(CFLAGS) $^ -c

TestGetPeep: TestGetPeep.o peepEditor.o blockChain.o
	$(CC) $(CFLAGS) -lssl -lcrypto $^ -o $@

TestGetPeep.o: ./Tests/TestGetPeep.c
	$(CC) $(CFLAGS) $^ -c


.PHONY: tar clean A3.tar.gz testWrite testAdd testRead testGetPeep


testWrite: TestWrite
	./$^ ./binaryTestFiles/BlockChainOut ./binaryTestFiles/transactionOut 


testVerify: TestVerify
	./$^ ./binaryTestFiles/transactionOut ./binaryTestFiles/BlockChainOut

testAdd: TestAdd
	./$^ ./binaryTestFiles/BlockChainOut  ./binaryTestFiles/transactionOut 

testRead: TestRead
	./$^ ./binaryTestFiles/BlockChainOut ./binaryTestFiles/transactionOut

testGetPeep: TestGetPeep
	./$^ ./binaryTestFiles/BlockChainOut ./binaryTestFiles/transactionOut

A3.tar.gz: *.c *.h makefile ./Tests/* binaryTestFiles *.txt
	 tar -czvf A3.tar.gz ../ A3

tar: *.c *.h makefile ./Tests/* binaryTestFiles *.txt
	tar -czvf A3.tar.gz ../A3

all: peepEditor.o blockChain.o peep TestRead TestWrite TestAdd TestGetPeep TestVerify

clean:
	rm peep *.o TestAdd TestGetPeep TestRead TestWrite TestVerify
