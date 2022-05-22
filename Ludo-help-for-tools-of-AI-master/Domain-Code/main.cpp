#include <iostream>
#include "game.h"
#include "test_game.h"
#include "iplayer.h"
#include "player_random.h"
#include "my_player.h"

#include <fstream>

using namespace std;

int main()
{
    int batches = 100;
    int n_games_per_batches = 100;
    int wins_overall[] = {0, 0, 0, 0};
    bool do_learning = true;

            // ******************************************************
                // For the report !
                // Plot Q-table values after each game for 100 games
    //Run the unit tests
    test_game tester;
    tester.run_all_tests();
    Q_Table q_table(do_learning);
    //Create players
    
    my_player CreamBot(q_table, 0.1);
    player_random player_1;
    player_random player_2;
    player_random player_3;
    cout << "Playing Ludo... " << endl;
    //Play a game of Ludo
    game g(&CreamBot, &player_1, &player_2, &player_3);
    g.play_game();
    cout << "Player " << g.get_winner() << " won the game!" << endl << endl;

    std::ofstream wins_file;
    if (do_learning) wins_file.open("../../logs/wins.csv");
    else wins_file.open("../../logs/sanity_check.csv");

    if(!wins_file.is_open()) std::cout << "Log file not open" <<std::endl;
    wins_file << "Player 0, Player 1, Player 2, Player 3\n";

    for(int i = 0; i < batches; i++)
    {   
        int wins_batch[] = {0, 0, 0, 0};
        for (int batch = 0; batch < n_games_per_batches; batch++)
        {
            g.reset();
            g.set_first(batch%4); //alternate who starts the game
            g.play_game();
            wins_batch[g.get_winner()]++; 
            wins_overall[g.get_winner()]++;     
        }
        std::cout << "Batch " << i <<" score: \n";
        for(int j = 0; j < 4; j++){
        cout << "Player " << j << " won " << wins_batch[j] << " games. Winrate "<< 100.0f *  ((float)wins_batch[j]/((float)n_games_per_batches)) <<"%" << endl;
        }
            wins_file   << 100.0f * ((float)wins_batch[0]/n_games_per_batches) << ", "
                        << 100.0f * ((float)wins_batch[1]/n_games_per_batches) << ", "
                        << 100.0f * ((float)wins_batch[2]/n_games_per_batches) << ", "
                        << 100.0f * ((float)wins_batch[3]/n_games_per_batches) << ", "
                        << "\n";

    }
    wins_file.close();
    std::cout << "Overall score: \n";
    for(int i = 0; i < 4; i++){
        cout << "Player " << i << " won " << wins_overall[i] << " games. Winrate "<< 100.0f * ((float)wins_overall[i]/n_games_per_batches/batches ) <<"%" << endl;
    }
        
    cout << "End of main" << endl;
    return 0;
}


