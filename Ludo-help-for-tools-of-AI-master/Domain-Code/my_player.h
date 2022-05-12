#include "iplayer.h"
#include "../AI_player/q_table.h"
#include "positions_and_dice.h"

#include "random"

struct positions{

    int pos[16];
    positions() {}
};

class my_player : public iplayer
{
private:
    int my_players_privates;

    Q_Table* q_table;
    int post_move_position[16];

    double learning_rate = 0.10;
    int move_count = 0;
    bool learning_on = true;
    int pieces_in_goal = 0;



    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;

    positions move_to_start(int piece, positions positions);

    // Helper functions
    int is_star_tile(int tile); //Checks if special tile and returns new location
    bool is_globe_tile(int tile); 
    void send_pieces_home(int tile); //Sends all pieces on tile home
    int opponents_on_pos(int tile);
    
    int get_unprotected_PIP(int n_PIP);
    int get_opponent_PIP(int opponent);

    // Q-learning
   // int get_q_idx();
    

public:

    my_player(Q_Table& table);
    my_player(Q_Table q_table, double learning_rate = 0.10);
    ~my_player();

    void increment_pieces_in_goal();
    
    void set_learning_rate(double value);


private:
    positions get_post_move_pos(int piece);
    int make_decision();

    int get_q_idx();

};



