#ifndef CARDS_H
#define CARDS_H

#include <array>
#include <iostream>


// Global constants
constexpr char suitChar[] = {'X', 'C', 'D', 'S', 'H'};
constexpr char rankChar[] = {'X', 'X', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

constexpr int cardCount { 7 };
constexpr int handSize { 5 };

constexpr int suitCount { 4 };
constexpr int rankCount { 13 };
constexpr int cardsInDeck { 52 };


// Enums
enum class Suit
{
    noSuit,
    C,
    D,
    S, 
    H,
};

enum Rank
{
    T = 10,
    J,
    Q,
    K,
    A,
};

enum class HandType
{
    noHandType = 0,
    highCard,
    pair,
    twoPair,
    trio,
    straight,
    flush,
    fullHouse,
    quad,
    straightFlush,
    royalFlush,
};

enum class Result 
{
    lose = 0,
    draw,
    win,
};


// Class Types
struct Card
{
    int rank { 0 };
    Suit suit { Suit:: noSuit };
    bool isHole { false };
};

struct HandInfo
{
    HandType handType { HandType::noHandType };
    int highCardRank { 0 };
    int quadOf { 0 };
    int trioOf { 0 };
    int pairOf { 0 };
    int twoPairOf { 0 };
};

struct Hand
{
    std::array<Card, handSize> cards {};
    HandInfo handInfo {};
};


// Function declarations
void printHandType(const Hand& hand);
int rankIntFromChar(char ch);
Suit suitFromChar(char ch);
int getFlatArrIndex(int rank, Suit suit);
Suit getSuitFromIndex(int index);
int getRankFromIndex(int index);

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
bool getCardsFromCL(std::array<Card, N>& array, int argc, char* argv[])
{
    // Check number of CL args is right
    if (argc != array.size() + 1)
    {
        std::cout << "Error: incorrect number of arguments\n";
        return false;
    }

    std::array<bool, cardsInDeck> isCardOutDeck { false };

    for (int i { 0 }; i < argc - 1; ++i)
    {
        int rank = rankIntFromChar(argv[i + 1][0]);
        Suit suit = suitFromChar(argv[i + 1][1]);
        // Check for repeat cards
        int index { getFlatArrIndex(rank, suit) };
        if(isCardOutDeck[index])
        {
            std::cout << "Error: repeated cards\n";
            return false;
        }

        // Add card to array
        array[i].rank = rank;
        array[i].suit = suit;
        if (argv[i + 1][2] == 'h')
            array[i].isHole = true;
        isCardOutDeck[index] = true;
    }
    return true;
}


#endif