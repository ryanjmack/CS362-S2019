/**********************************************************************************
* Unit test for the playVillage refactored function in dominion.c
* Sources: assignment 3 assistance (2) code, cardtest4.c provided code
* To run, makefile should include:
* testPlayVillage: unittest4.c dominion.o rngs.o
*     gcc -o testPlayVillage -g unittest4.c dominion.o rngs.o $(CFLAGS)
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
        (prevState.whoseTurn != state.whoseTurn) || (prevState.phase != state.phase) || (prevState.coins != state.coins) || (prevState.numBuys != state.numBuys))
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

    printf("\n*********** Testing Village Card Function ***********\n\n");

    // copy previous game state to check later
    memcpy(&prevState, &state, sizeof(struct gameState));

    // should have drawn one card (deckCount should be -1 and handCount should be +1)

    // play the Village card for both players
    for (i = 0; i < numPlayers; i++)
    {
        playVillage(&state, i, 0);
        // if first player, after play, numActions should only have incremented by 2
        if (i == 0)
        {
            printf("Test 1: +2 numActions after player 1 plays card\n");
            printf("Expected state.numActions: %d, Actual state.numActions: %d\n\n", prevState.numActions + 2, state.numActions);
            if (state.numActions == prevState.numActions + 2)
                printf("Test 1: PASS\n\n");
            else
            {
                printf("Test 1: FAIL\n\n");
                fails++;
            }
        }
    }

    // after play, numActions state should be increased by 2 for each player
    printf("Test 2: +2 numActions after both players play card\n");
    printf("Expected state.numActions: %d, Actual state.numActions: %d\n\n", prevState.numActions + (numPlayers * 2), state.numActions);
    if (state.numActions == prevState.numActions + (numPlayers * 2))
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

    // after play, each player should have drawn one card from deck
    printf("Test 4: -1 card in each player's deck after playing Village card\n");
    printf("Expected deck count for player 1: %d, Actual deck count for player 1: %d\n", prevState.deckCount[0] - 1, state.deckCount[0]);
    printf("Expected deck count for player 2: %d, Actual deck count for player 2: %d\n", prevState.deckCount[1] - 1, state.deckCount[1]);
    if ((prevState.deckCount[0] - 1 == state.deckCount[0]) && (prevState.deckCount[1] - 1 == state.deckCount[1]))
        printf("Test 4: PASS\n\n");
    else
    {
        printf("Test 4: FAIL\n\n");
        fails++;
    }

    // after play, each player should have one fewer card in hand (including +1 card from drawing a card in playVillage)
    printf("Test 5: -1 card in each player's hand after playing Village card (including drawn card)\n");
    printf("Expected hand count for player 1: %d, Actual hand count for player 1: %d\n", prevState.handCount[0], state.handCount[0]);
    printf("Expected hand count for player 2: %d, Actual hand count for player 2: %d\n\n", prevState.handCount[1], state.handCount[1]);
    if ((prevState.handCount[0] == state.handCount[0]) && (prevState.handCount[1] == state.handCount[1]))
        printf("Test 5: PASS\n\n");
    else
    {
        printf("Test 5: FAIL\n\n");
        fails++;
    }

    if (fails == 0)
        printf("Village Card Function: All Tests Passed!\n\n");
    else
        printf("Village Card Function: Failed %d Tests\n\n", fails);

    return 0;
}
