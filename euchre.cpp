// Project UID 1d9f47bfc76643019cfbf037641defe1
#include <iostream>
#include "Player.h"
#include "Card.h"
#include "Pack.h"
#include <fstream>
#include <cstring>
#include <vector>
#include <cassert>
#include <string>

using namespace std;

class Game {
 public:
  Game(vector <Player*> player_in, Pack pack_in,
   bool choice, int target_score) {
    pack = pack_in;
    players = player_in;
    shuffle = choice;
    total_score = target_score;
  }

  void play_game() {

    int hand = 0;
    while (team_1_score < total_score && team_2_score < total_score) {
        cout << "Hand " << hand << endl;

        shuffle_pack();

        int dealer_index = hand % 4;
        cout << players[dealer_index]->get_name() << " deals" << endl;
        deal(dealer_index); //deals cards and sets upcard

        int order_up_team;
        Suit trump;
        make_trump(dealer_index, trump, order_up_team);
        play_hand(dealer_index, trump, order_up_team);
        tabulate();

        hand++;
    }

}

void delete_players() {
        for (size_t i = 0; i < players.size(); ++i) {
            delete players[i];
        }
}

private:
  vector <Player*> players;
  Pack pack;
  bool shuffle;
  Card upcard;
  int team_1_score = 0;
  int team_2_score = 0;
  int total_score = 0;

  void shuffle_pack() {
    if(shuffle == true) {
        pack.shuffle();
    }
    else {
        pack.reset();
    }
  }

  void deal(int dealer_index) { // deals cards and sets upcard

    int deal_no = 0;

    for(deal_no = 0; deal_no < 8; deal_no++) {
        int player_turn = (dealer_index + deal_no + 1) % 4;
        if(deal_no == 0 || deal_no == 2 || deal_no == 5 ||
            deal_no == 7) { //deal 3 cards
                for(int i = 0; i < 3; ++i) {
                    players[player_turn]->add_card(pack.deal_one());
                }
        }
        else { // deal 2 cards
                for(int i = 0; i < 2; ++i) {
                    players[player_turn]->add_card(pack.deal_one());
                }
        }
    }
    upcard = pack.deal_one();
    cout << upcard << " turned up" << endl;
  }

  void make_trump(int dealer_index, Suit &trump, int &order_up_team) {
    bool is_dealer;
    int round;

    for(int i = 0; i < 8; ++i) {
        
        if((i + dealer_index + 1 ) % 4 == dealer_index) {
            is_dealer = true; // player is dealer
        }
        else {
            is_dealer = false;
        }

        if(i <= 3) {
            round = 1;
        }
        else {
            round = 2;
        }
        if(players[(i + dealer_index + 1) % 4]->make_trump
        (upcard, is_dealer, round, trump) == true) {
            cout << players[(i + dealer_index + 1) % 4]->get_name();
            cout << " orders up " << trump << endl << endl;

            if(((i + dealer_index + 1) % 4) == 0 ||
            ((i + dealer_index + 1) % 4) == 2) {
                order_up_team = 1; // team 1 ordered up
            }
            else {
                order_up_team = 2; // team 2 ordered up
            }
            if(i <= 3) {
                players[dealer_index]->add_and_discard(upcard);
            }
            break;
            }
        cout << players[(i + dealer_index + 1) % 4]->get_name() << " passes" 
        << endl; 
        }
    } 

void play_hand(int dealer_index, Suit &trump, int &order_up_team) {

    int leader_index = (dealer_index + 1) % 4;; // start of new hand
    int temp_leader_index = leader_index;
    int team_1_tricks = 0;
    int team_2_tricks = 0;
    for(int i = 0; i < 5; ++i) { // 5 tricks to be played

        Card lead = players[leader_index]->lead_card(trump); // to find the lead card
        cout << lead << " led by " << players[leader_index]->get_name() << endl;
        Card highest_card = lead;

        for(int j = 0; j < 3; ++j) { // 3 players to play_card after card led
            Card played = players[(leader_index + j + 1) % 4]->play_card(lead, trump);
            cout << played << " played by " << 
            players[(leader_index + j + 1) % 4]->get_name() << endl;
            if(Card_less(highest_card, played, lead, trump) == true) { 
            // highest_card is less than the card played
                highest_card = played;
                temp_leader_index = (leader_index + j + 1) % 4;
            }
        }
        leader_index = temp_leader_index;
        cout << players[leader_index]->get_name() << " takes the trick" 
        << endl << endl;

        if (leader_index == 0 || leader_index == 2) { // team 1 got the trick
            team_1_tricks++;
        }
        else
           team_2_tricks++;
    }
    calculate(team_1_tricks , team_2_tricks, order_up_team);
}

 void calculate (int team_1_tricks, int team_2_tricks, int &order_up_team) {
  if(team_1_tricks > team_2_tricks) { // team 1 won
    cout << players[0]->get_name() << " and " << players[2]->get_name()
    << " win the hand" << endl;
    if(order_up_team == 1) { //if the first team ordered up
        if(team_1_tricks == 5) {
            cout << "march!" << endl;
            team_1_score +=2; // team 1 gets 2 points for march
        }
        else
            team_1_score++; // team 1 gets 1 point
    }
    else {
        cout << "euchred!" << endl;
        team_1_score +=2; // team 1 gets 2 points
    }
  }
  else { // if team2 won
    cout << players[1]->get_name() << " and " << players[3]->get_name()
    << " win the hand" << endl;
    if(order_up_team == 2) { // if team 2 ordered up
        if(team_2_tricks == 5) {
            cout << "march!" << endl;
            team_2_score +=2;
        }
        else
            team_2_score++;
    }
    else {
        cout << "euchred!" << endl;
        team_2_score +=2;
        }
   }
}


void tabulate () {

    cout << players[0]->get_name() << " and " << players[2]->get_name()
    << " have " << team_1_score << " points" << endl;
    cout << players[1]->get_name() << " and " << players[3]->get_name()
    << " have " << team_2_score << " points" << endl << endl;

    if (team_1_score >= total_score) { //if team 1 reached the target
        cout << players[0]->get_name() << " and " << players[2]->get_name()
    << " win!" << endl;
    }
    else if (team_2_score >= total_score) {
        cout << players[1]->get_name() << " and " << players[3]->get_name()
    << " win!" << endl;
    }
    else {}
    }

};

int main(int argc, char **argv) {

    for (int i = 0; i < argc; ++i)
	{
		cout << argv[i] << " ";
	}

	ifstream os_Pack(argv[1]);
	if (!os_Pack.is_open()) //opening pack file
	{
		cout << "Error opening " << argv[1] << endl;
        return -1;
	}
	cout << endl;

  if (argc != 12 || atoi(argv[3]) < 1 || atoi(argv[3]) > 100 ||
  (string(argv[2]) != "noshuffle" && string(argv[2]) != "shuffle") ||
  (string(argv[5]) != "Human" && string(argv[5]) != "Simple") ||
  (string(argv[7]) != "Human" && string(argv[7]) != "Simple") ||
  (string(argv[9]) != "Human" && string(argv[9]) != "Simple") ||
  (string(argv[11]) != "Human" && string(argv[11]) != "Simple")) {

  cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl; // error message
     return -1;
    }

    bool choice;
    if (std::string(argv[2]).compare("shuffle") == 0) { 
        // checking if choice == shuffle
        choice = true;
    }
    else {
        choice = false;
    }
    int target_score = atoi(argv[3]); //retrieving target score

    Player *player_0 = Player_factory(argv[4], argv[5]);
	Player *player_1 = Player_factory(argv[6], argv[7]);
	Player *player_2 = Player_factory(argv[8], argv[9]);
	Player *player_3 = Player_factory(argv[10], argv[11]);
	vector <Player*> players;
	players.push_back(player_0);
	players.push_back(player_1);
	players.push_back(player_2);
	players.push_back(player_3);

    Game *game= new Game(players, os_Pack, choice, target_score);
    game->play_game();
    game->delete_players();

    delete game; // deleting game

    os_Pack.close();
    return 0;
}