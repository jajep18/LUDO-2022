#include "my_player.h"
#include <iostream>
#include <exception>

// struct positions{

//         int pos[16];
//         positions() {}
//     };

my_player::my_player(Q_Table q_table_, double learning_rate = 0.10){

    // Assign Q-table
    q_table = &q_table_;
    set_learning_rate(learning_rate);

    // Assign random generator
    std::random_device rd;
    generator = std::mt19937(rd());

}


my_player::~my_player(){  }

positions my_player::move_to_start(int piece, positions positions_){
    positions new_positions = positions_;
    if(position[piece] == -1 && dice == 6){
        new_positions.pos[piece] = 0;
        send_pieces_home(0);
        return new_positions;
    }
    else
        throw std::exception();
    
}

void my_player::increment_pieces_in_goal(){
    if (pieces_in_goal < 4){
        pieces_in_goal++;
    }
}

int my_player::is_star_tile(int tile)
{
    switch(tile) // Check for star
    {
    case 5:  return 6;
    case 18: return 6;
    case 31: return 6;
    case 44: return 6;

    case 11: return 7;
    case 24: return 7;
    case 37: return 7;

    case 50: return 6; //jump to goal

    default: return 0;
    }

}

bool my_player::is_globe_tile(int tile){
    switch (tile)
    {
    case 8:
        return 1;
    case 21:
        return 1;
    case 34:
        return 1;
    case 47:
        return 1;
    
    default:
        return 0;
    }
}

int my_player::opponents_on_pos(int tile){
    int opponents = 0;

    for (int i = 4; i < 16; i++){
        if (post_move_position[i] == tile)
            opponents++;   

    }
    return opponents;

}

void my_player::set_learning_rate(double value){
    if(value < 0.0 || value > 1.0)
        throw std::exception();
    learning_rate = value;
}

void my_player::send_pieces_home(int tile){

    for (int i = 4; i < 16; i++){
        if (post_move_position[i] == tile)
            post_move_position[i] = -1;

    }

}



positions my_player::get_post_move_pos(int piece_moving){

    positions post_move_pos;

    for(int i = 0; i < 16; i++){
        post_move_pos.pos[i] = position[i];
    }

    int piece_pos = position[piece_moving];

    if( piece_pos == -1){
       post_move_pos = move_to_start(piece_moving,post_move_pos); // Move piece to start if possible
    }

    else{
        int next_pos = piece_pos + dice;
        next_pos += is_star_tile(next_pos);

        if(next_pos == 56)
            post_move_pos.pos[piece_moving] = 99;
        else if(next_pos > 50 && next_pos < 56)
            post_move_pos.pos[piece_moving] = next_pos;
        else if(next_pos > 55)
            post_move_pos.pos[piece_moving] = 112 - next_pos;
        else if (next_pos > 0 && next_pos < 51)
        {
            int opponents = opponents_on_pos(next_pos);

            if(opponents == 0)
                post_move_pos.pos[piece_moving] = next_pos;
            else if(opponents == 1)
            {
                if(is_globe_tile(next_pos))
                    post_move_pos.pos[piece_moving] = -1;
                else
                {
                    post_move_pos.pos[piece_moving] = next_pos;
                    send_pieces_home(next_pos);
                }
            }
            else
                post_move_pos.pos[piece_moving] = -1;
        }
        else
            throw std::exception();   
    }

    return post_move_pos;

}

int my_player::make_decision()
{
//***************************************************************
//    Find available for piece moves in the current position    

    int available_moves[4];     //Vector of available moves 
    int available_count = 0;    //Number of available moves

    for(int i = 0; i < 4; i++) 
    {
        if(is_valid_move(i)) // Checks if piece has valid moves
        {
            available_moves[available_count] = i;
            available_count++;
        }
    }

    // Move according to no available moves
    if(available_count == 0) return -1;
    // Move according to only available move
    if(available_count == 1) return available_moves[0];

//***************************************************************
//    Find best available moves in the current position    

    double best_val = -1;
    int best_val_count = 0;
    int best_moves[4];
    positions best_pos;

    for (int i = 0; i < available_count; i++)
    {
        int piece_current = available_moves[i];
        best_pos = get_post_move_pos(piece_current); 
        double q_val = q_table->get_value(get_q_idx()); 

        if (q_val > best_val){
            best_val = q_val;
            best_moves[0] = piece_current;
            best_val_count = 1;
        }
        else if(q_val == best_val){
            best_moves[best_val_count] = piece_current;
            best_val_count++;
        }
    }

    // Move according to best available move
    if(available_count == 1) return best_moves[0];

    // Something went wrong
    if(available_count < 1) throw std::exception();

    // Move accordingly randowmly selected best move
    distribution = std::uniform_int_distribution<int>(0, best_val_count - 1);
    return best_moves[distribution(generator)];
}

int my_player::get_q_idx(){

    //Q-table index variables 
    // Used to check own pieces //
    int in_goal_score               = 0;
    int safe_score                  = 0; // Pieces safe from enemys   
    int in_danger_score             = 0; //6 or less pieces in front of an enemy
    int within_goal_range_score     = 0; //Number of pieces able to get to the goal
    int unproctected__score         = 0; //Number of pieces not on a globe
    // Used to check if ahead or behind opponents //
    int best_opponet_score          = 0; //Score of best opponent 
    int second_opponet_score        = 0; //Score of 2nd best opponent   

    //Check if you already won
    if(pieces_in_goal == 4){
        return Q_Table::WON_STATE;
    }
    // PIP = Pieces In Play
    int n_PIP = 4 - pieces_in_goal;
    int PIP_unprotected = get_unprotected_PIP(n_PIP);

    int opponent_1_PIP = calc_opponent_PIP(1);
    int opponent_2_PIP = calc_opponent_PIP(2);
    int opponent_3_PIP = calc_opponent_PIP(3);
    int best_opponent = get_lowest(opponent_1_PIP, opponent_2_PIP, opponent_3_PIP);
    int second_opponent = get_middle(opponent_1_PIP, opponent_2_PIP, opponent_3_PIP);
    int pip_diff = second_opponent - best_opponent;
    int diff_divider = 7;
    
    //Assigning the Q-table scores
    in_goal_score = pieces_in_goal;



}

int my_player::get_unprotected_PIP(int n_PIP){

    int total = 0;

    for(int i = 0; i < 4; i++)
    {
        int square = post_move_position[i];
        if(is_globe_tile(square) == false)
            total++;
    }

    return total;

}



