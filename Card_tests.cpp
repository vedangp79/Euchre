// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {

    Card two_spades = Card();
    ASSERT_EQUAL(two_spades.get_rank(), TWO);
    ASSERT_EQUAL(two_spades.get_suit(), SPADES);

    Card c(JACK, DIAMONDS);
    ASSERT_EQUAL(JACK, c.get_rank());
    ASSERT_EQUAL(DIAMONDS, c.get_suit());
}

TEST(test_card_suit_and_rank) {

    Card jack_spades = Card(JACK, SPADES);
    ASSERT_EQUAL(jack_spades.get_rank(), JACK);
    ASSERT_EQUAL(jack_spades.get_suit(), SPADES);
    ASSERT_EQUAL(jack_spades.get_suit(CLUBS), CLUBS);

    Card five_hearts = Card(FIVE, HEARTS);
    ASSERT_EQUAL(five_hearts.get_suit(DIAMONDS), HEARTS);
}

TEST(test_card_type) {
    Card jack_spades = Card(JACK, SPADES);
    ASSERT_TRUE(jack_spades.is_face_or_ace());
    ASSERT_TRUE(jack_spades.is_right_bower(SPADES));
    ASSERT_TRUE(jack_spades.is_left_bower(CLUBS));
    ASSERT_TRUE(jack_spades.is_trump(SPADES));

    Card ace_hearts = Card(ACE, HEARTS);
    ASSERT_TRUE(ace_hearts.is_face_or_ace());
    ASSERT_FALSE(ace_hearts.is_right_bower(CLUBS));
    ASSERT_FALSE(ace_hearts.is_left_bower(CLUBS));
    ASSERT_FALSE(ace_hearts.is_trump(CLUBS));

    Card two_diamonds = Card(TWO, DIAMONDS);
    ASSERT_FALSE(two_diamonds.is_face_or_ace());
    ASSERT_FALSE(two_diamonds.is_right_bower(DIAMONDS));
    ASSERT_FALSE(two_diamonds.is_left_bower(HEARTS));
    ASSERT_TRUE(two_diamonds.is_trump(DIAMONDS));
}

TEST(test_card_self_comparison) {
    Card three_hearts = Card(THREE, HEARTS);
    ASSERT_FALSE(three_hearts < three_hearts);
    ASSERT_TRUE(three_hearts <= three_hearts);
    ASSERT_FALSE(three_hearts > three_hearts);
    ASSERT_TRUE(three_hearts >= three_hearts);
    ASSERT_TRUE(three_hearts == three_hearts);
    ASSERT_FALSE(three_hearts != three_hearts);
}

TEST(test_card_comparison) {
    Card three_hearts = Card(THREE, HEARTS);
    Card three_diamonds = Card(THREE, DIAMONDS);
    Card ace_spades = Card(ACE, SPADES);
    Card eight_hearts = Card(EIGHT, HEARTS);

    ASSERT_FALSE(three_diamonds < three_hearts);
    ASSERT_TRUE(three_hearts <= three_diamonds);
    ASSERT_FALSE(three_hearts > eight_hearts);
    ASSERT_TRUE(ace_spades >= three_hearts);
    ASSERT_FALSE(ace_spades == three_hearts);
    ASSERT_TRUE(ace_spades != eight_hearts);
}

TEST(test_Suit_next) {
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
    ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
    ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
    ASSERT_EQUAL(Suit_next(DIAMONDS), HEARTS);
}

TEST(test_Card_less) {
    Card seven_hearts = Card(SEVEN, HEARTS);
    Card seven_diamonds = Card(SEVEN, DIAMONDS);
    Card jack_diamonds = Card(JACK, DIAMONDS);
    Card jack_hearts = Card(JACK, HEARTS);
    Card ace_spades = Card(ACE, SPADES);
    Card king_hearts = Card(KING, HEARTS);
    ASSERT_FALSE(Card_less(seven_hearts, seven_diamonds, jack_diamonds,
                           HEARTS));
    ASSERT_TRUE(Card_less(seven_hearts, seven_diamonds, jack_diamonds,
                           CLUBS));
    ASSERT_TRUE(Card_less(seven_hearts, seven_diamonds, ace_spades,
                           CLUBS));
    ASSERT_TRUE(Card_less(king_hearts, ace_spades, ace_spades,
                           CLUBS));
    ASSERT_FALSE(Card_less(king_hearts, seven_hearts, ace_spades,
                           HEARTS));
    ASSERT_FALSE(Card_less(king_hearts, seven_hearts, seven_hearts,
                           CLUBS));
    ASSERT_FALSE(Card_less(jack_diamonds, seven_diamonds, ace_spades,
                           DIAMONDS));
    ASSERT_TRUE(Card_less(jack_hearts, jack_diamonds, king_hearts,
                           DIAMONDS));
    ASSERT_FALSE(Card_less(jack_hearts, jack_diamonds, ace_spades,
                           HEARTS));
    ASSERT_TRUE(Card_less(king_hearts, jack_diamonds, jack_hearts,
                           HEARTS));
    ASSERT_TRUE(Card_less(ace_spades, jack_diamonds, ace_spades,
                           HEARTS));

    ASSERT_TRUE(Card_less(seven_diamonds, ace_spades, CLUBS));
    ASSERT_TRUE(Card_less(ace_spades, jack_diamonds, HEARTS));
    ASSERT_TRUE(Card_less(king_hearts, jack_diamonds, HEARTS));
    ASSERT_FALSE(Card_less(seven_diamonds, seven_hearts, CLUBS));
    ASSERT_FALSE(Card_less(seven_diamonds, seven_hearts, DIAMONDS));
    ASSERT_FALSE(Card_less(king_hearts, seven_hearts, HEARTS));
    ASSERT_FALSE(Card_less(jack_hearts, jack_hearts, HEARTS));

}
//have to do more from here
TEST(test_Card_less_self) {
    Card ten_spades = Card(TEN, SPADES);
    ASSERT_FALSE(Card_less(ten_spades, ten_spades, CLUBS));
    ASSERT_FALSE(Card_less(ten_spades, ten_spades, SPADES));
    ASSERT_FALSE(Card_less(ten_spades, ten_spades, ten_spades,
                           CLUBS));
    ASSERT_FALSE(Card_less(ten_spades, ten_spades, ten_spades,
                           HEARTS));
}

TEST(test_card_insertion) {
    Card six_hearts = Card(SIX, HEARTS);
    ostringstream oss;
    oss << six_hearts;
    ASSERT_EQUAL(oss.str(), "Six of Hearts");

    Card jack_clubs = Card(JACK, CLUBS);
    ostringstream os;
    os << jack_clubs;
    ASSERT_EQUAL(os.str(), "Jack of Clubs");
}

TEST(test_card_extraction) {
    istringstream iss("Queen of Spades");
    Card c;
    iss >> c;
    ASSERT_EQUAL(c, Card(QUEEN, SPADES));

    istringstream is("Two of Diamonds");
    is >> c;
    ASSERT_EQUAL(c, Card(TWO, DIAMONDS));
}

// Add more test cases here

TEST_MAIN()
