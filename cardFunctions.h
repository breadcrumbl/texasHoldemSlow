#ifndef CARDSFUNCTIONS_H
#define CARDSFUNCTIONS_H

#include "cards.h"
#include <array>
#include <iostream>

// Function declarations
Hand findBestHand(std::array<Card, cardCount>& allCards);
void updateHandInfo(Hand& outHand);

Hand returnStrongerOfTwoHands(const Hand& hand1, const Hand& hand2);
Result returnFirstHandResult(const Hand& hand1, const Hand& hand2);

void calculatePotEquity(const std::array<Card, cardCount>& allCardsOut, const Hand& ourBestHand);
int rankIntFromChar(char ch);
Suit suitFromChar(char ch);
void printHandType(const Hand& hand);

// Function templates

// Print an array of cards of size N
template <std::size_t N>
void printCards(const std::array<Card, N>& cards)
{
    for (int i = 0; i < N; i++)
    {
        std::cout << rankChar[cards[i].rank] << suitChar[static_cast<std::size_t>(cards[i].suit)];
        if (cards[i].isHole)
            std::cout << 'h';
        std::cout << ' ';
    }
    std::cout << '\n';
}


// Feed cards to CL in form THh 6Ch 3D AS etc. where subscript h denotes two hole cards
template <std::size_t N>
void getCardsFromCL(std::array<Card, N>& array, int argc, char* argv[])
{
    // Check number of CL args is right
    if (argc != array.size() + 1)
    {
        std::cout << "Incorrect number of arguments\n";
        return;
    }

    for (int i { 0 }; i < argc - 1; ++i)
    {
        array[i].rank = rankIntFromChar(argv[i + 1][0]);
        array[i].suit = suitFromChar(argv[i + 1][1]);
        if (argv[i + 1][2] == 'h')
            array[i].isHole = true;
    }
}

// For cardsInDeck[13][4] array, 
template <std::size_t N>
void updateDeck(std::array<std::array<bool, suitCount>, rankCount>& cardsInDeck, const std::array<Card, N>& cardsOutDeck)
{
    // Set all elements of cardsInDeck to true
    for (auto& row : cardsInDeck) 
    {
        row.fill(true);
    }

    // Set hole or community cards to false
    for (int i { 0 }; i < N; ++i)
    {
        cardsInDeck[static_cast<std::size_t>(cardsOutDeck[i].rank - 2)][static_cast<std::size_t>(cardsOutDeck[i].suit) - 1] = false;
    }
}

template <std::size_t N>
void updateDeck2(std::array<bool, cardsInDeck>& isCardOutDeck, const std::array<Card, N>& cardsOutDeck)
{
    // Set hole or community cards to true
    for (int i { 0 }; i < N; ++i)
    {
        std::size_t rankIndex { static_cast<std::size_t>(cardsOutDeck[i].rank - 2)};
        std::size_t suitIndex { static_cast<std::size_t>(cardsOutDeck[i].suit) - 1 };
        std::size_t arrayIndex { rankCount * suitIndex + rankIndex};
    }
}

#endif