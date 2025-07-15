#ifndef CARDS_H
#define CARDS_H

#include <array>

// Global constants
constexpr char suitChar[] = {'X', 'H', 'S', 'D', 'C'};
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
    H,
    S,
    D, 
    C,
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

#endif