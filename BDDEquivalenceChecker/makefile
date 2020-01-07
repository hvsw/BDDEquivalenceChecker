CC=g++
SRCPATH=./
OBJPATH= include/gerentebdd/


bdd: main.o gerentebdd.o mainTesteBdds.o
	$(CC) main.o $(OBJPATH)gerentebdd.o $(OBJPATH)mainTesteBdds.o -o bdd 

main.o: main.cpp
	$(CC) -c main.cpp -o main.o

gerentebdd.o: $(OBJPATH)gerentebdd.cpp
	$(CC) -c $(OBJPATH)gerentebdd.cpp -o $(OBJPATH)gerentebdd.o

mainTesteBdds.o: $(OBJPATH)mainTesteBdds.cpp
	$(CC) -c $(OBJPATH)mainTesteBdds.cpp -o $(OBJPATH)mainTesteBdds.o

clean:
	rm -f *.o bdd
	rm -f $(OBJPATH)*.o bdd
