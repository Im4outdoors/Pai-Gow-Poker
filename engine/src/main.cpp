// src/main.cpp
#include "engine.hpp"
#include <iostream>
#include <vector>

static Card C(CardRanks r, Suit s) { return Card{r, s}; }
static Card J() { return Card(); }

int main() {
    std::vector<Card> hand = {
        C(CardRanks::Ten,  Suit::Spades),
        C(CardRanks::Jack, Suit::Spades),
        C(CardRanks::Queen,Suit::Spades),
        C(CardRanks::King, Suit::Spades),
        C(CardRanks::Ace,  Suit::Spades)
    };
    auto res = evaluateHand(hand);
    std::cout << "Rank=" << static_cast<int>(res.first)
              << " high=" << static_cast<int>(res.second) << "\n";
    return 0;
}
