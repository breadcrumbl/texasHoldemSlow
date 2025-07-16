#ifndef FINDBESTHAND_H
#define FINEBESTHAND_H

#include "cards.h"
#include <array>

Hand findBestHand(std::array<Card, cardCount>& allCards);
void updateHandInfo(Hand& outHand);
Result returnFirstHandResult(const Hand& hand1, const Hand& hand2);

#endif