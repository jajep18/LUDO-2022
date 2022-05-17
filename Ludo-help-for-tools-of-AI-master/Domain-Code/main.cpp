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

    //Run the unit tests
    test_game tester;
    tester.run_all_tests();
    Q_Table q_table;
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
    // Test for batches of games, print winrate for every 50 games to compare learning
    //Play many games of Ludo
    int batches = 100;
    int n_games_per_batches = 100;
    int wins_overall[] = {0, 0, 0, 0};

    std::ofstream wins_file;
    wins_file.open("../logs/wins.csv");
    
    if(!wins_file.is_open()) throw std::exception("Log file not open");
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
        cout << "Player " << j << " won " << wins_batch[i] << " games. Winrate "<< ((double)wins_batch[j]/n_games_per_batches)*100 <<"%" << endl;
        }
            wins_file << ((double)wins_batch[0]/n_games_per_batches)*100 << ", "
             << ((double)wins_batch[1]/n_games_per_batches)*100 << ", "
             << ((double)wins_batch[2]/n_games_per_batches)*100 << ", "
             << ((double)wins_batch[3]/n_games_per_batches)*100 << ", "
             << "\n";

    }
    wins_file.close();
    std::cout << "Overall score: \n";
    for(int i = 0; i < 4; i++){
        cout << "Player " << i << " won " << wins_overall[i] << " games. Winrate "<< ((double)wins_overall[i]/n_games_per_batches/batches )*100 <<"%" << endl;
    }
        
    cout << "End of main" << endl;
    return 0;
}


