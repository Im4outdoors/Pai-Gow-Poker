#include <array>
#include <string>
#include <vector>
#include "card.hpp"
using namespace std;

struct Player {
    string name;
    double bank = 0.0;
    std::vector<Card> hand;

    // Constructor
    Player(const std::string& n, int b = 0) 
        : name(n), bank(b) {}

    int bet(double amount);
    void clearHand();
    void receiveCards(const std::vector<Card>& cards);
};