// game.hpp
#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include <stdexcept>
#include "card.hpp"
#include "player.hpp"
#include "engine.cpp"

struct Game {
    std::vector<Card> deck;
    std::vector<Player> players;
    int dealerIndex = 0;  // rotate each round

    void initializeDeck() { // fill deck with 52 + jokers
        for(int i = 0; i < 4; i++) { // Suit
            for(int j = 2; j <= 14; j++) { // Card Rank
                deck.push_back(Card(static_cast<CardRanks>(j), static_cast<Suit>(i)));
            }
        }
        deck.push_back(Card()); // Joker
    }   

    static std::mt19937_64& rng() {
        static std::mt19937_64 gen{
            std::random_device{}()
        };
        return gen;
    }       

    void shuffleDeck() {
        std::shuffle(deck.begin(), deck.end(), rng());
    }

    void dealHands() {
        // Pai Gow: 7 cards per player
        const int N = static_cast<int>(players.size());
        const int perPlayer = 7;
        if (deck.size() < static_cast<size_t>(N * perPlayer)) {
            throw std::runtime_error("Deck underflow: not enough cards to deal");
        }

        // Clear old hands
        for (auto& p : players) p.clearHand();

        // Deal 7 to each player, sequentially
        auto it = deck.end();
        for (int i = 0; i < N; ++i) {
            it -= perPlayer;
            players[i].receiveCards(std::vector<Card>(it, it + perPlayer));
        }
        deck.erase(it, deck.end()); // remove dealt cards
    }

    // Helper: naive 5/2 split (placeholder until you implement house way)
    static inline std::pair<std::vector<Card>, std::vector<Card>>
    splitFiveTwo(const std::vector<Card>& seven) {
        // TODO: replace with proper house-way logic
        std::vector<Card> hi(seven.begin(), seven.begin() + 5);
        std::vector<Card> lo(seven.begin() + 5, seven.begin() + 7);
        return {hi, lo};
    }

    struct RoundOutcome {
        // per-player result vs dealer
        // -1 dealer wins, 1 player wins; dealer wins ties by your compareHand()
        int low = 0;
        int high = 0;
        int net = 0; // payout in dollars; fill when you add betting
    };

    std::vector<RoundOutcome> evaluateRound() {
        // Assume seat 0 is dealer for now; or use your dealerIndex
        const int dealerSeat = dealerIndex; // set dealerIndex elsewhere
        const auto& dealer7 = players[dealerSeat].hand;

        // safety
        if (dealer7.size() != 7) return {};

        auto [dHigh, dLow] = splitFiveTwo(dealer7);
        auto dHighEval = evaluateHand(dHigh);
        auto dLowEval  = evaluateHand(dLow);

        std::vector<RoundOutcome> results(players.size());

        for (int i = 0; i < static_cast<int>(players.size()); ++i) {
            if (i == dealerSeat) continue;

            const auto& p7 = players[i].hand;
            if (p7.size() != 7) continue;

            auto [pHigh, pLow] = splitFiveTwo(p7);
            auto pHighEval = evaluateHand(pHigh);
            auto pLowEval  = evaluateHand(pLow);

            RoundOutcome r{};
            r.high = compareHand(dHighEval, pHighEval); // 1 means player wins, -1 dealer wins
            r.low  = compareHand(dLowEval,  pLowEval);

            // Settlement stub (no commission, flat bet not wired yet):
            // If player wins both → +1 unit; loses both → -1 unit; split → 0 (but your compareHand makes dealer win ties)
            if (r.high == 1 && r.low == 1)      r.net = +1;
            else if (r.high == -1 && r.low == -1) r.net = -1;
            else                                  r.net = 0;

            results[i] = r;
        }

        return results;
    }
};