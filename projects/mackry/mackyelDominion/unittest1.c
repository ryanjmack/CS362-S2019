/**********************************************************************************
* Unit test for the playAdventurer refactored function in dominion.c
* Sources: assignment 3 assistance (2) code, cardtest4.c provided code
* To run, makefile should include:
* testPlayAdventurer: unittest1.c dominion.o rngs.o
*     gcc -o testPlayAdventurer -g unittest1.c dominion.o rngs.o $(CFLAGS)
***********************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

// positive unit test for playAdventurer function
int main()
{
    // declare variables
    int seed = 1000;
    int numPlayers = 2;
    int cards[10] = {adventurer, outpost, salvager, village, minion, mine, cutpurse,
           sea_hag, estate, smithy};
    struct gameState state, prevState;
    int deckSize = 10;
    int handSize = 5;
    int i, j, m, p, q, r, u, v, y;
    int numHandPre[2] = {0, 0};
    int numTreasurePre[2] = {0, 0};
    int numTreasurePost[2] = {0, 0};
    int flag = 0;
    int fails = 0;

    // initialize the game
    memset(&state, 23, sizeof(struct gameState));
    initializeGame(numPlayers, cards, seed, &state);

    printf("\n*********** Testing Adventurer Card Function ***********\n\n");

    // manually reset the deck to have 2 treasure only
    for (i = 0; i < numPlayers; i++)
    {
        state.deckCount[i] = 0;
        for (j = 0; j < deckSize - 8; j++)
        {
            state.deck[i][j] = gold;
            state.deckCount[i]++;
        }
        for (j = deckSize - 8; j < deckSize; j++)
        {
            state.deck[i][j] = estate;
            state.deckCount[i]++;
        }
    }

    // reset hand and then draw from new deck into hand for player 0
    state.handCount[0] = 0;
    for (m = 0; m < handSize; m++)
    {
        drawCard(0, &state);
    }

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
        playAdventurer(&state, r);
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
        printf("Adventurer Card Function: All Tests Passed!\n\n");
    else
        printf("Adventurer Card Function: Failed %d Tests\n\n", fails);

    return 0;
}
