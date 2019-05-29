/**********************************************************************************
* Unit test for the playSalvager refactored function in dominion.c
* Sources: assignment 3 assistance (2) code, cardtest4.c provided code
* To run, makefile should include:
* testPlaySalvager: unittest3.c dominion.o rngs.o
*     gcc -o testPlaySalvager -g unittest3.c dominion.o rngs.o $(CFLAGS)
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
        (prevState.whoseTurn != state.whoseTurn) || (prevState.phase != state.phase) || (prevState.numActions != state.numActions))
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
    int numTests = 20;
    int cards[10] = {adventurer, outpost, salvager, village, minion, mine, cutpurse,
           sea_hag, estate, smithy};
    struct gameState state, prevState;
    int handPos = 0;
    int i;
    int card;
    int cardCost;
    int test1Pass = 0, test2Pass = 0, test3Pass = 0, test4Pass = 0, test5Pass = 0, test6Pass = 0;
    int fails = 0;

    srand(seed);

    printf("\n*********** Testing Salvager Card Function ***********\n\n");

    for (i = 0; i < numTests; i++)
    {
        int cardPos = rand() % 5;

        // initialize the game
        memset(&state, 23, sizeof(struct gameState));
        initializeGame(numPlayers, cards, seed, &state);

        // copy previous game state to check later
        memcpy(&prevState, &state, sizeof(struct gameState));

        // play the Salvager card for player 1, using a randomly assigned card for each iteration
        card = state.hand[0][cardPos];
        cardCost = getCost( handCard(card, &state) );
        playSalvager(&state, 0, handPos, card);

        // numBuys should go up by 1
        if (state.numBuys == prevState.numBuys + 1)
            test1Pass++;

        // coins should go up by cost of card played
        if (state.coins == prevState.coins + cardCost)
            test2Pass++;

        // hand should be 2 fewer than before
        if (state.handCount[0] == prevState.handCount[0] - 2)
            test3Pass++;

        // nothing else in the state should change besides what was already tested for
        if (statesSame(prevState, state))
            test4Pass++;

        // hand should no longer have the chosen coin card
        if (state.hand[0][state.handCount[0] + 1] == -1)
            test5Pass++;

        // hand should no longer have the handPos card
        if (state.hand[0][state.handCount[0]] == -1)
            test6Pass++;
    }

    // check that all tests passed
    printf("Test 1: +1 numBuys after player 1 plays card\n");
    printf("Expected pass number: %d, Actual pass number: %d\n", numTests, test1Pass);
    if (numTests == test1Pass)
        printf("Test 1: PASS\n\n");
    else
    {
        printf("Test 1: FAIL\n\n");
        fails++;
    }

    printf("Test 2: Coins increment by cost of chosen card\n");
    printf("Expected pass number: %d, Actual pass number: %d\n", numTests, test2Pass);
    if (numTests == test2Pass)
        printf("Test 2: PASS\n\n");
    else
    {
        printf("Test 2: FAIL\n\n");
        fails++;
    }

    printf("Test 3: -2 in hand after player 1 plays card\n");
    printf("Expected pass number: %d, Actual pass number: %d\n", numTests, test3Pass);
    if (numTests == test3Pass)
        printf("Test 3: PASS\n\n");
    else
    {
        printf("Test 3: FAIL\n\n");
        fails++;
    }

    printf("Test 4: State not otherwise adjusted\n");
    printf("Expected pass number: %d, Actual pass number: %d\n", numTests, test4Pass);
    if (numTests == test4Pass)
        printf("Test 4: PASS\n\n");
    else
    {
        printf("Test 4: FAIL\n\n");
        fails++;
    }

    printf("Test 5: Hand no longer has chosen coin card\n");
    printf("Expected pass number: %d, Actual pass number: %d\n", numTests, test5Pass);
    if (numTests == test5Pass)
        printf("Test 5: PASS\n\n");
    else
    {
        printf("Test 5: FAIL\n\n");
        fails++;
    }

    printf("Test 6: Hand no longer has played card\n");
    printf("Expected pass number: %d, Actual pass number: %d\n", numTests, test6Pass);
    if (numTests == test6Pass)
        printf("Test 6: PASS\n\n");
    else
    {
        printf("Test 6: FAIL\n\n");
        fails++;
    }

    if (fails == 0)
        printf("Salvager Card Function: All Tests Passed!\n\n");
    else
        printf("Salvager Card Function: Failed %d Tests\n\n", fails);

    return 0;
}
