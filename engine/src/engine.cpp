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

int compareHand(pair<HandRanks, CardRanks> d_hand, pair<HandRanks, CardRanks> p_hand) { // Returns 1 for player wins, -1 for dealer wins, 0 for tie
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

pair<HandRanks, CardRanks> ofAKind(const vector<Card>& hand) { // Return a pair representing how many cards of a kind there are.
    int counts[15] = {0};  // 0..14, but only 2..14 used
    int jokers = 0;
    int bestCount = 0;
    int bestRank = -1;
    bool multi = false;

    // Count ranks and jokers
    for (const Card& c : hand) { // Read Hand
        if (c.joker) {
            jokers++;
        } else {
            counts[static_cast<int>(c.rank)]++;
        }
    }

    for(int i = 2; i < 15; i ++) { // Check for best Rank, Counts, and multiple pairs
        if(counts[i] == bestCount &&  bestCount > 1) {
            multi = true;
            bestRank = i;
        } else if(counts[i] >= bestCount) {
            bestCount = counts[i];
            bestRank = i;
        }  
    }

    if(jokers > 0) {
        for(int i = 0; i < jokers; i ++){
            bestCount++;
        }    
    }

    if(multi) {
        if(bestCount > 2) { // Full House
            return {HandRanks::FullHouse, static_cast<CardRanks>(bestRank)};
        } else { // Two Pair
            return {HandRanks::TwoPair, static_cast<CardRanks>(bestRank)};
        }
    } else {
        if(bestCount == 2) { // Pair
            return {HandRanks::Pair, static_cast<CardRanks>(bestRank)};
        } else if(bestCount == 3) { // Trips
            return {HandRanks::ThreeKind, static_cast<CardRanks>(bestRank)};
        } else if(bestCount == 4) { // Quads
            return {HandRanks::FourKind, static_cast<CardRanks>(bestRank)};
        } else if(bestCount == 5) { // Five Ace
            return {HandRanks::FiveAce, static_cast<CardRanks>(bestRank)};
        }
    }

    return {HandRanks::Invalid, CardRanks::Invalid};

}

CardRanks findHighestCard(const vector<Card>& hand) { // returns highest card  
    CardRanks bestRank = CardRanks::Invalid;
    for(const Card& c: hand) {
        if(c.rank > bestRank) {
            bestRank = c.rank;
        }
    }
    return bestRank;
}

CardRanks isStraight(const vector<Card>& hand) { // Return the highest card rank of a straight or -1
    return CardRanks::Invalid;
}

Card isFlush(const vector<Card>& hand) { // Return the highest card of a flush or -1
    return { CardRanks::Invalid, Suit::Invalid, false };
}

pair<HandRanks, CardRanks> evaluateHand(const vector<Card>& hand) { // Returns the hand rank and highest card rank
    if (hand.size() != 2 && hand.size() != 5) {
        return {HandRanks::Invalid, CardRanks::Invalid};
    }

    pair<HandRanks, CardRanks> eval = ofAKind(hand);

    if(hand.size() == 2) {  // Low Hand 
        if( eval.first != HandRanks::Invalid ) { // check ofAKind() output
            return eval;
        } else { // Highest Card
            return { HandRanks::High, findHighestCard(hand) }; 
        }
 
    } else { // High Hand
        if( eval.first != HandRanks::Invalid ) { // check ofAKind() output
            return eval;
        } else {
            CardRanks checkStraight = isStraight(hand);
            Card checkFlush = isFlush(hand);
            if(checkStraight != CardRanks::Invalid) {
                if(checkFlush.rank != CardRanks::Invalid) {
                    return {HandRanks::StraightFlush, checkFlush.rank};
                }
            } else if(checkFlush.rank != CardRanks::Invalid){
                return {HandRanks::Flush, checkFlush.rank};
            } else {
                return { HandRanks::High, findHighestCard(hand) }; 
            }
        }
    }
}  