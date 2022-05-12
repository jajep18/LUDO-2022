#include "my_q_table.h"

Q_Table::Q_Table(){}
Q_Table::~Q_Table(){}


double Q_Table::get_q_table_value(std::vector<int> states, int action){
    return q_table[states[0]][states[1]][states[2]][states[3]][action];
}

void Q_Table::update_q_table(std::vector<int> states, int action, int* next_state){
    // Update Q-table values
    //Q[state, action] = Q[state, action] + lr * (reward + gamma * np.max(Q[new_state, :]) — Q[state, action])
}



