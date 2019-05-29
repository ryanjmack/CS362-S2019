/**********************************************************************************
* Random test for the Adventurer card
* To run, makefile should include:
* randomTestAdventurer: randomtestadventurer.c dominion.o rngs.o
*     gcc -o randomTestAdventurer -g randomtestadventurer.c dominion.o rngs.o $(CFLAGS)
***********************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rngs.h"

void getNumTreasure(int numPlayers, struct gameState state, int* numTreasure)
{
    for (int i = 0; i < numPlayers; i++)
    {
        numTreasure[i] = 0;
        for (int j = 0; j < state.handCount[i]; j++)
        {
            if (state.hand[i][j] == copper || state.hand[i][j] == silver || state.hand[i][j] == gold)
            {
                numTreasure[i]++;
            }
        }
    }
}

void getNumCards(int numPlayers, struct gameState state, int* numHand)
{
    for (int i = 0; i < numPlayers; i++)
    {
        numHand[i] = state.handCount[i];
    }
}

// random test for the Adventurer card
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
    int flag1 = 0, flag2 = 0, flag3 = 0, fail1 = 0, fail2 = 0, fail3 = 0;

    srand(time(NULL));

    printf("\n*********** Testing Adventurer Card Function ***********\n\n");

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

        // randomize the number of treasure cards in deck for each player
        for (int i = 0; i < numPlayers; i++)
        {
            int j;
            int numTreasures = rand() % (11 - 2) + 2;
            state.deckCount[i] = 0;
            for (j = 0; j < numTreasures; j++)
            {
                state.deck[i][j] = gold;
                state.deckCount[i]++;
            }
            for (j = numTreasures; j < 10; j++)
            {
                state.deck[i][j] = estate;
                state.deckCount[i]++;
            }
        }

        // reset hand and then draw from new deck into hand for player 0
        state.handCount[0] = 0;
        for (int k = 0; k < 5; k++)
        {
            drawCard(0, &state);
        }

        // get number of total cards and treasure cards in hand before play
        getNumCards(numPlayers, state, numHandPre);
        getNumTreasure(numPlayers, state, numTreasurePre);

        // play the Adventurer card for all players
        for (int o = 0; o < numPlayers; o++)
        {
            cardEffect(adventurer, 0, 0, 0, &state, 0, 0);
            state.whoseTurn = o + 1;
        }

        // get number of total cards and treasure cards in hand after play
        getNumCards(numPlayers, state, numHandPost);
        getNumTreasure(numPlayers, state, numTreasurePost);

        // after play, hand should have increased by 2 cards overall for both players
        for (int s = 0; s < numPlayers; s++)
        {
            if (numHandPost[s] != numHandPre[s] + 2)
                flag1 = 1;
        }

        if (flag1)
            fail1++;

        // after play, hand should have +2 treasure cards
        for (int t = 0; t < numPlayers; t++)
        {
            if (numTreasurePost[t] != numTreasurePre[t] + 2)
                flag2 = 1;
        }

        if (flag2)
            fail2++;

        // after play, anything before treasure cards should be in discard pile
        // loop through to check what should be discarded for each player
        for (int o = 0; o < numPlayers; o++)
        {
            int deckPos = prevState.deckCount[o] - 1;
            int drawnTreasure = 0;
            int index = 0;
            int expectedDiscards[10];

            while (drawnTreasure < 2 && deckPos > 0)
            {
                int card = prevState.deck[o][deckPos];
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
            for (int p = 0; p < index; p++)
            {
                if (expectedDiscards[p] != state.discard[o][p])
                {
                    flag3 = 1;
                }
            }
        }

        if (flag3)
            fail3++;
    }

    printf("Test 1: +2 cards in hand for each player\n");
    printf("Test 1: FAILED %d TIMES\n", fail1);
    printf("Test 1: PASSED %d TIMES\n\n", counter - fail1);

    printf("Test 2: +2 treasure cards in hand for each player\n");
    printf("Test 2: FAILED %d TIMES\n", fail2);
    printf("Test 2: PASSED %d TIMES\n\n", counter - fail2);

    printf("Test 3: All cards in deck before treasure cards now in discard pile\n");
    printf("Test 3: FAILED %d TIMES\n", fail3);
    printf("Test 3: PASSED %d TIMES\n\n", counter - fail3);

    if (fail1 == 0 && fail2 == 0 && fail3 == 0)
        printf("Adventurer Card Function: All Tests Passed!\n\n");
    else
        printf("Adventurer Card Function: Failed %d Tests\n\n", fail1 + fail2 + fail3);

    free(numTreasurePre);
    free(numTreasurePost);
    free(numHandPre);
    free(numHandPost);
    return 0;
}
