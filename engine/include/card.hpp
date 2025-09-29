// ----- Card Enums -----
enum class HandRanks { High = 0, Pair, TwoPair, ThreeKind, Straight, Flush, FullHouse, FourKind, StraightFlush, FiveAce, Invalid = -1};

enum class CardRanks { Two = 2, Three, Four, Five, Six, Seven,  Eight, Nine,  Ten, Jack, Queen, King, Ace, Invalid = -1};

enum class Suit { Spades, Hearts, Diamonds, Clubs, Invalid = -1 };

// ----- Card Class -----
struct Card {
    CardRanks rank;   // 2..14 (where 11=J, 12=Q, 13=K, 14=A)
    Suit suit;   // 0=♠, 1=♥, 2=♦, 3=♣ ; use -1 if Joker
    bool joker; // true = Joker card

     // Constructor for normal cards
    Card(Suit s, CardRanks r) : rank(r), suit(s), joker(false) {} 

    // Constructor for jokers
    Card() : rank(CardRanks::Invalid), suit(Suit::Invalid), joker(true) {}
};