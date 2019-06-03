/**********************************************************************************
* Unit test for the Smithy card implementation in dominion.c
* Sources: assignment 3 assistance (2) code, cardtest4.c provided code
* To run, makefile should include:
* testSmithy: cardtest2.c dominion.o rngs.o
*     gcc -o testSmithy -g cardtest2.c dominion.o rngs.o $(CFLAGS)
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
    int i, j, k, l, m;
    int handPos = 0;
    int fails = 0;
    int flag = 0;

    // initialize the game
    memset(&state, 23, sizeof(struct gameState));
    initializeGame(numPlayers, cards, seed, &state);

    printf("\n*********** Testing Play Smithy Card ***********\n\n");

    // copy previous game state to check later
    memcpy(&prevState, &state, sizeof(struct gameState));

    // play the Smithy card for both players
    for (i = 0; i < numPlayers; i++)
    {
        cardEffect(smithy, 0, 0, 0, &state, handPos, 0);
        state.whoseTurn = 1;
    }

    // hand count should have incremented by 2 (+3 - 1 discard) for each player
    printf("Test 1: +2 cards in hand after both players play card\n");
    for (j = 0; j < numPlayers; j++)
    {
        printf("Expected player %d hand count: %d, Actual player %d hand count: %d\n", j + 1, prevState.handCount[j] + 2, j + 1, state.handCount[j]);
        if (state.handCount[j] != prevState.handCount[j] + 2)
            flag++;
    }

    if (flag == 0)
        printf("\nTest 1: PASS\n\n");
    else
    {
        printf("Test 1: FAIL\n\n");
        fails++;
        flag = 0;
    }

    // deck count should have decremented by 3 for each player
    printf("Test 2: -3 cards in deck after both players play card\n");
    for (m = 0; m < numPlayers; m++)
    {
        printf("Expected player %d deck count: %d, Actual player %d deck count: %d\n", m + 1, prevState.deckCount[m] - 3, m + 1, state.deckCount[m]);
        if (state.deckCount[m] != prevState.deckCount[m] - 3)
            flag++;
    }

    if (flag == 0)
        printf("\nTest 2: PASS\n\n");
    else
    {
        printf("Test 2: FAIL\n\n");
        fails++;
        flag = 0;
    }

    // after play, additional cards in players' hands should be from their deck
    printf("Test 3: new cards in hand should be from each player's deck\n");
    for (k = 0; k < numPlayers; k++)
    {
        int deckIndex = prevState.deckCount[k] - 2;    // -2 rather than -1 to account for discarded card
        int handIndex = prevState.handCount[k];

        for (l = 0; l < 2; l++)
        {
            int cardPrev = prevState.deck[k][deckIndex];
            int cardPost = state.hand[k][handIndex];

            if (cardPrev != cardPost)
            {
                flag++;
            }
            deckIndex--;
            handIndex++;
        }
    }

    if (flag == 0)
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
        printf("Smithy Card Function: All Tests Passed!\n\n");
    else
        printf("Smithy Card Function: Failed %d Tests\n\n", fails);

    return 0;
}
