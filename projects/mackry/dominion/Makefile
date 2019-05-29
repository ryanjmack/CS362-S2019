CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

testPlayAdventurer: unittest1.c dominion.o rngs.o
	gcc -o testPlayAdventurer -g unittest1.c dominion.o rngs.o $(CFLAGS)

testPlayOutpost: unittest2.c dominion.o rngs.o
	gcc -o testPlayOutpost -g unittest2.c dominion.o rngs.o $(CFLAGS)

testPlaySalvager: unittest3.c dominion.o rngs.o
	gcc -o testPlaySalvager -g unittest3.c dominion.o rngs.o $(CFLAGS)

testPlayVillage: unittest4.c dominion.o rngs.o
	gcc -o testPlayVillage -g unittest4.c dominion.o rngs.o $(CFLAGS)

testAdventurer: cardtest1.c dominion.o rngs.o
	gcc -o testAdventurer -g cardtest1.c dominion.o rngs.o $(CFLAGS)

testSmithy: cardtest2.c dominion.o rngs.o
	gcc -o testSmithy -g cardtest2.c dominion.o rngs.o $(CFLAGS)

testCouncilRoom: cardtest3.c dominion.o rngs.o
	gcc -o testCouncilRoom -g cardtest3.c dominion.o rngs.o $(CFLAGS)

testGreatHall: cardtest4.c dominion.o rngs.o
	gcc -o testGreatHall -g cardtest4.c dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

unittestresults: testPlayAdventurer testPlayOutpost testPlaySalvager testPlayVillage testAdventurer testSmithy testCouncilRoom testGreatHall
	./testPlayAdventurer &> unittestresults.out
	gcov dominion.c >> unittestresults.out
	./testPlayOutpost >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	./testPlaySalvager >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	./testPlayVillage >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	./testAdventurer >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	./testSmithy >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	./testCouncilRoom >> unittestresults.out
	gcov dominion.c >> unittestresults.out
	./testGreatHall >> unittestresults.out
	gcov dominion.c >> unittestresults.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player testPlayAdventurer testPlayOutpost testPlaySalvager testPlayVillage testAdventurer testSmithy testCouncilRoom testGreatHall

clean:
	rm -f *.o playdom.exe playdom player player.exe testPlayAdventurer testPlayOutpost testPlaySalvager testPlayVillage testAdventurer testSmithy testCouncilRoom testGreatHall *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
