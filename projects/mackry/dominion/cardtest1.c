/**********************************************************************************
* Unit test for the Adventurer card implementation in dominion.c
* Sources: assignment 3 assistance (2) code, cardtest4.c provided code
* To run, makefile should include:
* testAdventurer: cardtest1.c dominion.o rngs.o
*     gcc -o testAdventurer -g cardtest1.c dominion.o rngs.o $(CFLAGS)
***********************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

// boundary unit test for Adventurer card
int main()
{
    // declare variables
    int seed = 1000;
    int numPlayers = 2;
    int cards[10] = {adventurer, outpost, salvager, village, minion, mine, cutpurse,
           sea_hag, estate, smithy};
    struct gameState state, prevState;
    int p, q, r, u, v, y;
    int numHandPre[2] = {0, 0};
    int numTreasurePre[2] = {0, 0};
    int numTreasurePost[2] = {0, 0};
    int flag = 0;
    int fails = 0;

    // initialize the game
    memset(&state, 23, sizeof(struct gameState));
    initializeGame(numPlayers, cards, seed, &state);

    printf("\n*********** Testing Play Adventurer Card (Boundary) ***********\n\n");

    // get number of total cards in hand before play
    numHandPre[0] = state.handCount[0];
    numHandPre[1] = state.handCount[1];

    // get number of treasure cards in hand before play
    for (p = 0; p < numPlayers; p++)
    {
        for (q = 0; q < state.handCount[p]; q++)
        {
            if (state.hand[p][q] == copper || state.hand[p][q] == silver || state.hand[p][q] == gold)
            {
                numTreasurePre[p]++;
            }
        }
    }

    // play the Adventurer card for both players
    for (r = 0; r < numPlayers; r++)
    {
        cardEffect(adventurer, 0, 0, 0, &state, 0, 0);
        state.whoseTurn = 1;
    }

    // get number of treasure cards in hand after play
    for (u = 0; u < numPlayers; u++)
    {
        for (v = 0; v < state.handCount[u]; v++)
        {
            if (state.hand[u][v] == copper || state.hand[u][v] == silver || state.hand[u][v] == gold)
            {
                numTreasurePost[u]++;
            }
        }
    }

    // after play, hand should have increased by 2 cards overall
    printf("Test 1: +2 cards in hand for each player\n");
    printf("Expected player 1 hand count: %d, Actual player 1 hand count: %d\n", numHandPre[0] + 2, state.handCount[0]);
    printf("Expected player 2 hand count: %d, Actual player 2 hand count: %d\n\n", numHandPre[1] + 2, state.handCount[1]);
    if ((state.handCount[0] == numHandPre[0] + 2) && (state.handCount[1] == numHandPre[1] + 2))
        printf("Test 1: PASS\n\n");
    else
    {
        printf("Test 1: FAIL\n\n");
        fails++;
    }

    // after play, hand should have +2 treasure cards
    printf("Test 2: +2 treasure cards in hand for each player\n");
    printf("Expected player 1 treasure count: %d, Actual player 1 treasure count: %d\n", numTreasurePre[0] + 2, numTreasurePost[0]);
    printf("Expected player 2 treasure count: %d, Actual player 2 treasure count: %d\n\n", numTreasurePre[1] + 2, numTreasurePost[1]);
    if ((numTreasurePost[0] == numTreasurePre[0] + 2) && (numTreasurePost[1] == numTreasurePre[1] + 2))
        printf("Test 2: PASS\n\n");
    else
    {
        printf("Test 2: FAIL\n\n");
        fails++;
    }

    // after play, anything before treasure cards should be in discard pile
    printf("Test 3: All cards in deck before treasure cards now in discard pile\n");

    // loop through to check what should be discarded for each player
    for (y = 0; y < numPlayers; y++)
    {
        int deckPos = prevState.deckCount[y] - 1;
        int drawnTreasure = 0;
        int index = 0;
        int expectedDiscards[5];

        while (drawnTreasure < 2 && deckPos > 0)
        {
            int card = prevState.deck[y][deckPos];
            if (card == copper || card == silver || card == gold)
            {
                drawnTreasure++;
            }
            else
            {
                expectedDiscards[index] = card;
                index++;
            }

            deckPos--;
        }
        for (int z = 0; z < index; z++)
        {
            if (expectedDiscards[z] != state.discard[y][z])
            {
                flag++;
            }
        }
    }

    if (flag == 0)
        printf("Test 3: PASS\n\n");
    else
    {
        printf("Test 3: FAIL\n\n");
        fails++;
    }

    if (fails == 0)
        printf("Adventurer Card Function (Boundary): All Tests Passed!\n\n");
    else
        printf("Adventurer Card Function (Boundary): Failed %d Tests\n\n", fails);

    return 0;
}
