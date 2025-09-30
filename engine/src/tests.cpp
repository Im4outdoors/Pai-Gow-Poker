// tests.cpp
#include "engine.hpp"
#include <vector>
#include <iostream>
#include <cstdlib>

using std::cout;
using std::vector;

static int toInt(CardRanks r) { return static_cast<int>(r); }
static int toInt(HandRanks r) { return static_cast<int>(r); }
static int toInt(Suit s)      { return static_cast<int>(s); }

// Minimal assert helpers
#define ASSERT_TRUE(cond) do { if(!(cond)){ \
  cout << "ASSERT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; std::exit(1);} } while(0)

#define ASSERT_EQ(a,b) do { auto _va=(a), _vb=(b); if(!((_va)==(_vb))){ \
  cout << "ASSERT_EQ failed: " #a " vs " #b " at " __FILE__ ":" << __LINE__ \
       << " | got=" << toInt(_va) << " expected=" << toInt(_vb) << "\n"; std::exit(1);} } while(0)

// Convenience card builders
static Card C(CardRanks r, Suit s) { return Card{r, s}; }
static Card J() { return Card(); }

// ---------- Individual tests ----------
static void test_low_hand() {
    // Pair (K K)
    vector<Card> h1 = { C(CardRanks::King, Suit::Spades), C(CardRanks::King, Suit::Hearts) };
    auto r1 = evaluateHand(h1);
    ASSERT_EQ(r1.first, HandRanks::Pair);
    ASSERT_EQ(r1.second, CardRanks::King);

    // High card (K, 7)
    vector<Card> h2 = { C(CardRanks::King, Suit::Spades), C(CardRanks::Seven, Suit::Hearts) };
    auto r2 = evaluateHand(h2);
    ASSERT_EQ(r2.first, HandRanks::High);
    ASSERT_EQ(r2.second, CardRanks::King);

    // Joker + Q => Pair of Queens
    vector<Card> h3 = { C(CardRanks::Queen, Suit::Spades), J() };
    auto r3 = evaluateHand(h3);
    ASSERT_EQ(r3.first, HandRanks::Pair);
    ASSERT_EQ(r3.second, CardRanks::Queen);
}

static void test_ofAKind() {
    // Trips
    vector<Card> h = {
        C(CardRanks::Nine, Suit::Spades),
        C(CardRanks::Nine, Suit::Hearts),
        C(CardRanks::Nine, Suit::Clubs),
        C(CardRanks::Two,  Suit::Diamonds),
        C(CardRanks::Five, Suit::Clubs)
    };
    auto r = ofAKind(h);
    ASSERT_EQ(r.first, HandRanks::ThreeKind);
    ASSERT_EQ(r.second, CardRanks::Nine);

    // Two pair (5,9)
    vector<Card> h2 = {
        C(CardRanks::Nine, Suit::Spades),
        C(CardRanks::Nine, Suit::Hearts),
        C(CardRanks::Five, Suit::Clubs),
        C(CardRanks::Five, Suit::Diamonds),
        C(CardRanks::Two,  Suit::Spades)
    };
    auto r2 = ofAKind(h2);
    ASSERT_EQ(r2.first, HandRanks::TwoPair);
    ASSERT_EQ(r2.second, CardRanks::Nine);

    // Trips + Joker => Quads
    vector<Card> h3 = {
        C(CardRanks::Ten, Suit::Spades),
        C(CardRanks::Ten, Suit::Hearts),
        C(CardRanks::Ten, Suit::Clubs),
        J(),
        C(CardRanks::Two, Suit::Clubs)
    };
    auto r3 = ofAKind(h3);
    ASSERT_EQ(r3.first, HandRanks::FourKind);
    ASSERT_EQ(r3.second, CardRanks::Ten);

    // Two pair + Joker => Full House
    vector<Card> h4 = {
        C(CardRanks::Eight, Suit::Spades),
        C(CardRanks::Eight, Suit::Hearts),
        C(CardRanks::King,  Suit::Clubs),
        C(CardRanks::King,  Suit::Diamonds),
        J()
    };
    auto r4 = ofAKind(h4);
    ASSERT_EQ(r4.first, HandRanks::FullHouse);
    ASSERT_TRUE(r4.second == CardRanks::Eight || r4.second == CardRanks::King); // trips rank per your impl
}

static void test_straight() {
    // Natural straight A-K-Q-J-10
    vector<Card> s1 = {
        C(CardRanks::Ten,  Suit::Spades),
        C(CardRanks::Jack, Suit::Hearts),
        C(CardRanks::Queen,Suit::Clubs),
        C(CardRanks::King, Suit::Diamonds),
        C(CardRanks::Ace,  Suit::Clubs)
    };
    ASSERT_EQ(isStraight(s1), CardRanks::Ace);

    // Wheel A-2-3-4-5 (mixed suits)
    vector<Card> s2 = {
        C(CardRanks::Ace,  Suit::Spades),
        C(CardRanks::Two,  Suit::Hearts),
        C(CardRanks::Three,Suit::Clubs),
        C(CardRanks::Four, Suit::Diamonds),
        C(CardRanks::Five, Suit::Clubs)
    };
    ASSERT_EQ(isStraight(s2), CardRanks::Five);

    // Gap filled by Joker: 10 J Q (Joker) A -> K-high
    vector<Card> s3 = {
        C(CardRanks::Ten,  Suit::Spades),
        C(CardRanks::Jack, Suit::Hearts),
        C(CardRanks::Queen,Suit::Clubs),
        C(CardRanks::Ace,  Suit::Diamonds),
        J()
    };
    ASSERT_EQ(isStraight(s3), CardRanks::Ace);

    // Not a straight
    vector<Card> s4 = {
        C(CardRanks::Ten, Suit::Spades),
        C(CardRanks::Ten, Suit::Hearts),
        C(CardRanks::Three, Suit::Clubs),
        C(CardRanks::Seven, Suit::Diamonds),
        C(CardRanks::Ace, Suit::Clubs)
    };
    ASSERT_EQ(isStraight(s4), CardRanks::Invalid);
}

static void test_flush() {
    // Natural flush (spades)
    vector<Card> f1 = {
        C(CardRanks::Two,   Suit::Spades),
        C(CardRanks::Nine,  Suit::Spades),
        C(CardRanks::Queen, Suit::Spades),
        C(CardRanks::Five,  Suit::Spades),
        C(CardRanks::King,  Suit::Spades)
    };
    auto fr1 = isFlush(f1);
    ASSERT_EQ(fr1.suit, Suit::Spades);
    ASSERT_EQ(fr1.rank, CardRanks::King);

    // 4 hearts + Joker => hearts flush
    vector<Card> f2 = {
        C(CardRanks::Two,   Suit::Hearts),
        C(CardRanks::Nine,  Suit::Hearts),
        C(CardRanks::Queen, Suit::Hearts),
        C(CardRanks::Five,  Suit::Hearts),
        J()
    };
    auto fr2 = isFlush(f2);
    ASSERT_EQ(fr2.suit, Suit::Hearts);
    ASSERT_EQ(fr2.rank, CardRanks::Queen); // highest natural in hearts

    // No flush
    vector<Card> f3 = {
        C(CardRanks::Two,   Suit::Hearts),
        C(CardRanks::Nine,  Suit::Spades),
        C(CardRanks::Queen, Suit::Diamonds),
        C(CardRanks::Five,  Suit::Clubs),
        C(CardRanks::King,  Suit::Spades)
    };
    auto fr3 = isFlush(f3);
    ASSERT_EQ(fr3.rank, CardRanks::Invalid);
    ASSERT_EQ(fr3.suit, Suit::Invalid);
}

static void test_evaluate_and_compare() {
    // Straight Flush (player) vs Four of a Kind (dealer) → player wins
    vector<Card> sf = {
        C(CardRanks::Ten,  Suit::Spades),
        C(CardRanks::Jack, Suit::Spades),
        C(CardRanks::Queen,Suit::Spades),
        C(CardRanks::King, Suit::Spades),
        C(CardRanks::Ace,  Suit::Spades)
    };
    vector<Card> quads = {
        C(CardRanks::Nine, Suit::Hearts),
        C(CardRanks::Nine, Suit::Spades),
        C(CardRanks::Nine, Suit::Clubs),
        C(CardRanks::Nine, Suit::Diamonds),
        C(CardRanks::Two,  Suit::Clubs)
    };
    auto ev_player_sf = evaluateHand(sf);
    auto ev_dealer_quads = evaluateHand(quads);
    ASSERT_EQ(ev_player_sf.first, HandRanks::StraightFlush);
    ASSERT_EQ(ev_dealer_quads.first, HandRanks::FourKind);
    {
        int cmp = compareHand(ev_dealer_quads, ev_player_sf); // dealer vs player
        ASSERT_TRUE(cmp == 1); // player wins
    }

    // --- NEW: Dealer wins exact ties ---
    // Dealer: Pair of Kings (low hand), Player: Pair of Kings (low hand)
    vector<Card> dealer_tie = { C(CardRanks::King, Suit::Hearts), C(CardRanks::King, Suit::Clubs) };
    vector<Card> player_tie = { C(CardRanks::King, Suit::Spades), C(CardRanks::King, Suit::Diamonds) };
    auto d_tie = evaluateHand(dealer_tie);
    auto p_tie = evaluateHand(player_tie);
    ASSERT_EQ(d_tie.first, HandRanks::Pair);
    ASSERT_EQ(p_tie.first, HandRanks::Pair);
    ASSERT_EQ(d_tie.second, CardRanks::King);
    ASSERT_EQ(p_tie.second, CardRanks::King);
    {
        int cmp = compareHand(d_tie, p_tie);
        ASSERT_TRUE(cmp == -1); // dealer wins ties
    }

    // --- NEW: Same rank, dealer higher kicker → dealer wins ---
    // Dealer: Pair of Queens, Player: Pair of Jacks
    vector<Card> dealer_higher = { C(CardRanks::Queen, Suit::Spades), C(CardRanks::Queen, Suit::Hearts) };
    vector<Card> player_lower  = { C(CardRanks::Jack,  Suit::Spades), C(CardRanks::Jack,  Suit::Hearts) };
    auto d_hi = evaluateHand(dealer_higher);
    auto p_lo = evaluateHand(player_lower);
    ASSERT_EQ(d_hi.first, HandRanks::Pair);
    ASSERT_EQ(p_lo.first, HandRanks::Pair);
    {
        int cmp = compareHand(d_hi, p_lo);
        ASSERT_TRUE(cmp == -1); // dealer's pair (Q) > player's pair (J)
    }

    // --- NEW: Same rank, player higher kicker → player wins ---
    // Dealer: Pair of Tens, Player: Pair of Jacks
    vector<Card> dealer_lower = { C(CardRanks::Ten,   Suit::Spades), C(CardRanks::Ten,   Suit::Hearts) };
    vector<Card> player_higher= { C(CardRanks::Jack,  Suit::Spades), C(CardRanks::Jack,  Suit::Hearts) };
    auto d_lo = evaluateHand(dealer_lower);
    auto p_hi = evaluateHand(player_higher);
    ASSERT_EQ(d_lo.first, HandRanks::Pair);
    ASSERT_EQ(p_hi.first, HandRanks::Pair);
    {
        int cmp = compareHand(d_lo, p_hi);
        ASSERT_TRUE(cmp == 1); // player wins (J > T)
    }
}

// --- registry of tests ---
using TestFn = void(*)();
struct NamedTest { const char* name; TestFn fn; };

static NamedTest kTests[] = {
    {"low_hand",            &test_low_hand},
    {"of_a_kind",           &test_ofAKind},
    {"straight",            &test_straight},
    {"flush",               &test_flush},
    {"eval_compare",        &test_evaluate_and_compare},
};

static void run_one(const NamedTest& t) {
    std::cout << "[ RUN      ] " << t.name << "\n";
    t.fn(); // your ASSERT_* will exit(1) on failure
    std::cout << "[       OK ] " << t.name << "\n";
}

int main(int argc, char** argv) {
    if (argc > 1) {
        // run only the named test
        std::string name = argv[1];
        for (auto& t : kTests) {
            if (name == t.name) { run_one(t); std::cout << "All tests passed!\n"; return 0; }
        }
        std::cerr << "Unknown test: " << name << "\n";
        return 2;
    }
    // run all
    for (auto& t : kTests) run_one(t);
    std::cout << "All tests passed!\n";
    return 0;
}

