#include "bestHand.h"
#include "cards.h"

#include <algorithm> // for std::sort
#include <array>
#include <iostream>


// Be given seven cards and output the best five-card hand with hand info updated
Hand findBestHand(std::array<Card, cardCount>& allCards)
{
    // Sort seven card hand in descending order
    std::sort(allCards.begin(), allCards.end(), [](const Card& a, const Card& b) {
        return a.rank > b.rank;
        });

    // Define best five card hand
    Hand bestHand {};

    for (int i = 0; i < cardCount; ++i)
    {
        for (int j = i + 1; j < cardCount; ++j)
        {
            for (int k = j + 1; k < cardCount; ++k)
            {
                for (int l = k + 1; l < cardCount; ++l)
                {
                    for (int m = l + 1; m < cardCount; ++m)
                    {
                        // Create hand
                        Hand tempHand {};
                        tempHand.cards = { allCards[i], allCards[j], allCards[k], allCards[l], allCards[m] };
                        
                        // Identify hand
                        updateHandInfo(tempHand);

                        // Compare to last hand
                        Result result = returnFirstHandResult(bestHand, tempHand);
                        if (result != Result::win)
                            bestHand = tempHand;
                    }
                }
            }
        }
    }
    return bestHand;
}


// Takes in a hand sorted in descending order and updates handInfo
void updateHandInfo(Hand& outHand)
{
    using enum HandType;
    // Set high card (this will only change if five-high straight is best hand)
    outHand.handInfo.highCardRank = outHand.cards[0].rank;

    // Flush and straight bool
    bool isFlush { true };
    bool isStraight { true };
    
    // Array to count number of each rank - manually increment the rank for card 0 as below loops starts from 1
    std::array<int, A + 1> rankCount { 0 };
    ++rankCount[outHand.cards[0].rank];

    for (int i { 1 }; i < handSize; ++i)
    {
        // Check for flush
        if (outHand.cards[i].suit != outHand.cards[0].suit)
            isFlush = false;

        // Check for straight (excluding five-high)
        if (outHand.cards[i].rank + 1 != outHand.cards[i - 1].rank)
            isStraight = false;

        // Count any repeats
        ++rankCount[outHand.cards[i].rank];
        
    }

    // Check for five-high straight (A2345) if no higher straight found
    bool isFiveHigh { false };
    if (!isStraight && outHand.cards[0].rank == A  && outHand.cards[1].rank == 5)
    {
        isStraight = true;
        isFiveHigh = true;
        for (int i { 2 }; i < handSize; ++i)
        {
            if (outHand.cards[i].rank + 1 != outHand.cards[i - 1].rank)
            {
                isStraight = false;
                isFiveHigh = false;
                break;
            }
        }
    }

    // Check for straight flush
    if (isFlush && isStraight)
    {
        // If five-high, return straight flush with high card = 5
        if (isFiveHigh)
        {
            outHand.handInfo.handType = straightFlush;
            outHand.handInfo.highCardRank = 5;
            return;
        }
        // If royal flush, return royal flush
        else if (outHand.cards[0].rank == A)
        {
            outHand.handInfo.handType = royalFlush;
            return;
        }
        else
        {
        // Return straight flush
        outHand.handInfo.handType = straightFlush;
        return;
        }

    }

    // Check for quad, full house, trios, and pairs
    bool isTrio { false };
    bool isTwoPair { false };
    bool isPair { false };

    for (int i { A }; i >= 2; --i)
    {
        // Check for quad
        if (rankCount[i] == 4)
        {
            outHand.handInfo.handType = quad;
            outHand.handInfo.quadOf = i;
            return;
        }
        // Check for trio
        if (rankCount[i] == 3)
        {
            isTrio = true;
            outHand.handInfo.trioOf = i;
        }
        // Check for pair - if more than one pair store highest in pairOf and lowest in twoPairOf
        if (rankCount[i] == 2)
        {
            if (!isPair)
            {
                isPair = true;
                outHand.handInfo.pairOf = i;
            }
            else
            {
                isTwoPair = true;
                outHand.handInfo.twoPairOf = i;
            }
        }
    }

    if (isTrio && isPair) // Check for full house
    {
        outHand.handInfo.handType = fullHouse;
        return;
    }
    if (isFlush)
    {
        outHand.handInfo.handType = flush;
        return;
    }
    if (isStraight)
    {
        outHand.handInfo.handType = straight;
        return;
    }
    if (isTrio)
    {
        outHand.handInfo.handType = trio;
        return;
    }
    if (isTwoPair)
    {
        outHand.handInfo.handType = twoPair;
        return;
    }
    if (isPair)
    {
        outHand.handInfo.handType = pair;
        return;
    }
    outHand.handInfo.handType = highCard;
}


// Version 2 of returnStrongerOfTwoHands
// Return enum - win, draw or lose depending on hand1's result
// Later will probably turn this into an OOP style e.g. hand1.beats(hand2) or hand1.vs(hand2)
Result returnFirstHandResult(const Hand& hand1, const Hand& hand2)
{
    using enum HandType;
    using enum Result;
    if (hand1.handInfo.handType > hand2.handInfo.handType) return win;
    if (hand1.handInfo.handType < hand2.handInfo.handType) return lose;

    switch (hand1.handInfo.handType)
    {
        case straight:
        case straightFlush:
        case royalFlush:
            if (hand1.handInfo.highCardRank > hand2.handInfo.highCardRank) return win;
            if (hand1.handInfo.highCardRank < hand2.handInfo.highCardRank) return lose;
            return draw;
        case fullHouse:
            // First compare rank of trio
            if (hand1.handInfo.trioOf > hand2.handInfo.trioOf) return win;
            if (hand1.handInfo.trioOf < hand2.handInfo.trioOf) return lose;
            // If trio rank the same, compare rank of pair
            if (hand1.handInfo.pairOf > hand2.handInfo.pairOf) return win;
            if (hand1.handInfo.pairOf < hand2.handInfo.pairOf) return lose;
            return draw;
        case pair:
            if (hand1.handInfo.pairOf > hand2.handInfo.pairOf) return win;
            if (hand1.handInfo.pairOf < hand2.handInfo.pairOf) return lose;
            break; // If same, fall through to loop comparing to find first highest card
        case twoPair:
            // Compare highest pair first (pairOf)
            if (hand1.handInfo.pairOf > hand2.handInfo.pairOf) return win;
            if (hand1.handInfo.pairOf < hand2.handInfo.pairOf) return lose;
            // If two  pair, compare next pair 
            if (hand1.handInfo.twoPairOf > hand2.handInfo.twoPairOf) return win;
            if (hand1.handInfo.twoPairOf < hand2.handInfo.twoPairOf)return lose;
            break;
        case trio:
            if (hand1.handInfo.trioOf > hand2.handInfo.trioOf) return win;
            if (hand1.handInfo.trioOf < hand2.handInfo.trioOf) return lose;
            break;
        case quad:
            if (hand1.handInfo.quadOf > hand2.handInfo.quadOf) return win;
            if (hand1.handInfo.quadOf < hand2.handInfo.quadOf) return lose;
            break;
        default:
            break;
    }
    
    // If everything else the same, cycle through cards from highest until one wins or ends in draw
    for (int i { 0 }; i < handSize; ++i)
    {
        if (hand1.cards[i].rank > hand2.cards[i].rank) return win;
        if (hand1.cards[i].rank < hand2.cards[i].rank) return lose;
    }
    return draw;
}