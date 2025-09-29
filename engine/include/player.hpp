#include <array>
#include <string>
#include <vector>
#include "card.hpp"
using namespace std;

struct Player {
    string name;
    double bank = 0.0;
    std::vector<Card> hand;

    void clearHand() { hand.clear(); }

    void receiveCards(const std::vector<Card>& cards) {
        hand.insert(hand.end(), cards.begin(), cards.end());
    }
};