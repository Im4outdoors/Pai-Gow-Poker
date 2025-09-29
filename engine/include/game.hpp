// game.hpp
#pragma once
#include <vector>
#include "player.hpp"

struct Game {
    std::vector<Card> deck;
    std::vector<Player> players;
    int dealerIndex = 0;  // rotate each round

    void initializeDeck();   // fill deck with 52 + jokers
    void shuffleDeck();
    void dealHands();
    void evaluateRound();
};