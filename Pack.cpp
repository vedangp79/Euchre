// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <array>
#include "Pack.h"
#include "Card.h"
#include <vector>

using namespace std;


// EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on.
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
Pack::Pack() {

    int i = 0;
    next = 0;
    for(int s = SPADES; s <= DIAMONDS; ++s) {

        Suit suit = static_cast<Suit>(s);

        for(int r = NINE; r<= ACE; ++r) {
        Rank rank = static_cast<Rank>(r);
        Card c(rank, suit);
        cards[i] = c;
        i++;
        }
    }
}

// REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
Pack::Pack(std::istream& pack_input) {

    Rank rank;
    Suit suit;
    next = 0;
    
    for(int i = 0; i < PACK_SIZE; ++i){
        pack_input >> rank;
        string ofStr;
        pack_input >> ofStr >> suit;

        Card c(rank, suit);
        cards[i] = c;
    }
}

// REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
Card Pack::deal_one() {
    assert(next < PACK_SIZE);
    next = next + 1;
    int index = next - 1;
    return cards[index];
}

// EFFECTS: Resets next index to first card in the Pack
void Pack::reset() {
  next = 0;
}

  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
void Pack::shuffle() {

    Card shuffled[PACK_SIZE];

    for(int k = 0; k < 7; ++k){

        int j = 0;
        for(int i = 0; i < (PACK_SIZE/2); ++i) {
            shuffled[j++] = cards[i + (PACK_SIZE/2)];
            shuffled[j++] = cards[i];
        }

        for (int i = 0; i < PACK_SIZE; ++i) {
            cards[i] = shuffled[i];
        }
    }
    reset();
}

// EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const {
    if(next >= PACK_SIZE){
        return true;
    }
    else{
        return false;
    }
}
