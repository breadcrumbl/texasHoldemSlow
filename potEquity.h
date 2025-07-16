#ifndef POTEQUITY_H
#define POTEQUITY_H

#include "bestHand.h"
#include "cards.h"

#include <array>


// Function declarations
void calculatePotEquity(const std::array<Card, cardCount>& allCardsOut, const Hand& ourBestHand);


// Template Functions
template <std::size_t N>
void updateDeck(std::array<bool, cardsInDeck>& isCardOutDeck, const std::array<Card, N>& cardsOutDeck)
{
    for (int i { 0 }; i < N; ++i)
    {
        int index { getFlatArrIndex(cardsOutDeck[i].rank, cardsOutDeck[i].suit) };
        isCardOutDeck[index] = true;
    }
}

#endif