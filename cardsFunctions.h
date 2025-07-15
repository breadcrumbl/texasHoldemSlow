#ifndef CARDSFUNCTIONS_H
#define CARDSFUNCTIONS_H

#include "cards.h"
#include <array>
#include <iostream>

// Function declarations
Hand findBestHand(const std::array<Card, cardCount>& allCards);
void updateHandInfo(Hand& outHand);
Hand returnStrongerOfTwoHands(const Hand& hand1, const Hand& hand2);
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
        std::cout << rankChar[cards[i].rank] << suitChar[static_cast<std::size_t>(cards[i].suit)] << ' ';
    }
    std::cout << '\n';
}


// Feed cards to CL in form TH 6C 3D AS etc.
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
    }
}

#endif