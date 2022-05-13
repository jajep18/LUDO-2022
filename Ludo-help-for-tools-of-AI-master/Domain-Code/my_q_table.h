#ifndef Q_TABLE_H
#define Q_TABLE_H

#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <iostream>
#include <fstream>
#include <random>

enum actions {Spawn,  Normal_move, Go_goal, Go_goal_zone, Go_star, Go_globe, Group_up, Kill, Suicide, Bounce, Last};
enum states { Home = 0, Safe = 1, Unprotected = 2, Danger = 3, Goal = 4 };
class Q_Table{
public:

    Q_Table();
    ~Q_Table();
    double get_q_table_value(std::vector<int> states, int action); 

    void update_q_table(std::vector<int> states, int action, std::vector<int> next_states, double alpha, double gamma);

    double get_reward(int action);
    int get_best_action(std::vector<int> next_state);
  

private:

    std::vector<std::string> states_vector  = { "Home", "Safe", "Unprotected", "Danger", "Goal"};

    //enum actions                          {Spawn,  Normal_move, Go_goal, Go_goal_zone, Star, Globe, group_up, Kill, Suicide, Bounce};
    std::vector<double> reward_vector  =    { 0.4,   0.1,         0.7,     0.5,          0.3,   0.35, 0.35,     0.4   -0.4, -0.2};

    double q_table[5][5][5][5][10] = {0}; //[Piece1][Piece2][Piece3][Piece4][Actions] 
    //Piece contains:  "Home", "Safe", "Unprotected", "Danger", "Goal"
     // Actions: 
        // Spawn
        // Move
        // Go Goal
        // Go to goal zone
        // Star
        // Globe
        // Protect
        // Kill
        // Suicide
        // Nothing 
        // States: Number of states chosen using n-choose-K  For report: https://www.probabilitycourse.com/chapter2/2_1_4_unordered_with_replacement.php


    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;

};

#endif // Q_TABLE_H
