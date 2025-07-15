#include "cards.h"
#include "cardFunctions.h"

#include <algorithm> // for std::sort
#include <array>
#include <iostream>

template<std::size_t ROW, std::size_t COL>
void printDeck(std::array<std::array<bool,COL>, ROW>& deck)
{
    for (int i { 0 }; i < ROW; ++i)
    {
        for (int j { 0 }; j < COL; ++j)
        {
            std::cout << deck[i][j] << " ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

int main(int argc, char* argv[])
{
    // Get cards
    using enum Suit;
    std::array<Card, cardCount> allCards {};
    getCardsFromCL(allCards, argc, argv);


    Hand bestHand {};
    bestHand = findBestHand(allCards);
    printCards(bestHand.cards);
    printHandType(bestHand);
    calculatePotEquity(allCards, bestHand);



    return 0;
}



