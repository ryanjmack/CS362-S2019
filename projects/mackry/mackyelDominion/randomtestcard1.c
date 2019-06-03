/**********************************************************************************
* Random test for the Outpost card
* To run, makefile should include:
* randomTestCard1: randomtestcard1.c dominion.o rngs.o
*     gcc -o randomTestCard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)
***********************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rngs.h"

// check all properties of the previous state against all properties of the current state
int statesSame(struct gameState prevState, struct gameState state)
{
    int k;
    int supplyCards[4] = {curse, estate, duchy, province};
    int statesSame = 1;

    if ((prevState.numPlayers != state.numPlayers) || (prevState.outpostTurn != state.outpostTurn) || (prevState.phase != state.phase)
        || (prevState.numActions != state.numActions) || (prevState.coins != state.coins) || (prevState.numBuys != state.numBuys))
        statesSame = 0;

    for (k = 0; k < 4; k++)
    {
        if ((prevState.supplyCount[supplyCards[k]] != state.supplyCount[supplyCards[k]]) || (prevState.embargoTokens[k] != state.embargoTokens[k]))
            statesSame = 0;
    }

    return statesSame;
}

// random test for the Outpost card
int main()
{
    // declare variables
    int continueOn = 1;
    int checker = 0;
    int counter = 0;
    int seed = 1000;
    int cards[10] = {adventurer, outpost, salvager, village, minion, mine, cutpurse,
           sea_hag, estate, smithy};
    struct gameState state, prevState;
    int* numHandPre = malloc(sizeof(int) * 2);
    int* numTreasurePre = malloc(sizeof(int) * 2);
    int* numHandPost = malloc(sizeof(int) * 2);
    int* numTreasurePost = malloc(sizeof(int) * 2);
    int fail1 = 0, fail2 = 0, fail3 = 0;

    srand(time(NULL));

    printf("\n*********** Testing Outpost Card Function ***********\n\n");

    while(continueOn)
    {
        counter++;
        // randomize the number of players for the game, between 2 and 4
        int numPlayers = rand() % (5 - 2) + 2;

        // stop the game after we've played with all of the different numPlayer options
        // source: random test quiz code
        if (numPlayers == 2 && checker == 0)
            checker = 1;
        if (numPlayers == 3 && checker == 1)
            checker = 2;
        if (numPlayers == 4 && checker == 2)
            continueOn = 0;

        // initialize the game
        memset(&state, 23, sizeof(struct gameState));
        initializeGame(numPlayers, cards, seed, &state);

        // randomly set the initial value for outpostPlayed
        state.outpostPlayed = rand() % 101;

        // copy previous game state to check later
        memcpy(&prevState, &state, sizeof(struct gameState));

        // randomly generate the handPos value
        int handPos = rand() % 5;

        // play the Outpost card for all players
        for (int o = 0; o < numPlayers; o++)
        {
            cardEffect(outpost, 0, 0, 0, &state, handPos, 0);
            state.whoseTurn = o + 1;
        }

        // after play, outpostPlayed state should be increased by 1 for each player
        if (state.outpostPlayed != prevState.outpostPlayed + numPlayers)
            fail1++;

        // after play, no other properties of state should be adjusted
        if (!statesSame(prevState, state))
            fail2++;

        // after play, each player should have one fewer card in hand
        for (int p = 0; p < numPlayers; p++)
        {
            if (prevState.handCount[p] - 1 != state.handCount[p])
                fail3++;
        }

    }

    printf("Test 1: +numPlayers outpostPlayed after all players play card\n");
    printf("Test 1: FAILED %d TIMES\n", fail1);
    printf("Test 1: PASSED %d TIMES\n\n", counter - fail1);

    printf("Test 2: State not otherwise adjusted\n");
    printf("Test 2: FAILED %d TIMES\n", fail2);
    printf("Test 2: PASSED %d TIMES\n\n", counter - fail2);

    printf("Test 3: -1 card in each player's hand after playing Outpost card\n");
    printf("Test 3: FAILED %d TIMES\n", fail3);
    printf("Test 3: PASSED %d TIMES\n\n", counter - fail3);

    if (fail1 == 0 && fail2 == 0 && fail3 == 0)
        printf("Outpost Card Function: All Tests Passed!\n\n");
    else
        printf("Outpost Card Function: Failed %d Tests\n\n", fail1 + fail2 + fail3);

    free(numTreasurePre);
    free(numTreasurePost);
    free(numHandPre);
    free(numHandPost);
    return 0;
}
