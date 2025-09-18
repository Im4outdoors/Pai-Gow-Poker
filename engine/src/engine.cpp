#include "engine.hpp"

#include <array>       
#include <vector>      
#include <algorithm>   
#include <random>      
#include <string>      
#include <sstream>     
#include <iostream>    
#include <stdexcept>   

using namespace std;

bool compareHand(pair<HandRanks, cardRanks> d_hand, pair<HandRanks, cardRanks> p_hand) { // Returns 1 for player wins, -1 for dealer wins, 0 for tie
    if(d_hand.first > p_hand.first) {
        return -1;
    } else if(d_hand.first < p_hand.first) {
        return 1;
    } else {

        if(d_hand.second > p_hand.second) {
            return -1;
        } else if(d_hand.second > p_hand.second) {
            return 1;
        } else {
            return 0;
        }

    }
}

pair<int, cardRanks> ofAKind(const vector<Card>& hand) { // Return a string representing how many cards of a kind there are.

}

HandRanks findHighestCard(const vector<Card>& cards) { // returns highest card  

}

HandRanks isStraight(const array<Card, 5>& hand) { // Return the highest card of a straight or -1

}

Card isFlush(const array<Card, 5>& hand) { // Return the highest card of a flush or -1

}

pair<HandRanks, cardRanks> evaluateHand(const vector<Card>& a) { // Returns the hand rank and highest card rank

}