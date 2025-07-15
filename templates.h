#ifndef TEMPLATES_H
#define TEMPLATES_H

#include "cards.h"
#include <array>
#include <iostream>

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