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
        if(static_cast<int>(c.rank) > static_cast<int>(bestRank)) {
            bestRank = c.rank;
        }
    }
    return bestRank;
}

CardRanks isStraight(const vector<Card>& hand) { // Return the highest card rank of a straight or -1
    int J = 0;
    std::vector<int> ranks;
    ranks.reserve(5);
    for (const auto& c : hand) {
        if (c.joker) { ++J; continue; }
        ranks.push_back(static_cast<int>(c.rank));
    }

    std::sort(ranks.begin(), ranks.end());
    ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end()); // Only keep unique rank values

    if (ranks.empty()) return (J >= 5) ? CardRanks::Ace : CardRanks::Invalid;

    // Helper: membership test on sorted vector
    auto has = [&](int r)->bool {
        return std::binary_search(ranks.begin(), ranks.end(), r);
    }; 

    // Try non-wheel straights: high from Ace(14) down to 6
    for (int high = 14; high >= 6; --high) {
        int missing = 0;
        for (int r = high - 4; r <= high; ++r) {
            if (!has(r)) ++missing;
        }
        if (missing <= J) return static_cast<CardRanks>(high);
    }

    // Wheel (A-2-3-4-5): high card is Five
    bool hasAce = has(14);
    if (hasAce) {
        int have = 0;
        for (int r = 2; r <= 5; ++r) if (has(r)) ++have;
        int missing = 5 - (have + 1); // +1 for Ace acting as 1
        if (missing <= J) return CardRanks::Five;
    }

    return CardRanks::Invalid;

}

Card isFlush(const std::vector<Card>& hand) {
    if (hand.size() != 5) return { CardRanks::Invalid, Suit::Invalid, false };

    int J = 0;
    int suitCount[4] = {0,0,0,0}; 
    CardRanks hiInSuit[4] = { CardRanks::Invalid, CardRanks::Invalid,
                              CardRanks::Invalid, CardRanks::Invalid };

    for (const auto& c : hand) {
        if (c.joker) { ++J; continue; }
        int s = static_cast<int>(c.suit);  
        if (s >= 0 && s < 4) {
            ++suitCount[s];
            if (static_cast<int>(c.rank) > static_cast<int>(hiInSuit[s])) {
                hiInSuit[s] = c.rank;
            }
        }
    }

    // Find the best flush candidate: suit with (count+J>=5) and highest hiInSuit
    int bestSuit = -1;
    CardRanks bestHigh = CardRanks::Invalid;

    for (int s = 0; s < 4; ++s) {
        if (suitCount[s] + J >= 5) {
            // if we have no natural card in that suit (all jokers), treat high as Ace
            CardRanks hi = (hiInSuit[s] == CardRanks::Invalid) ? CardRanks::Ace : hiInSuit[s];

            if (bestSuit == -1 ||
                static_cast<int>(hi) > static_cast<int>(bestHigh) ||
               (static_cast<int>(hi) == static_cast<int>(bestHigh) && s > bestSuit)) {
                bestSuit = s;
                bestHigh = hi;
            }
        }
    }

    if (bestSuit == -1) {
        return { CardRanks::Invalid, Suit::Invalid, false }; // no flush
    }

    // Map index back to Suit
    Suit suitVal = (bestSuit == 0) ? Suit::Spades
                  : (bestSuit == 1) ? Suit::Hearts
                  : (bestSuit == 2) ? Suit::Diamonds
                                    : Suit::Clubs;

    // Return a representative "highest flush card" (joker=false is fine here)
    return { bestHigh, suitVal, false };
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