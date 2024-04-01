// Project UID 1d9f47bfc76643019cfbf037641defe1
#include <cassert>
#include <iostream>
#include <algorithm>
#include <string>
#include "Player.h"
#include "Card.h"

using namespace std;

class SimplePlayer : public Player {

public:

   SimplePlayer(string name_in) {
    name = name_in;
    num_cards = 0;
   }

  //EFFECTS returns player's name
  virtual const std::string & get_name() const {
    return name;
  }

  //REQUIRES player has less than MAX_HAND_SIZE cards
  //EFFECTS  adds Card c to Player's hand
  virtual void add_card(const Card &c) {
    assert(num_cards < MAX_HAND_SIZE);

    hand.push_back(c);
    num_cards++;

  }

  //REQUIRES round is 1 or 2
  //MODIFIES order_up_suit
  //EFFECTS If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.
  virtual bool make_trump(const Card &upcard, bool is_dealer,
                        int round, Suit &order_up_suit) const {

    assert(round == 1 || round == 2);

    int num_face_trump = 0;
    Suit trump = upcard.get_suit();
    Suit next_suit = Suit_next(trump);

    if (round == 1) {
        for(int i = 0; i < num_cards; ++i) {
            if(hand[i].is_trump(trump) && hand[i].is_face_or_ace()) {
                num_face_trump++;
            }
        }
        if(num_face_trump >= 2) {
            order_up_suit = trump;
            return true;
        }
        return false;
    }

    // round == 2
    if(is_dealer == false) {
        for(int i = 0; i < num_cards; ++i) {
            if(hand[i].is_trump(next_suit) && hand[i].is_face_or_ace()) { 
                num_face_trump++;
            }
        }
        if(num_face_trump >= 1) {
            order_up_suit = next_suit;
            return true;
        }
        return false;
    }

    // screw the dealer
    order_up_suit = next_suit;
    return true;
}

//REQUIRES Player has at least one card
//EFFECTS  Player adds one card to hand and removes one card from hand.
  virtual void add_and_discard(const Card &upcard) {
    assert(num_cards >= 1);

    int low_card = 0;

    hand.push_back(upcard);
    num_cards++;

    for(int i = 0; i < num_cards; ++i) {
        if(Card_less(hand[i], hand[low_card], upcard.get_suit()))
            low_card = i;

    }

    hand.erase(hand.begin() + low_card);
    num_cards--;
  }

  //REQUIRES Player has at least one card
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.
   virtual Card lead_card(Suit trump) {
    assert(num_cards >= 1);

    int trump_count = 0;
    Card low_card = Card();
    if (trump == SPADES)
	{
		low_card = Card(TWO, HEARTS);
	}
    int high_card = 0;

    for(int i = 0; i < num_cards; ++i) {
        if (hand[i].is_trump(trump))
			{
				trump_count++;
			}
    }
    if(trump_count == num_cards) { //all cards are trumps
        for(int i = 0; i < num_cards; ++i) {
            if(Card_less(low_card, hand[i], trump)){
                low_card = hand[i];
                high_card = i;
            }
        }
    Card lead = hand[high_card];
    hand.erase(hand.begin() + high_card);
    num_cards--;
    return lead;
    }

    for(int i = 0; i < num_cards; ++i) {
        if(Card_less(low_card, hand[i], trump)) {

            if(hand[i].get_suit(trump) != trump) {
                low_card = hand[i];
                high_card = i;
            }
        }
    }
    Card lead = hand[high_card];
    hand.erase(hand.begin() + high_card);
    num_cards--;
    return lead;

} 


  //REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  The card is removed from the player's hand.
  virtual Card play_card(const Card &led_card, Suit trump) {
    assert(num_cards >= 1);

    int led_count = 0;
    int index = 0;

    Card low_card = Card();
	if (trump == SPADES)
	{
		low_card = Card(TWO, HEARTS);
	}

    for(int i = 0; i < num_cards; ++i) {
        if (hand[i].get_suit(trump) == led_card.get_suit(trump))
			{
				led_count++;
			}
    }

   
    if(led_count == 0) {
        for(int i = 0; i < num_cards; ++i) {
            if(Card_less(hand[i], hand[index], trump))
                index = i;
        }
        Card play = hand[index];
        hand.erase(hand.begin() + index);
        num_cards--;
        return play;
    }

    for(int i = 0; i < num_cards; ++i) {
            if(Card_less(low_card, hand[i], trump)) {

                if(hand[i].get_suit(trump) == led_card.get_suit(trump)) { 
                    low_card = hand[i];
                    led_count = i;
                }
            }
        }
    index = led_count;
    Card play = hand[index];
    hand.erase(hand.begin() + index);
    num_cards--;
    return play;
  }

  private:
  string name;
  int num_cards = 0;
  vector <Card> hand;
};

class HumanPlayer : public Player {

    public:

    HumanPlayer(string name_in) {
        name = name_in;
    }

    void print_hand() const {
        for (size_t i=0; i < hand.size(); ++i) {
            cout << "Human player " << name << "'s hand: "
             << "[" << i << "] " << hand[i] << "\n";
        }
    }

    //EFFECTS returns player's name
  virtual const std::string & get_name() const {
    return name;
  }

  //REQUIRES player has less than MAX_HAND_SIZE cards
  //EFFECTS  adds Card c to Player's hand
  virtual void add_card(const Card &c) {
    assert(hand.size() < MAX_HAND_SIZE);

    hand.push_back(c);
    sort(hand.begin(), hand.end());
    num_cards++;
  }

  //REQUIRES round is 1 or 2
  //MODIFIES order_up_suit
  //EFFECTS If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.
  virtual bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const {
                            assert(round == 1 || round == 2);
                            print_hand();
                            cout << "Human player " << name 
                            << ", please enter a suit, or \"pass\":\n";

                            string decision;
                            cin >> decision;

                            if (decision != "pass") {
                                order_up_suit = string_to_suit(decision);
                                return true;
                            }
                            else {
                                return false;
                            }       
                          }

  //REQUIRES Player has at least one card
  //EFFECTS  Player adds one card to hand and removes one card from hand.
  virtual void add_and_discard(const Card &upcard) {
    assert(num_cards >= 1);

    print_hand();
    int discard;
    int i;
    cout << "Discard upcard: [-1]\n";
    cout << "Human player " << name << ", please select a card to discard:\n";
    cin >> discard;
    int index = 0;

    if(discard != -1) {
        for(i = 0; i < hand.size(); ++i) {
            if(hand[discard] == hand[i]){
                index = i;
            }
        }
       hand[index] = upcard;
    }

  }

  //REQUIRES Player has at least one card
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.
  virtual Card lead_card(Suit trump) {
    assert(num_cards >= 1);

    print_hand();
    int discard;
    cout << "Human player " << name << ", please select a card:\n";
    cin >> discard;
    Card lead = hand[discard];
    hand.erase(hand.begin() + discard);
    num_cards--;
    return lead;
  }

  //REQUIRES Player has at least one card
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  The card is removed from the player's hand.
  virtual Card play_card(const Card &led_card, Suit trump) {
    assert(num_cards >= 1);

    print_hand();
    int discard;
    cout << "Human player " << name << ", please select a card:\n";
    cin >> discard;
    Card play = hand[discard];
    hand.erase(hand.begin() + discard);
    num_cards--;
    return play;
  }

  private:
  string name;
  int num_cards = 0;
  vector <Card> hand;
};

Player * Player_factory(const std::string &name, const std::string &strategy) {

   if (strategy == "Simple") {
    return new SimplePlayer(name);
  }
  else if (strategy == "Human") {
    return new HumanPlayer(name);
  }
  else{
    assert(false);
  return nullptr;
  }

}


std::ostream & operator<<(std::ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}