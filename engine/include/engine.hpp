#include <array>
#include <string>
#include <vector>
using namespace std;
// ----- Core Enums -----
enum class HandRanks { High = 0, Pair, TwoPair, ThreeKind, Straight, Flush, FullHouse, FourKind, StraightFlush, FiveAce, Invalid = -1};

enum class CardRanks { Two = 2, Three, Four, Five, Six, Seven,  Eight, Nine,  Ten, Jack, Queen, King, Ace, Invalid = -1};

enum class Suit { Spades, Hearts, Diamonds, Clubs, Invalid = -1 };

// ----- Card Type -----
struct Card {
    CardRanks rank;   // 2..14 (where 11=J, 12=Q, 13=K, 14=A)
    Suit suit;   // 0=♠, 1=♥, 2=♦, 3=♣ ; use -1 if Joker
    bool joker; // true = Joker card
};

// ----- Valuation Functions -----
int compareHand(pair<HandRanks, CardRanks> d_hand, pair<HandRanks, CardRanks> p_hand); // Returns 1 for player wins, -1 for dealer wins

pair<HandRanks, CardRanks> ofAKind(const vector<Card>& hand); // Returns HandRanks:: {Pairs, TwoPairs, ThreeKind, Fullhouse, FourKind, or FiveAce} and Highest Card for Ties or Invalid.

CardRanks findHighestCard(const vector<Card>& hand); // returns highest CardRank  

CardRanks isStraight(const vector<Card>& hand); // Return the highest CardRank of a straight or Invalid

Card isFlush(const vector<Card>& hand); // Return the highest card of a flush or Invalid

pair<HandRanks, CardRanks> evaluateHand(const vector<Card>& hand); // Returns the hand rank and highest card rank


