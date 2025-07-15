#include "cards.h"
#include "cardFunctions.h"

#include <algorithm> // for std::sort
#include <array>
#include <iostream>


// Calculate pot equity on the river
void calculatePotEquity(const std::array<Card, cardCount>& allCardsOut, const Hand& ourBestHand)
{
    // Fill seven card hand with commmunity cards
    std::array<Card,cardCount> sevenCardHand {};

    int index { 0 };
    for (int i { 0 }; i < cardCount; ++i)
    {
        if (!allCardsOut[i].isHole)
        {
            sevenCardHand[index++] = allCardsOut[i];
        }    
    }

    // Fill seven card hand with other cards left in deck
    // Find cards left in deck
    std::array<bool, cardsInDeck> isCardOutDeck {};
    std::array<std::array<bool, suitCount>, rankCount> cardsInDeck {};
    updateDeck(cardsInDeck, allCardsOut);

    Hand theirBestHand {};
    std::array<int, 3> loseDrawWinCount {};
    int totComparisons { 0 };

    
    // Calculate their best hand
    // Iterate over first card
    for (int iSuit { 0 }; iSuit < suitCount; ++iSuit)
    {
        for (int iRank { 0 }; iRank < rankCount; ++iRank)
        {   
            if (!cardsInDeck[iRank][iSuit])
                continue;
            // Iterate over second card
            for (int jSuit { 0 }; jSuit < suitCount; ++jSuit)
            {
                for (int jRank { 0 }; jRank < rankCount; ++jRank)
                {   
                    if (iRank == jRank && iSuit == jSuit)
                        continue;
                    if (!cardsInDeck[jRank][jSuit])
                        continue;
                    // Make hand of seven cards
                    sevenCardHand[5].rank = static_cast<Rank>(iRank + 2);
                    sevenCardHand[5].suit = static_cast<Suit>(iSuit + 1);
                    sevenCardHand[6].rank = static_cast<Rank>(jRank + 2);
                    sevenCardHand[6].suit = static_cast<Suit>(jSuit + 1);

                    // Find best hand of those seven cards
                    theirBestHand = findBestHand(sevenCardHand);

                    // Compare to our hand and update loseDrawWinCount array
                    //Result result { returnFirstHandResult(ourBestHand, theirBestHand) };
                    //constexpr std::array resultArray {"lose", "draw", "win"};
                    ++loseDrawWinCount[static_cast<std::size_t>(returnFirstHandResult(ourBestHand, theirBestHand))];
                    ++totComparisons;
                }
            }
        }     
    }

    // Calculate pot equity
    int winCount { loseDrawWinCount[static_cast<std::size_t>(Result::win)] };
    int drawCount { loseDrawWinCount[static_cast<std::size_t>(Result::draw)] };
    int lossCount { loseDrawWinCount[static_cast<std::size_t>(Result::lose)] };
    double totalComparisons { static_cast<double>(winCount) + drawCount + lossCount };
    double winRate { winCount / totalComparisons };
    std::cout << "Wins: " << winCount << " Draws: " << drawCount << " Losses: " << lossCount << '\n';
    std::cout << "Win rate " << winRate * 100 << '\n';
    std::cout << "Comparison count " << totComparisons << '\n';
    
    }








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
                        tempHand.cards = {allCards[i], allCards[j], allCards[k], allCards[l], allCards[m]};
                        
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


// Compare two hands and return the strongest, or default return hand1 is a tie
//(might need to update this to return win, lose or draw
// when expand code to calc prob of winning but for now this is fine)
// maybe update to a member fn i.e. hand1.beats(hand2) = enum {true, false, draw}
// Although could just have a global array counting win, lose and draw
Hand returnStrongerOfTwoHands(const Hand& hand1, const Hand& hand2)
{
    using enum HandType;
    if (hand1.handInfo.handType > hand2.handInfo.handType)
    {
        return hand1;
    }
    if (hand1.handInfo.handType < hand2.handInfo.handType)
    {
        return hand2;
    }

    // If a straight (including royal flush and straight flush), hand with highest high card wins
    if (hand1.handInfo.handType == straight || hand1.handInfo.handType == straightFlush || hand1.handInfo.handType == royalFlush)
    {
        if (hand1.handInfo.highCardRank > hand2.handInfo.highCardRank)
            return hand1;
         if (hand1.handInfo.highCardRank < hand2.handInfo.highCardRank)
            return hand2;
        // If tied, return hand1
        return hand1;       
    }

    // If full house
    if (hand1.handInfo.handType == fullHouse)
    {
        // First compare rank of trio
        if (hand1.handInfo.trioOf > hand2.handInfo.trioOf)
            return hand1;
        if (hand1.handInfo.trioOf < hand2.handInfo.trioOf)
            return hand2;
        // If trio rank the same, compare rank of pair
        if (hand1.handInfo.pairOf > hand2.handInfo.pairOf)
            return hand1;
        if (hand1.handInfo.pairOf < hand2.handInfo.pairOf)
            return hand2;
        // If tie, return hand1
        return hand1;
    }

    // If two pair or pair 
    if (hand1.handInfo.handType == pair || hand1.handInfo.handType == twoPair)
    {
        // Compare highest pair first (pairOf)
        if (hand1.handInfo.pairOf > hand2.handInfo.pairOf)
            return hand1;
        if (hand1.handInfo.pairOf < hand2.handInfo.pairOf)
            return hand2;
        // If two  pair, compare next pair 
        if (hand1.handInfo.handType == twoPair)
        {
            if (hand1.handInfo.twoPairOf > hand2.handInfo.twoPairOf)
                return hand1;
            if (hand1.handInfo.twoPairOf < hand2.handInfo.twoPairOf)
                return hand2;
        }
        // Otherwise cycle through
    }

    // If trio
    if (hand1.handInfo.handType == trio)
    {
        if (hand1.handInfo.trioOf > hand2.handInfo.trioOf)
            return hand1;
        if (hand1.handInfo.trioOf < hand2.handInfo.trioOf)
            return hand2;
        // otherwise cycle through   
    }

    // If quad
    if (hand1.handInfo.handType == quad)
    {
        if (hand1.handInfo.quadOf > hand2.handInfo.quadOf)
            return hand1;
        if (hand1.handInfo.quadOf < hand2.handInfo.quadOf)
            return hand2;
        // otherwise cycle through   
    }

    // If everything else the same, cycle through cards from highest until one wins
    for (int i { 0 }; i < handSize; ++i)
    {
        if (hand1.cards[i].rank > hand2.cards[i].rank)
            return hand1;
        if (hand1.cards[i].rank < hand2.cards[i].rank)
            return hand2;
    }
    // If tie, return hand 1
    return hand1;
}



// Version 2 of returnStrongerOfTwoHands
// Return enum - win, draw or lose depending on hand1's result
// Later will probably turn this into an OOP style e.g. hand1.beats(hand2) or hand1.vs(hand2)
Result returnFirstHandResult(const Hand& hand1, const Hand& hand2)
{
    using enum HandType;
    using enum Result;
    if (hand1.handInfo.handType > hand2.handInfo.handType)
    {
        return win;
    }
    if (hand1.handInfo.handType < hand2.handInfo.handType)
    {
        return lose;
    }

    // If a straight (including royal flush and straight flush), hand with highest high card wins
    if (hand1.handInfo.handType == straight || hand1.handInfo.handType == straightFlush || hand1.handInfo.handType == royalFlush)
    {
        if (hand1.handInfo.highCardRank > hand2.handInfo.highCardRank)
            return win;
         if (hand1.handInfo.highCardRank < hand2.handInfo.highCardRank)
            return lose;       
    }

    // If full house
    if (hand1.handInfo.handType == fullHouse)
    {
        // First compare rank of trio
        if (hand1.handInfo.trioOf > hand2.handInfo.trioOf)
            return win;
        if (hand1.handInfo.trioOf < hand2.handInfo.trioOf)
            return lose;
        // If trio rank the same, compare rank of pair
        if (hand1.handInfo.pairOf > hand2.handInfo.pairOf)
            return win;
        if (hand1.handInfo.pairOf < hand2.handInfo.pairOf)
            return lose;
    }

    // If two pair or pair 
    if (hand1.handInfo.handType == pair || hand1.handInfo.handType == twoPair)
    {
        // Compare highest pair first (pairOf)
        if (hand1.handInfo.pairOf > hand2.handInfo.pairOf)
            return win;
        if (hand1.handInfo.pairOf < hand2.handInfo.pairOf)
            return lose;
        // If two  pair, compare next pair 
        if (hand1.handInfo.handType == twoPair)
        {
            if (hand1.handInfo.twoPairOf > hand2.handInfo.twoPairOf)
                return win;
            if (hand1.handInfo.twoPairOf < hand2.handInfo.twoPairOf)
                return lose;
        }
        // Otherwise cycle through
    }

    // If trio
    if (hand1.handInfo.handType == trio)
    {
        if (hand1.handInfo.trioOf > hand2.handInfo.trioOf)
            return win;
        if (hand1.handInfo.trioOf < hand2.handInfo.trioOf)
            return lose;
        // otherwise cycle through   
    }

    // If quad
    if (hand1.handInfo.handType == quad)
    {
        if (hand1.handInfo.quadOf > hand2.handInfo.quadOf)
            return win;
        if (hand1.handInfo.quadOf < hand2.handInfo.quadOf)
            return lose;
        // otherwise cycle through   
    }

    // If everything else the same, cycle through cards from highest until one wins
    for (int i { 0 }; i < handSize; ++i)
    {
        if (hand1.cards[i].rank > hand2.cards[i].rank)
            return win;
        if (hand1.cards[i].rank < hand2.cards[i].rank)
            return lose;
    }
    // If tie, return draw
    return draw;
}






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




