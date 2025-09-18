#include <array>
#include <string>
#include <vector>

// ----- Core Enums -----
enum class HandRanks { High = 0, Pair, TwoPair, ThreeKind, Straight, Flush, FullHouse, FourKind, StraightFlush, FiveAce};

enum class cardRanks { Two = 2, Three, Four, Five, Six, Seven,  Eight, Nine,  Ten, Jack, Queen, King, Ace };

enum class suit { Spades, Hearts, Diamonds, Clubs, None };

// ----- Card Type -----
const struct Card {
    cardRanks rank;   // 2..14 (where 11=J, 12=Q, 13=K, 14=A)
    int suit;   // 0=♠, 1=♥, 2=♦, 3=♣ ; use -1 if Joker
    bool joker; // true = Joker card
};

// ----- Valuation Functions -----
bool compareHand(pair<HandRanks, cardRanks> d_hand, pair<HandRanks, cardRanks> p_hand); // Returns 1 for player wins, -1 for dealer wins

pair<int, cardRanks> ofAKind(const vector<Card>& hand); // Return a string representing how many cards of a kind there are.

HandRanks findHighestCard(const vector<Card>& cards); // returns highest card  

HandRanks isStraight(const array<Card, 5>& hand); // Return the highest card of a straight or -1

Card isFlush(const array<Card, 5>& hand); // Return the highest card of a flush or -1

pair<HandRanks, cardRanks> evaluateHand(const vector<Card>& a); // Returns the hand rank and highest card rank
