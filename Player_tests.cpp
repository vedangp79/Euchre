// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_player_insertion) { //from public tests
  // Create a Human player
  Player * human = Player_factory("NotRobot", "Human");

  // Print the player using the stream insertion operator
  ostringstream oss1;
  oss1 << * human;

  // Verify that the output is the player's name
  ASSERT_EQUAL(oss1.str(), "NotRobot");

  // Create a Simple player
  Player * alice = Player_factory("Alice", "Simple");

  // Print the player using the stream insertion operator
  ostringstream oss2;
  oss2 << *alice;
  ASSERT_EQUAL(oss2.str(), "Alice");

  // Clean up players that were created using Player_factory()
  delete human;
  delete alice;
}

TEST(test_simple_player_make_trump1) { //1 round, dealer, orders up
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(TEN, HEARTS));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(JACK, SPADES));
  bob->add_card(Card(ACE, CLUBS));

  // Bob makes trump
  Card nine_spades(NINE, SPADES);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_spades,    // Upcard
    true,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, SPADES);

  delete bob;
}

TEST(test_simple_player_make_trump2) { // 1 round, dealer, does not order up
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, HEARTS));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(JACK, SPADES));
  bob->add_card(Card(ACE, CLUBS));

  // Bob makes trump
  Card nine_diamonds(NINE, DIAMONDS);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_diamonds,    // Upcard
    true,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_FALSE(orderup);

  delete bob;
}

TEST(test_simple_player_make_trump3) { // 1 round, not dealer, does not order up
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, HEARTS));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(JACK, SPADES));
  bob->add_card(Card(ACE, CLUBS));

  // Bob makes trump
  Card nine_diamonds(NINE, DIAMONDS);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_diamonds,    // Upcard
    false,           // Bob is not the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_FALSE(orderup);

  delete bob;
}

TEST(test_simple_player_make_trump4) { // 2 round, not dealer, orders up next_suit
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(QUEEN, HEARTS));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, HEARTS));
  bob->add_card(Card(ACE, CLUBS));

  // Bob makes trump
  Card nine_diamonds(NINE, DIAMONDS);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_diamonds,    // Upcard
    false,           // Bob is not the dealer
    2,              // Second round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, HEARTS);

  delete bob;
}

TEST(test_simple_player_make_trump5) { // 2 round, dealer, screw the dealer
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(FIVE, SPADES));
  bob->add_card(Card(TEN, HEARTS));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(NINE, HEARTS));
  bob->add_card(Card(ACE, CLUBS));

  // Bob makes trump
  Card nine_diamonds(NINE, DIAMONDS);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_diamonds,    // Upcard
    true,           // Bob is the dealer
    2,              // Second round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, HEARTS);

  delete bob;
}

TEST(test_simple_player_make_trump6) { // 1 round, not dealer, less cards
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(ACE, DIAMONDS));
  bob->add_card(Card(JACK, HEARTS));

  // Bob makes trump
  Card nine_diamonds(NINE, DIAMONDS);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_diamonds,    // Upcard
    false,           // Bob is not the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, DIAMONDS);

  delete bob;
}

TEST(test_simple_player_lead_card1) { // highest non trump card
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob adds a card to his hand and discards one card
  bob->add_and_discard(
    Card(NINE, HEARTS) // upcard
  );

  // Bob leads
  Card card_led = bob->lead_card(HEARTS);

  // Verify the card Bob selected to lead
  Card ace_spades(ACE, SPADES);
  ASSERT_EQUAL(card_led, ace_spades); //check led card

  delete bob;
}

TEST(test_simple_player_lead_card2) { // only trumps, plays highest trump
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(JACK, CLUBS));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob adds a card to his hand and discards one card
  bob->add_and_discard(
    Card(JACK, SPADES) // upcard
  );

  // Bob leads
  Card card_led = bob->lead_card(SPADES);

  // Verify the card Bob selected to lead
  Card jack_spades(JACK, SPADES);
  ASSERT_EQUAL(card_led, jack_spades); //check led card

  delete bob;
}

TEST(test_simple_player_lead_card3) { // plays highest non-trump card, 4 cards
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(ACE, SPADES));
  bob->add_card(Card(KING, CLUBS));
  bob->add_card(Card(ACE, HEARTS));
  bob->add_card(Card(JACK, DIAMONDS));

  // Bob adds a card to his hand and discards one card
  bob->add_and_discard(
    Card(JACK, HEARTS) // upcard
  );

  // Bob leads
  Card card_led = bob->lead_card(HEARTS);

  // Verify the card Bob selected to lead
  Card ace_spades(ACE, SPADES);
  ASSERT_EQUAL(card_led, ace_spades); //check led card

  delete bob;
}

TEST(test_simple_player_lead_card4) { // plays highest non-trump card, 4 cards
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(TEN, HEARTS));
  bob->add_card(Card(QUEEN, CLUBS));
  bob->add_card(Card(KING, CLUBS));
  bob->add_card(Card(NINE, HEARTS));

  // Bob leads
  Card card_led = bob->lead_card(HEARTS);

  // Verify the card Bob selected to lead
  Card king_clubs(KING, CLUBS);
  ASSERT_EQUAL(card_led, king_clubs); //check led card

  delete bob;
}

TEST(test_simple_player_play_card1) { // plays lowest card in hand 
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob plays a card
  Card nine_diamonds(NINE, DIAMONDS);
  Card card_played = bob->play_card(
    nine_diamonds,  // Nine of Diamonds is led
    HEARTS    // Trump suit
  ); 

  // Verify the card Bob played
  ASSERT_EQUAL(card_played, Card(NINE, SPADES));
  delete bob;
}

TEST(test_simple_player_play_card2) { // plays highest card that follows suit
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(JACK, HEARTS));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, DIAMONDS));

  // Bob plays a card
  Card nine_diamonds(NINE, DIAMONDS);
  Card card_played = bob->play_card(
    nine_diamonds,  // Nine of Diamonds is led
    CLUBS    // Trump suit
  ); 

  // Verify the card Bob played
  ASSERT_EQUAL(card_played, Card(ACE, DIAMONDS));
  delete bob;
}

TEST(test_simple_player_play_card3) { // led card and trump card same suit
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(JACK, HEARTS));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, DIAMONDS));

  // Bob plays a card
  Card nine_diamonds(NINE, DIAMONDS);
  Card card_played = bob->play_card(
    nine_diamonds,  // Nine of Diamonds is led
    DIAMONDS    // Trump suit
  ); 

  // Verify the card Bob played
  ASSERT_EQUAL(card_played, Card(JACK, HEARTS));
  delete bob;
}

TEST(test_add_discard1) { // led card and trump card same suit
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(TEN, HEARTS));
  bob->add_card(Card(QUEEN, HEARTS));
  bob->add_card(Card(JACK, DIAMONDS));
  bob->add_card(Card(KING, HEARTS));
  bob->add_card(Card(ACE, HEARTS));

   bob->add_and_discard(
    Card(NINE, HEARTS) // upcard, wont pick up
  );

  // Bob plays a card
  Card ten_diamonds(TEN, DIAMONDS);
  Card card_played = bob->play_card(
    ten_diamonds,  // Ten of Diamonds is led
    HEARTS    // Trump suit
  ); 

  // Verify the card Bob played
  ASSERT_EQUAL(card_played, Card(TEN, HEARTS));

  bob->add_and_discard(
    Card(JACK, HEARTS) // upcard, discards q.hearts
  );

  // Bob plays a card
  Card nine_hearts(NINE, HEARTS);
  card_played = bob->play_card(
    nine_hearts,  // Nine of hearts is led
    HEARTS    // Trump suit
  ); 

  // Verify the card Bob played
  ASSERT_EQUAL(card_played, Card(JACK, HEARTS));

  bob->add_and_discard(
    Card(NINE, HEARTS) // upcard, wont pick up
  );
  // should have j.diamonds, k.hearts, a.hearts 

  // Bob plays a card
  
   card_played = bob->play_card(
    nine_hearts,  // Nine of diamonds is led
    CLUBS    // Trump suit
  ); 

  // Verify the card Bob played
  ASSERT_EQUAL(card_played, Card(ACE, HEARTS));
  delete bob;

}
TEST_MAIN()
