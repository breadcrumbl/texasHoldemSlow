#include "cards.h"

#include <iostream>

void printHandType(const Hand& hand)
{
    using namespace std::string_view_literals;
    HandType handType = hand.handInfo.handType;
    constexpr std::array handTypeString {"X"sv, "High card"sv, "Two of a kind"sv, "Two pair"sv, "Three of a kind"sv, 
                    "Straight"sv, "Flush"sv, "Full house"sv, "Four of a kind"sv, "Straight flush"sv, "Royal flush"sv};
    static_assert(std::size(handTypeString) == static_cast<int>(HandType::royalFlush) + 1);

    std::cout << handTypeString[static_cast<int>(handType)];
    if (handType == HandType::pair)
    {
        std::cout << ": Pair of " << rankChar[hand.handInfo.pairOf] << '\n';
        return;
    }
    if (handType == HandType::twoPair)
    {
        std::cout << ": Pair of " << rankChar[hand.handInfo.pairOf] << " and " << rankChar[hand.handInfo.twoPairOf] << '\n';
        return;
    }
    if (handType == HandType::trio)
    {
        std::cout << ": Trio of " << rankChar[hand.handInfo.trioOf] << '\n';
        return;
    }
    if (handType == HandType::straight || handType == HandType::flush || handType == HandType::straightFlush)
    {
        std::cout <<  ": " << rankChar[hand.handInfo.highCardRank] << "-high\n";
        return;
    }
    if (handType == HandType::fullHouse)
    {
        std::cout <<  ": " << rankChar[hand.handInfo.trioOf] << " over " << rankChar[hand.handInfo.pairOf] << '\n';
        return;
    }
    if (handType == HandType::quad)
    {
        std::cout << ": Quad of " << rankChar[hand.handInfo.quadOf] << '\n';
        return;
    }
}


int rankIntFromChar(char ch)
{
    if (ch >= '2' && ch <= '9')
    {
        return static_cast<int>(ch - '0');
    }
    if (ch == 'T' || ch == 't')
        return 10;
    if (ch == 'J' || ch == 'j')
        return 11;
    if (ch == 'Q' || ch == 'q')
        return 12;
    if (ch == 'K' || ch == 'k')
        return 13;
    if (ch == 'A' || ch == 'a')
        return 14;
    return 'X';
}

Suit suitFromChar(char ch)
{
    if (ch == 'H' || ch == 'h')
        return Suit::H;
    if (ch == 'S' || ch == 's')
        return Suit::S;
    if (ch == 'D' || ch == 'd')
        return Suit::D;
    if (ch == 'C' || ch == 'c')
        return Suit::C;
    return Suit::noSuit;
}


// Return value from 0 to 51 where array of cards is order 2C, 3C, ... AC, 2D, ..., AH 
int getFlatArrIndex(int rank, Suit suit)
{
    int rankIndex { rank - 2 }; // rank from 0 (2) to 12 (A)
    int suitIndex { static_cast<int>(suit) - 1 }; // suit from 0 (C) to 3 (H)
    return rankCount * suitIndex + rankIndex; 
}


// Return suit from 1 (C) to 4 (H)
Suit getSuitFromIndex(int index)
{
    return static_cast<Suit>(index / rankCount + 1);
}


// Return rank from 2 to A (14)
int getRankFromIndex(int index)
{
    return index % rankCount + 2;
}


