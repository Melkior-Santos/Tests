#include <iostream>
#include <cstring>
#include <random>
#include <fstream>
#include <algorithm>

using namespace std;


random_device generator;
mt19937 rng(generator());
uniform_int_distribution<mt19937::result_type> rand100(1,100);  //  generates a random number from 1 to 100;

bool ignorant(int c_r, bool * opponent_l_r)  // Always returns 0
{
    return false;
}

bool nice_guy(int c_r, bool * opponent_l_r)  // Always returns 1
{
    return true;
}

bool tit_for_tat(int c_r, bool * opponent_l_r)  // Returns 1 on the first move, then mimics the last opponent's move    
{
    if(c_r == 0)
    {
        return true;
    }
    else
    {
        if(opponent_l_r[c_r-1])
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
bool vandetta(int c_r, bool * opponent_l_r)  //Returns 1 on the first move, if opponent plays 0 once, always return 0
{
    int played_false;    
    if(c_r == 0)
    {
        return true;
    }
    else
    {
        for (int k = 0; k < c_r; k++)
        {
            if(!(opponent_l_r[k]))
            {
                return false;
            }
        }
        return true;
    }

}

bool random(int c_r, bool * opponent_l_r)  // 50% chance to return either 1 or 0
{
    if (rand100(rng) < 51)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool bong_cloud(int c_r, bool * opponent_l_r)  //Returns 1 on the first move, if opponent played 0 once then 80% Chance to return 0
{

    int played_false;    
    if(c_r == 0)
    {
        return true;
    }
    else
    {
        for (int k = 0; k < c_r; k++)
        {
            if(!(opponent_l_r[k]))
            {
                if(rand100(rng) > 21)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
        return true;
    }
}

bool nice_tit_for_that(int c_r, bool * opponent_l_r)  // Returns 1 on the first move, then mimics the last opponent's move, with a 20% chance to return 1 regardless   
{

    if(c_r == 0)
    {
        return true;
    }
    else
    {
        if(opponent_l_r[c_r-1])
        {
            return true;
        }
        else
        {
            if (rand100(rng) > 21)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }

}

bool average(int c_r, bool * opponent_l_r)
{
    if(c_r == 0)
    {
        return true;
    }
    float played_false = 0;
    float avg_false;
    for (int i =0; i < c_r; i++)
    {
        if(opponent_l_r[i] == 0)
        {
            played_false++;
        }
    }
    avg_false = (played_false / c_r) * 100; 
    if(avg_false < 50)
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool mean(int c_r, bool * opponent_l_r)
{
    if (c_r < 3)
    {
        return true;
    }
    else
    {
        if(opponent_l_r[c_r - 2] && opponent_l_r[c_r -1])
        {
            return false;
        }
        else
        {
            return opponent_l_r[c_r - 1];
        }
    }
}

const int ROUNDS = 200;
const int PARTICIPANTS = 9;


typedef bool strat(int, bool*);  //  makes a function type

class Player
{
    private:
        string name;
        int points = 0;
        int overall_points = 0;
        int * op_ptr = &overall_points;  //  overall points pointer

        bool rounds_player[ROUNDS];
        bool round_opponent[ROUNDS];
        int player_current_round;

        strat * player_strategy;
        int wins = 0;
        int * wins_ptr = &wins;

        int draws = 0;
        int * draws_ptr = &draws;

        int losses = 0;
        int * losses_ptr = &losses;


    public:
        void init(string n, strat k);
        void change_points(int value)  // adds the value passed to the points variable
        {
            points += value;
        }
        int get_points() const
        {
            return points;
        }

        void change_overall_points(int value)  // adds the value passed to the points variable
        {
            *op_ptr += value;

        }
        int get_overall_points() const
        {
            return overall_points;
        }
        void set_overall_points(int value)
        {
            *op_ptr = value;
        }
        Player compare_overall_points(Player);

        int get_wins() const
        {
            return wins;
        }
        void add_win();

        int get_draws() const
        {
            return draws;
        }
        void add_draws();

        int get_losses() const
        {
            return losses;
        }
        void add_losses();


        bool * get_rounds();
        void set_player_round(int, bool);
        bool strategy(int c_r, bool* opponent_l_r, bool (*func)(int , bool *));
        strat * get_strategy();


        string get_name() const
        {
            return name;
        }

        Player()
        {
            name = "";
            overall_points = 0;
            points = 0;
        }

        Player(string N)
        {
            name = N;
        }

        Player(string N, strat K)
        {
            init(N, K);
        }

        Player(int value)
        {
            overall_points = value;
        }


};


class Game
{
    private:
        Player player1;
        Player player2;


        int current_round = 0;
    public:
        Player Run(Player, Player);  
        Player Compare_Points(Player, Player);
        int Get_current_round() const;

};


int main()
{
    std::ofstream out("results.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

    Player player[PARTICIPANTS];
    player[0].init("Ignorant", &ignorant);
    player[1].init("Nice Guy", &nice_guy);
    player[2].init("Tit-for-that", &tit_for_tat);
    player[3].init("Vandetta", &vandetta);
    player[4].init("Bong cloud", &bong_cloud);
    player[5].init("Nice Tit-for-that", &nice_tit_for_that);
    player[6].init("Average", &average);
    player[7].init("Mean", &mean);
    player[8].init("Random", &random);
    
    
    int cycles = 0;
    for(int i = PARTICIPANTS; i > 0; i--)
    {
        cycles += i - 1;
    }
    const int CYCLES = cycles;

    /* Player players[PARTICIPANTS]; */

    /* Game games[CYCLES]; */
    Game game;

    Player winner[CYCLES];
    int loop_counter = -1;
    for (int j = 0; j < 5; j++)
    {
        for(int k = 1; k < PARTICIPANTS; k ++)
        {
            for (int i = 0; i < k; i++)
            {
                cout << "\nMatch: " << loop_counter + 2 << "  -Tournament: " << j + 1 << endl;
                winner[++loop_counter] = game.Run(player[k],player[i]); 
            }
        }
        loop_counter = -1;
    }

    sort(player, player + PARTICIPANTS,
            [](Player const & a, Player const & b) -> bool
            { return a.get_overall_points() > b.get_overall_points(); } );

    cout << endl;

    for (int i = 0; i < PARTICIPANTS; i++)
    {
        cout << i + 1 << "°: " << player[i].get_name() << ": " << player[i].get_overall_points() << " || Wins:" << player[i].get_wins() << 
            "  Draws:" << player[i].get_draws() << "  Losses:" << player[i].get_losses() << endl;
    }

    /* Player rankings[PARTICIPANTS];  // i know the problem is between here and return 0 */

    /* Player max(-1); */

    /* for (int k = 0; k < PARTICIPANTS; k++) */

    /* {   int j; */
    /*     for (j = 0 ; j < PARTICIPANTS; j ++) */
    /*     { */
    /*         rankings[k] = max.compare_overall_points(player[j]); */
    /*     } */
    /*     player[j].change_overall_points(-(player[j].get_overall_points())- 1);  // the problem was here */

    /* } */

    /* for (int i = 0; i < PARTICIPANTS; i++) */
    /* { */
    /*     cout << i + 1 << ": " << rankings[i].get_overall_points() << endl; */
    /* } */

    return 0;

}

void Player::init(string n, strat k)
{
    name = n; 
    player_strategy = k;
}

bool * Player::get_rounds()
{
    return rounds_player;
}

bool Player::strategy(int c_r, bool* opponent_l_r, bool (*func)(int , bool *))
{
    return func(c_r, opponent_l_r);

}

strat * Player::get_strategy()
{
    return player_strategy;
}

void Player::set_player_round(int c_r, bool action)
{
    rounds_player[c_r] = action;
}


void Player::add_win()
{
        *wins_ptr += 1;
}

void Player::add_draws()
{
    *draws_ptr += 1;
}

void Player::add_losses()
{
    *losses_ptr += 1;
}

Player Player::compare_overall_points(Player player2)
{
    if (this->get_overall_points() < player2.get_overall_points())
    {
        return player2;
    }
    else
    {
        return *this;
    }

}


Player Game::Compare_Points (Player player1, Player player2)
{
    Player draw("draw");

    int points1 = player1.get_points();
    int points2 = player2.get_points();

    if (points1 == points2)
    {
        player1.add_draws();
        player2.add_draws();
        return draw;
    }
    else if (points1 > points2)
    {
        player1.add_win();
        player2.add_losses();

        return player1;
    }
    else
    {
        player2.add_win();
        player1.add_losses();
        return player2;
    }


}

Player Game::Run(Player player1, Player player2)
{
    cout << "Player 1: " << player1.get_name() << endl;
    cout << "Player 2: " << player2.get_name() << endl;

    for (int i = 0; i < ROUNDS; i++)
    {

        bool p1_move = player1.strategy(i, player2.get_rounds(), player1.get_strategy());
        bool p2_move = player2.strategy(i, player1.get_rounds(), player2.get_strategy());

        if (p1_move && p2_move)
        {
            player1.change_points(3);
            player2.change_points(3);
        }

        else if (!p1_move && !p2_move)
        {
            player1.change_points(1);
            player2.change_points(1);
        }

        else if (!p1_move && p2_move)
        {
            player1.change_points(5);
            player2.change_points(0);
        }

        else if (p1_move && !p2_move)
        {
            player1.change_points(0);
            player2.change_points(5);
        }

        player1.set_player_round(i, p1_move);
        player2.set_player_round(i, p2_move);


    }

    int p1_score = player1.get_points();
    int p2_score = player2.get_points();

    for (int k = 0; k < ROUNDS; k++)
    {
        cout << player1.get_rounds()[k] << " ";
    }

    cout << p1_score <<endl;

    for (int k = 0; k < ROUNDS; k++)
    {
        cout << player2.get_rounds()[k] << " ";
    }
    cout << p2_score << endl;
    cout << player1.get_name() << ": " << p1_score << " || "<< player2.get_name() << ": " << p2_score << endl;;

    player1.change_overall_points(p1_score);
    player2.change_overall_points(p2_score);

    return Compare_Points(player1,player2);

}

