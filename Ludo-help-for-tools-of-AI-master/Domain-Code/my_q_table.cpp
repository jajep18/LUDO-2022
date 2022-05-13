#include "my_q_table.h"

Q_Table::Q_Table(){
    std::cout << "Q-Table ready\n"<< std::endl;
}
Q_Table::~Q_Table(){}


double Q_Table::get_q_table_value(std::vector<int> states, int action){
    return q_table[states[0]][states[1]][states[2]][states[3]][action];
}

void Q_Table::update_q_table(std::vector<int> states, int action, std::vector<int> next_states, double alpha, double gamma){
    // Update Q-table values
    //Q[state, action] = Q[state, action] + lr * (reward + gamma * np.max(Q[new_state, :]) — Q[state, action])

    q_table[states[0]][states[1]][states[2]][states[3]][action] = get_q_table_value(states, action) + alpha * 
    ( get_reward(action) + gamma + get_q_table_value(next_states, get_best_action(next_states)) ) - get_q_table_value(states, action);
    
}


double Q_Table::get_reward(int action){
    return reward_vector[action];
}

int Q_Table::get_best_action(std::vector<int> next_states){
    
    int best_action = -99;
    double best_action_reward = -999.0;
    for (int action = 0; action != Last; action++)
    {   
        double contender_reward = get_reward(action);
        if (best_action_reward < contender_reward)
        {
            best_action_reward =  contender_reward;
            best_action = action;
        }
        else if(best_action_reward == contender_reward){
            distribution = std::uniform_int_distribution<int>(0, 1);
            int decider_val = distribution(generator);
            if (decider_val == 0){
                best_action = action;
                best_action_reward = contender_reward;
            }
        }
    }
   return best_action;
}