#include <array>
#include <string>
#include <vector>
#include <iostream>
#include "card.hpp"
using namespace std;

struct Player {
    string name;
    double bank = 0.0;
    std::vector<Card> hand;

    int bet(double amount) {
        if(bank - amount < 0) {
            std::cout << "Invalid Bet: Not enough Money" << std::endl;
            return -1;
        } else {
            bank -= amount;
            return amount;
        }
    }

    void clearHand() { hand.clear(); }

    void receiveCards(const std::vector<Card>& cards) {
        hand.insert(hand.end(), cards.begin(), cards.end());
    }

};