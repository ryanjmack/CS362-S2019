/**********************************************************************************
* Unit test for the Council Room card implementation in dominion.c
* Sources: assignment 3 assistance (2) code, cardtest4.c provided code
* To run, makefile should include:
* testCouncilRoom: cardtest3.c dominion.o rngs.o
*     gcc -o testCouncilRoom -g cardtest3.c dominion.o rngs.o $(CFLAGS)
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
        (prevState.phase != state.phase) || (prevState.numActions != state.numActions) || (prevState.coins != state.coins))
        statesSame = 0;

    for (k = 0; k < 4; k++)
    {
        if ((prevState.supplyCount[supplyCards[k]] != state.supplyCount[supplyCards[k]]) || (prevState.embargoTokens[k] != state.embargoTokens[k]))
            statesSame = 0;
    }

    return statesSame;
}

// check that cards are correct for given player
int badCards(struct gameState prevState, struct gameState state, int currentPlayer)
{
    int badCards = 0;

    // after first player's turn, they should have +4 -1 discard cards in their hand, and second player should have +1 card
    if (currentPlayer == 0)
    {
        printf("Test 1: +4 -1 discard cards in player 1's hand, +1 card in player 2's hand after player 1 plays\n");
        printf("Expected player %d hand count: %d, Actual player %d hand count: %d\n", currentPlayer + 1, prevState.handCount[currentPlayer] + 3, currentPlayer + 1, state.handCount[currentPlayer]);
        printf("Expected player %d hand count: %d, Actual player %d hand count: %d\n", currentPlayer + 2, prevState.handCount[currentPlayer + 1] + 1, currentPlayer + 2, state.handCount[currentPlayer + 1]);
        if (state.handCount[currentPlayer] != prevState.handCount[currentPlayer] + 3)
            badCards = 1;
        else if (state.handCount[currentPlayer + 1] != prevState.handCount[currentPlayer + 1] + 1)
            badCards = 1;
    }

    // after second player's turn, they should have +5 -1 discard cards in their hand and first player should have +4 -1 +1
    else if (currentPlayer == 1)
    {
        printf("Test 2: +4 -1 discard cards in player 2's hand, +1 card in player 1's hand after player 2 plays\n");
        printf("Expected player %d hand count: %d, Actual player %d hand count: %d\n", currentPlayer + 1, prevState.handCount[currentPlayer] + 4, currentPlayer + 1, state.handCount[currentPlayer]);
        printf("Expected player %d hand count: %d, Actual player %d hand count: %d\n", currentPlayer, prevState.handCount[currentPlayer - 1] + 4, currentPlayer, state.handCount[currentPlayer - 1]);
        if (state.handCount[currentPlayer] != prevState.handCount[currentPlayer] + 4)
            badCards = 1;
        else if (state.handCount[currentPlayer - 1] != prevState.handCount[currentPlayer - 1] + 4)
            badCards = 1;
    }

    return badCards;
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

    printf("\n*********** Testing Play Council Room Card ***********\n\n");

    // copy previous game state to check later
    memcpy(&prevState, &state, sizeof(struct gameState));

    // play the Council Room card for both players
    for (i = 0; i < numPlayers; i++)
    {
        cardEffect(council_room, 0, 0, 0, &state, handPos, 0);

        if (!badCards(prevState, state, i))
            printf("Test %d: PASS\n\n", i + 1);
        else
        {
            printf("Test %d: FAIL\n\n", i + 1);
            fails++;
        }

        state.whoseTurn = 1;
    }

    // state.numBuys should go up by 1 for each player
    printf("Test 3: +1 numBuys for each player after play\n");
    printf("Expected state.numBuys: %d, Actual state.numBuys: %d\n", prevState.numBuys + numPlayers, state.numBuys);
    if (state.numBuys == prevState.numBuys + numPlayers)
        printf("Test 3: PASS\n\n");
    else
    {
        printf("Test 3: FAIL\n\n");
        fails++;
    }

    // after play, no other properties of state should be adjusted
    printf("Test 4: State not otherwise adjusted\n");
    if (statesSame(prevState, state))
        printf("Test 4: PASS\n\n");
    else
    {
        printf("Test 4: FAIL\n\n");
        fails++;
    }

    if (fails == 0)
        printf("Council Room Card Function: All Tests Passed!\n\n");
    else
        printf("Council Room Card Function: Failed %d Tests\n\n", fails);

    return 0;
}
