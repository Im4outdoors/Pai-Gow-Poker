#include <array>
#include <string>
#include <vector>
#include "card.hpp"
using namespace std;

// ----- Valuation Functions -----
int compareHand(pair<HandRanks, CardRanks> d_hand, pair<HandRanks, CardRanks> p_hand); // Returns 1 for player wins, -1 for dealer wins

pair<HandRanks, CardRanks> ofAKind(const vector<Card>& hand); // Returns HandRanks:: {Pairs, TwoPairs, ThreeKind, Fullhouse, FourKind, or FiveAce} and Highest Card for Ties or Invalid.

CardRanks findHighestCard(const vector<Card>& hand); // returns highest CardRank  

CardRanks isStraight(const vector<Card>& hand); // Return the highest CardRank of a straight or Invalid

Card isFlush(const vector<Card>& hand); // Return the highest card of a flush or Invalid

pair<HandRanks, CardRanks> evaluateHand(const vector<Card>& hand); // Returns the hand rank and highest card rank


