#include "cards.h"
#include "templates.h"

#include <algorithm> // for std::sort
#include <array>
#include <iostream>

int main(int argc, char* argv[])
{
    // Get cards
    using enum Suit;
    std::array<Card, cardCount> allCards {};
    getCardsFromCL(allCards, argc, argv);

    // Sort cards in descending order (Ace = 14)
    std::sort(allCards.begin(), allCards.end(), [](const Card& a, const Card& b) {
        return a.rank > b.rank;
        });

    // Find and print best hand of 5 cards
    Hand bestHand {};
    bestHand = findBestHand(allCards);
    printCards(bestHand.cards);
    printHandType(bestHand);
    

    /*



    

    std::array<Card, cardCount> allCards {{
        {5, S},        
        {Q, H},
        {Q, S},
        {7, H},
        {7, D},
        {K, C},
        {2, H},
    }};

    //printCards(allCards);
    Hand testHand1 {};
    testHand1.cards = {{
        {A, H},        
        {A, H},
        {A, H},
        {6, D},
        {T, H},
    }};

    Hand testHand2 {};
    testHand2.cards = {{
        {A, H},        
        {A, H},
        {A, H},
        {3, D},
        {3, H},
    }};

    std::sort(testHand1.cards.begin(), testHand1.cards.end(), [](const Card& a, const Card& b) {
        return a.rank > b.rank;
        });
    
    std::sort(testHand2.cards.begin(), testHand2.cards.end(), [](const Card& a, const Card& b) {
        return a.rank > b.rank;
        });

    updateHandInfo(testHand1);
    updateHandInfo(testHand2);

    bestHand = returnStrongerOfTwoHands(testHand1, testHand2);

    


    */

    return 0;
}



