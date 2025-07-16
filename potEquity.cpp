#include "potEquity.h"
#include "cards.h"
#include "bestHand.h"

#include <array>
#include <iostream>


// Calculate pot equity on the river
void calculatePotEquity(const std::array<Card, cardCount>& allCardsOut, const Hand& ourBestHand)
{
    // Fill seven card hand with commmunity cards
    std::array<Card,cardCount> sevenCardHand {};
    
    int index { 0 };
    for (int i { 0 }; i < cardCount; ++i)
    {
        if (!allCardsOut[i].isHole)
            sevenCardHand[index++] = allCardsOut[i];
    }

    // Fill seven card hand with other cards left in deck
    // Find cards left in deck
    std::array<bool, cardsInDeck> isCardOutDeck { false };
    updateDeck(isCardOutDeck, allCardsOut);

    Hand theirBestHand {};
    std::array<int, 3> loseDrawWinCount {};
    
    for (int i { 0 }; i < cardsInDeck; ++i)
    {
        if (isCardOutDeck[i])
            continue;
        for (int j { i + 1 }; j < cardsInDeck; ++j)
        {   
            if (isCardOutDeck[j])
                continue;
            // Make hand of seven cards
            sevenCardHand[5].rank = getRankFromIndex(i);
            sevenCardHand[5].suit = getSuitFromIndex(i);
            sevenCardHand[6].rank = getRankFromIndex(j);
            sevenCardHand[6].suit = getSuitFromIndex(j);



            // Find best hand of those seven cards (create a copy as findBestArray sorts the hadn it is passed)
            std::array sevenCardHandCopy { sevenCardHand }; 
            theirBestHand = findBestHand(sevenCardHandCopy);

            // Compare to our hand and update loseDrawWinCount array
            Result result { returnFirstHandResult(ourBestHand, theirBestHand) };
            ++loseDrawWinCount[static_cast<std::size_t>(result)];


            // Debugging
            /*
            std::cout << "Seven cards: ";
            printCards(sevenCardHand);
            std::cout << "Their best hand: ";
            printCards(theirBestHand.cards);
            printHandType(theirBestHand);     
            constexpr std::array resultString { "Lose", "Draw", "Win"};
            std::cout << resultString[static_cast<int>(result)] << "\n\n";
            */
        }     
    }

    // Calculate pot equity
    int winCount { loseDrawWinCount[static_cast<std::size_t>(Result::win)] };
    int drawCount { loseDrawWinCount[static_cast<std::size_t>(Result::draw)] };
    int lossCount { loseDrawWinCount[static_cast<std::size_t>(Result::lose)] };
    double totalComparisons { static_cast<double>(winCount) + drawCount + lossCount };
    double winRate { winCount / totalComparisons };
    std::cout << "Wins: " << winCount << " Draws: " << drawCount << " Losses: " << lossCount << '\n';
    std::cout << "Win rate " << winRate * 100 << '\n';
    
    }


