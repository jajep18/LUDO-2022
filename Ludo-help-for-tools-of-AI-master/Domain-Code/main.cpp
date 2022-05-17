#include <iostream>
#include "game.h"
#include "test_game.h"
#include "iplayer.h"
#include "player_random.h"
#include "my_player.h"

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

    //Play many games of Ludo
    int n_games = 1000;
    int wins[] = {0, 0, 0, 0};
    for(int i = 0; i < n_games; i++)
    {
        g.reset();
        g.set_first(i%4); //alternate who starts the game
        g.play_game();
        wins[g.get_winner()]++;
    }
    for(int i = 0; i < 4; i++){
        cout << "Player " << i << " won " << wins[i] << " games. Winrate "<< ((double)wins[i]/n_games)*100 <<"%" << endl;
    }
        



    cout << "End of main" << endl;
    return 0;
}


