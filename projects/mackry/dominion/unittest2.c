/**********************************************************************************
* Unit test for the playOutpost refactored function in dominion.c
* Sources: assignment 3 assistance (2) code, cardtest4.c provided code
* To run, makefile should include:
* testPlayOutpost: unittest2.c dominion.o rngs.o
*     gcc -o testPlayOutpost -g unittest2.c dominion.o rngs.o $(CFLAGS)
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

    if ((prevState.numPlayers != state.numPlayers) || (prevState.outpostTurn != state.outpostTurn) || (prevState.whoseTurn != state.whoseTurn) ||
        (prevState.phase != state.phase) || (prevState.numActions != state.numActions) || (prevState.coins != state.coins) || (prevState.numBuys != state.numBuys))
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
    int cards[10] = {adventurer, outpost, salvager, village, minion, mine, cutpurse,
           sea_hag, estate, smithy};
    struct gameState state, prevState;
    int i;
    int fails = 0;

    // initialize the game
    memset(&state, 23, sizeof(struct gameState));
    initializeGame(numPlayers, cards, seed, &state);

    printf("\n*********** Testing Outpost Card Function ***********\n\n");

    // copy previous game state to check later
    memcpy(&prevState, &state, sizeof(struct gameState));

    // play the Outpost card for both players
    for (i = 0; i < numPlayers; i++)
    {
        playOutpost(&state, i, 0);
        // if first player, after play, outpostPlayed should only have incremented by 1
        if (i == 0)
        {
            printf("Test 1: +1 outpostPlayed after player 1 plays card\n");
            printf("Expected state.outpostPlayed: %d, Actual state.outpostPlayed: %d\n\n", prevState.outpostPlayed + 1, state.outpostPlayed);
            if (state.outpostPlayed == prevState.outpostPlayed + 1)
                printf("Test 1: PASS\n\n");
            else
            {
                printf("Test 1: FAIL\n\n");
                fails++;
            }
        }
    }

    // after play, outpostPlayed state should be increased by 1 for each player
    printf("Test 2: +2 outpostPlayed after both players play card\n");
    printf("Expected state.outpostPlayed: %d, Actual state.outpostPlayed: %d\n\n", prevState.outpostPlayed + numPlayers, state.outpostPlayed);
    if (state.outpostPlayed == prevState.outpostPlayed + numPlayers)
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

    // after play, each player should have one fewer card in hand
    printf("Test 4: -1 card in each player's hand after playing Outpost card\n");
    printf("Expected hand count for player 1: %d, Actual hand count for player 1: %d\n", prevState.handCount[0] - 1, state.handCount[0]);
    printf("Expected hand count for player 2: %d, Actual hand count for player 2: %d\n\n", prevState.handCount[1] - 1, state.handCount[1]);
    if ((prevState.handCount[0] - 1 == state.handCount[0]) && (prevState.handCount[1] - 1 == state.handCount[1]))
        printf("Test 4: PASS\n\n");
    else
    {
        printf("Test 4: FAIL\n\n");
        fails++;
    }

    if (fails == 0)
        printf("Outpost Card Function: All Tests Passed!\n\n");
    else
        printf("Outpost Card Function: Failed %d Tests\n\n", fails);

    return 0;
}
