
#include "Player.h"
#include "ale_interface.hpp"
#include "SDL.h"

const string Player::BREAKOUT_ROM = "../breakout/breakout.bin";
const string Player::BOXING_ROM = "../boxing/boxing.bin";
const string Player::DEMONATTACK_ROM = "../dattack/demon_attack.bin";
const string Player::STARGUNNER_ROM = "../strgunner/star_gunner.bin";

ALEInterface alei;

int Player::breakoutGetPlayerX()
{
   return alei.getRAM().get(72);// + ((rand() % 3) - 1);
}

int Player::breakoutGetBallX()
{
   return alei.getRAM().get(99);// + ((rand() % 3) - 1);
}

vector<int> Player::playBreakout(NeuralNetwork &nn, bool displayScreen)
{
    vector<int> res;
    int lastLives = 0;
    float totalReward = .0f;
    int maxSteps = 15000;

    alei.disableBufferedIO();

    // Init rand seed
    srand(time(NULL));

    // Create alei object.
    alei.setInt("random_seed", rand()%1000);
    alei.setFloat("repeat_action_probability", 0);
    alei.setBool("sound", false);
    alei.setBool("display_screen", displayScreen);
    alei.loadROM(Player::BREAKOUT_ROM);

    lastLives = alei.lives();

    int score = 0;
    int step = 0;
    int BallX_LastTick = Player::breakoutGetBallX();

    alei.act(PLAYER_A_FIRE);

    for (step = 0; !alei.game_over() && step < maxSteps; ++step) 
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
        Mat outputs(1, 2);

        inputs.set(0, 0, ballX);
        inputs.set(0, 1, direction);
        inputs.set(0, 2, playerX);
        inputs.set(0, 3, wide);

        outputs = nn.forwardPropagation(inputs);

        if (outputs.get(0, 0) > 0.5)
        {
            reward += alei.act(PLAYER_A_RIGHT);
        }
        else if (outputs.get(0, 1) > 0.5)
        {
            reward += alei.act(PLAYER_A_LEFT);
        }

        BallX_LastTick = Player::breakoutGetBallX();

        reward = (reward + alei.act(PLAYER_A_NOOP));
        totalReward += reward;
    }

    score = (int)totalReward;

    res.push_back(score);
    res.push_back(step);

    return res;
}

vector<int> Player::playBoxing(NeuralNetwork &nn, bool displayScreen)
{
    vector<int> res;

    return res;
}

vector<int> Player::playDemonAttack(NeuralNetwork &nn, bool displayScreen)
{
    vector<int> res;
    int lastLives = 0;
    float totalReward = .0f;
    int maxSteps = 7500;

    alei.disableBufferedIO();

    // Init rand seed
    srand(time(NULL));

    // Create alei object.
    alei.setInt("random_seed", rand()%1000);
    alei.setFloat("repeat_action_probability", 0);
    alei.setBool("sound", false);
    alei.setBool("display_screen", displayScreen);
    alei.loadROM(Player::DEMONATTACK_ROM);

    lastLives = alei.lives();

    int score = 0;
    int step = 0;


    for (step = 0; !alei.game_over() && step < maxSteps; ++step) 
    {   
        float reward = 0;
        
        Mat inputs(1, 128);
        Mat outputs(1, 3);

        for (unsigned i = 0; i < 128; ++i) {
            inputs.set(0, i, (double)alei.getRAM().get(i) / 255);                                                            
        }

        outputs = nn.forwardPropagation(inputs);

        int prediction = 2;

        for (int i = 0; i < 2; i++)
        {
            if (outputs.get(0, i) > outputs.get(0, i + 1))
            {
                prediction = i;
            }
        }
        
        switch (prediction)
        {
            case 0:
                reward += alei.act(PLAYER_A_FIRE);
                break;
            case 1:
                reward += alei.act(PLAYER_A_LEFT);
                break;
            case 2:
                reward += alei.act(PLAYER_A_RIGHT); 
                break;
            default:
                cout << "debug" << endl;
                break;
        }

        reward = (reward + alei.act(PLAYER_A_NOOP));
        totalReward += reward;
    }

    score = (int)totalReward;

    res.push_back(score);
    res.push_back(step);

    return res;
}

vector<int> Player::playStarGunner(NeuralNetwork &nn, bool displayScreen)
{
    vector<int> res;

    return res;
}