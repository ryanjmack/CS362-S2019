/**********************************************************************************
* Unit test for the Great Hall card implementation in dominion.c
* Sources: assignment 3 assistance (2) code, cardtest4.c provided code
* To run, makefile should include:
* testGreatHall: cardtest4.c dominion.o rngs.o
*     gcc -o testGreatHall -g cardtest4.c dominion.o rngs.o $(CFLAGS)
***********************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

// check all properties of the previous state against all properties of the current state
int statesSame(struct gameState prevState, struct gameState state)
{
    int k;
    int supplyCards[4] = {curse, estate, duchy, province};
    int statesSame = 1;

    if ((prevState.numPlayers != state.numPlayers) || (prevState.outpostTurn != state.outpostTurn) || (prevState.outpostPlayed != state.outpostPlayed) ||
        (prevState.phase != state.phase) || (prevState.numBuys != state.numBuys) || (prevState.coins != state.coins))
        statesSame = 0;

    for (k = 0; k < 4; k++)
    {
        if ((prevState.supplyCount[supplyCards[k]] != state.supplyCount[supplyCards[k]]) || (prevState.embargoTokens[k] != state.embargoTokens[k]))
            statesSame = 0;
    }

    return statesSame;
}

int main()
{
    // declare variables
    int seed = 1000;
    int numPlayers = 2;
    int cards[10] = {adventurer, outpost, salvager, village, council_room, great_hall, cutpurse,
           sea_hag, estate, smithy};
    struct gameState state, prevState;
    int i;
    int handPos = 0;
    int fails = 0;

    // initialize the game
    memset(&state, 23, sizeof(struct gameState));
    initializeGame(numPlayers, cards, seed, &state);

    printf("\n*********** Testing Play Great Hall Card ***********\n\n");

    // copy previous game state to check later
    memcpy(&prevState, &state, sizeof(struct gameState));

    // play the Great Hall card for both players
    for (i = 0; i < numPlayers; i++)
    {
        cardEffect(great_hall, 0, 0, 0, &state, handPos, 0);
        state.whoseTurn = 1;
    }

    // each player should have the same number of cards in their hand after play
    printf("Test 1: Each player should have same number of cards in hand after play\n");
    printf("Expected player 1 handCount: %d, Actual player 1 handCount: %d\n", prevState.handCount[0], state.handCount[0]);
    printf("Expected player 2 handCount: %d, Actual player 2 handCount: %d\n", prevState.handCount[1], state.handCount[1]);
    if ((prevState.handCount[0] == state.handCount[0]) && (prevState.handCount[1] == state.handCount[1]))
        printf("Test 1: PASS\n\n");
    else
    {
        printf("Test 1: FAIL\n\n");
        fails++;
    }

    // state.numActions should go up by 1 for each player
    printf("Test 2: +1 numActions for each player after play\n");
    printf("Expected state.numActions: %d, Actual state.numActions: %d\n", prevState.numActions + numPlayers, state.numActions);
    if (state.numActions == prevState.numActions + numPlayers)
        printf("Test 2: PASS\n\n");
    else
    {
        printf("Test 2: FAIL\n\n");
        fails++;
    }

    // after play, no other properties of state should be adjusted
    printf("Test 3: State not otherwise adjusted\n");
    if (statesSame(prevState, state))
        printf("Test 3: PASS\n\n");
    else
    {
        printf("Test 3: FAIL\n\n");
        fails++;
    }

    if (fails == 0)
        printf("Great Hall Card Function: All Tests Passed!\n\n");
    else
        printf("Great Hall Card Function: Failed %d Tests\n\n", fails);

    return 0;
}
