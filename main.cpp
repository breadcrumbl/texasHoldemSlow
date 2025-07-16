#include "bestHand.h"
#include "cards.h"
#include "potEquity.h"

#include <algorithm> // for std::sort
#include <array>
#include <iostream>


int main(int argc, char* argv[])
{
    // Get cards
    using enum Suit;
    
    std::array<Card, cardCount> allCards {};
    if(!getCardsFromCL(allCards, argc, argv))
        return 1;


    Hand bestHand {};
    bestHand = findBestHand(allCards);
    printCards(bestHand.cards);
    printHandType(bestHand);
    calculatePotEquity(allCards, bestHand);


    return 0;
}



