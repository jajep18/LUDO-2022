#include "my_q_table.h"

Q_Table::Q_Table(bool learning){

    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < 5; j++)
        {
            for (size_t k = 0; k < 5; k++)
            {
                for (size_t l = 0; l < 5; l++)
                {
                    for (size_t m = 0; m < 10; m++)
                    {
                        q_table[i][j][k][l][m] = 0;
                    }
                    
                }
                
            }
            
        }
        
    }
    learn = learning;
    std::cout << "Q-Table ready\n"<< std::endl;
}
Q_Table::~Q_Table(){}


double Q_Table::get_q_table_value(std::vector<int> states, int action){
    // std::cout << "States size " << states.size() << std::endl;
    if(states[0] < 0 || states[0] > 4) throw std::exception();
    if(states[1] < 0 || states[1] > 4) throw std::exception();
    if(states[2] < 0 || states[2] > 4) throw std::exception();
    if(states[3] < 0 || states[3] > 4) throw std::exception();
    if(action < 0 || action > 9) throw std::exception();

    // for (int i = 0; i < states.size(); i++)
    // {
    //     std::cout << "State " << i << ": "  <<  states[i]<< std::endl;
    // }
    // std::cout << "Action" << action << std::endl;
    return q_table[states[0]][states[1]][states[2]][states[3]][action];
}

void Q_Table::update_q_table(std::vector<int> states, int action, std::vector<int> next_states, double alpha, double gamma){
    // Update Q-table values
    //Q[state, action] = Q[state, action] + lr * (reward + gamma * np.max(Q[new_state, :]) — Q[state, action])
    double updated_val = get_q_table_value(states, action) + alpha * 
    ( get_reward(action) + gamma + get_q_table_value(next_states, get_best_action(next_states)) ) - get_q_table_value(states, action);
    if (learn)
        q_table[states[0]][states[1]][states[2]][states[3]][action] = updated_val;
    
}


double Q_Table::get_reward(int action){
    // std::cout << "Reward vector size " << reward_vector.size() << std::endl;
    // for (size_t i = 0; i < reward_vector.size(); i++)
    // {
    //     std::cout << "Reward vector " << reward_vector[i] << " " << std::endl;
    // }
    
    
    // std::cout << "Action being rewarded: " << action  << std::endl;
    double reward = reward_vector[action];
    // std::cout << "Reward for action: "<< reward << std::endl;
    return reward;
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