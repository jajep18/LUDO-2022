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

int* my_player::move_to_start(int piece, int* positions_){ // Possible bug due to pointers 
    int* new_positions;
    for(int i = 0; i < 16; i++){
        new_positions[i] = positions_[i];
    }
    if(position[piece] == -1 && dice == 6){
        new_positions[piece] = 0;
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

int my_player::star_tile_move(int tile)
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

int my_player::opponents_on_pos(int tile){// Possible bug due to pointers 
    int opponents = 0;

    for (int i = 4; i < 16; i++){
        if (predicted_move_position[i] == &tile)
            opponents++;   

    }
    return opponents;

}

void my_player::set_learning_rate(double value){
    if(value < 0.0 || value > 1.0)
        throw std::exception();
    learning_rate = value;
}

bool my_player::send_pieces_home(int tile){ // Possible bug due to pointers 
    bool kill = false;
    for (int i = 4; i < 16; i++){
        if (predicted_move_position[i] == &tile){
            *predicted_move_position[i] = -1;
            kill = true;
        }
    }
    return kill;
}


int my_player::predict_action(int piece_moving){ // Possible bug due to pointers 


    int action = -1;

    for(int i = 0; i < 16; i++){
        predicted_move_position[i] = &position[i];
    }

    int piece_pos = position[piece_moving];

    if( piece_pos == -1){
       *predicted_move_position = move_to_start(piece_moving, *predicted_move_position); // Move piece to start if possible
       action = Spawn;
    }

    else{ // Possible bug due to pointers 
        int next_pos = piece_pos + dice;
        next_pos += star_tile_move(next_pos);

        if(next_pos == 56){
            *predicted_move_position[piece_moving] = 99;
            action = Go_goal;
            increment_pieces_in_goal();
        }
        else if(next_pos > 50 && next_pos < 56){
            predicted_move_position[piece_moving] = &next_pos;
            action = Go_goal_zone;
        }
        else if(next_pos > 55){
            *predicted_move_position[piece_moving] = 112 - next_pos;
            action = Bounce;
        }
        else if (next_pos > 0 && next_pos < 51)
        {
            int opponents = opponents_on_pos(next_pos);

            if(opponents == 0){
                predicted_move_position[piece_moving] = &next_pos;
                action = Normal_move;
                if (star_tile_move(next_pos) > 0)
                    action = Go_star;
                if (is_globe_tile(next_pos))
                    action = Go_globe;
                if(check_grouping(next_pos))
                    action = Group_up;
            }
            else if(opponents == 1)
            {
                if(is_globe_tile(next_pos)){
                    *predicted_move_position[piece_moving] = -1;
                    action = Suicide;
                }
                else
                {
                    predicted_move_position[piece_moving] = &next_pos;
                    if(send_pieces_home(next_pos) == true){
                        action = Kill;
                    }
                }
            }
            else{
                *predicted_move_position[piece_moving] = -1;
                action = Suicide;
            }
        }
        else
            throw std::exception();   
    }
    
    return action;

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
    int action = -1;
    std::vector<int> states = get_states(position);
    std::vector<int> next_states = get_states(*predicted_move_position);

    for (int i = 0; i < available_count; i++)
    {
        int piece_current = available_moves[i];
        action = predict_action(piece_current); 
       
        double q_val = q_table->get_q_table_value(states, action); 

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
    int next_action = predict_action(best_moves[0]);
    q_table->update_q_table(states, action, next_states, learning_rate, discount_factor);

    // Move according to best available move
    if(best_val_count == 1) return best_moves[0];

    // Something went wrong
    if(best_val_count < 1) throw std::exception();

    // Move from randomly selected best moves
    distribution = std::uniform_int_distribution<int>(0, best_val_count - 1);
    return best_moves[distribution(generator)];
}


int my_player::get_cozy_pieces(int* position){

    int total = 0;
    for(int i = 0; i < 4; i++)
    {
        int tile = position[i];
        if(tile == -1)
            total++;
    }
    return total;
    
}
int my_player::get_safe_pieces(int* position){

    int total = 0;
    for(int i = 0; i < 4; i++)
    {
        int tile = position[i];
        if((is_globe_tile(tile) == true) || (tile > 50 && tile < 56))
            total++;
    }
    
    return total;

}
int my_player::get_scared_pieces(int* position){ 

    int total = 0;
    for (int player = 0; player < 4; player++) // Go through all pieces
    {   
        bool danger = false;
        int tile = position[player];
        if((tile != 99 || tile != -1 ) && (tile <= 50) && (is_globe_tile(tile) == false)){
            for (int enemy = 4; enemy < 16; enemy++)// Check if enemy behind 6 of piece
            {
                int difference = position[player] - position[enemy];
                if (difference <= 6 && difference >= 1)  // if true count++
                {
                danger = true;
                }

            }
            if(danger)    
                total++;
        }
    }
    return total;    
}

int my_player::get_goal_pieces(int* position){
    int total = 0;
    for(int i = 0; i < 4; i++)
    {
        int tile = position[i];
        if(tile == 99)
            total++;
    }
    return total;
}

int my_player::get_unsafe_pieces(int* position){ 

    int total = 0;
    for(int i = 0; i < 4; i++)
    {
        int tile = position[i];

        if((tile != 99 || tile != -1 ) && (tile <= 50)){
            if(is_globe_tile(tile) == false){
                total++;
            }
        }
    }
    return total;
}


bool my_player::check_grouping(int tile){
    bool grouping = false;
    
    for (int i = 0; i < 3; i++)
    {
        if(position[i] == tile)
            grouping = 1;
    }
    return grouping;
}


std::vector<int> my_player::get_states(int* position){
    // "Home", "Safe", "Unprotected", "Danger", "Goal"
    std::vector<int> states;
    int count_home      = get_cozy_pieces(position); // Cozy beacuse it's at home
    int count_safe      = get_safe_pieces(position);
    int count_danger    = get_scared_pieces(position); // scared because in danger
    int count_unsafe    = get_unsafe_pieces(position) - count_danger;
    int count_goal      = get_goal_pieces(position);

    if( (count_home + count_safe + count_danger + count_unsafe + count_goal) != 4 ) 
        throw std::exception("Count of pieces != 4");

    for (int i = 0; i < count_home; i++)
        states.push_back(Home);
    for (int i = 0; i < count_safe; i++)
        states.push_back(Safe);
    for (int i = 0; i < count_safe; i++)
        states.push_back(Safe);
    for (int i = 0; i < count_danger; i++)
        states.push_back(Danger);
    for (int i = 0; i < count_goal; i++)
        states.push_back(Goal);

    return states;
}


double my_player::get_learning_rate(){
    return learning_rate;
}

double my_player::get_discount_factor(){
    return discount_factor;
}