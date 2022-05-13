#include "iplayer.h"
#include "my_q_table.h"
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
    int* predicted_move_position[16];

    double learning_rate = 0.10;
    double discount_factor = 0.80;
    int move_count = 0;
    bool learning_on = true;
    int pieces_in_goal = 0;

    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;

    int* move_to_start(int piece, int* positions);

    // Helper functions
    int star_tile_move(int tile); //Checks if special tile and returns new location
    bool is_globe_tile(int tile); 
    bool send_pieces_home(int tile); //Sends all pieces on tile home
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

    double get_learning_rate();
    double get_discount_factor();


private:
    int predict_action(int piece);
    int make_decision();

    int get_q_idx(std::vector<int> states, int action);

    int calc_opponent_PIP(positions positions, int player);

    int get_lowest(int a, int b, int c);
    int get_middle(int a, int b, int c);

    int get_cozy_pieces(int* positions); 
    int get_safe_pieces(int* positions);
    int get_scared_pieces(int* positions);
    int get_goal_pieces(int* positions);
    int get_unsafe_pieces(int* positions);

    bool check_grouping(int tile);

    std::vector<int> get_states(int* position);//Returns states for all 4 pieces for the given positions

};




