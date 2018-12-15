
#include <vector>

#include "Player.h"
#include "ale_interface.hpp"
#include "SDL.h"

ALEInterface alei;

//int Player::playBreakout(NeuralNetwork &nn){return 1;}

int Player::breakoutGetPlayerX()
{
   return alei.getRAM().get(72);// + ((rand() % 3) - 1);
}

int Player::breakoutGetBallX()
{
   return alei.getRAM().get(99);// + ((rand() % 3) - 1);
}

int Player::playBreakout(NeuralNetwork &nn)
{
    int lastLives = alei.lives();
    float totalReward = .0f;
    int maxSteps = 15000;

    // Init rand seed
    srand(time(NULL));

    // Create alei object.
    alei.setInt("random_seed", rand()%1000);
    alei.setFloat("repeat_action_probability", 0);
    alei.setBool("sound", false);
    alei.setBool("display_screen", false);
    alei.loadROM("breakout.bin");

    int score = 0;
    int BallX_LastTick = Player::breakoutGetBallX();

    for (int step = 0; !alei.game_over() && step < maxSteps; ++step) 
    {
        int wide = alei.getRAM().get(108);
        float reward = 0;

        if (alei.lives() != lastLives)
        {
            --lastLives;
            alei.act(PLAYER_A_FIRE);
        }

        int playerX = Player::breakoutGetPlayerX();
        int ballX = Player::breakoutGetBallX();
        double direction = 0.5;
        
        if (BallX_LastTick < ballX) {
            direction = 0.0;
        }
        if (BallX_LastTick > ballX) {
            direction = 1.0;
        }

        Mat inputs(1, 4);
        Mat outputs(1, 1);

        inputs.set(0, 0, ballX);
        inputs.set(0, 1, direction);
        inputs.set(0, 2, playerX);
        inputs.set(0, 3, wide);

        outputs = nn.forwardPropagation(inputs);

        if (outputs.get(0, 0) < 0.5)
        {
            reward += alei.act(PLAYER_A_RIGHT);
        }
        else
        {
            reward += alei.act(PLAYER_A_LEFT);
        }

        BallX_LastTick = Player::breakoutGetBallX();

        reward = (reward + alei.act(PLAYER_A_NOOP));
        totalReward += reward;
    }

    score = (int)totalReward;

    return score;
}