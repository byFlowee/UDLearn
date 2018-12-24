
#include <limits>

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
   return alei.getRAM().get(72);
}

int Player::breakoutGetBallX()
{
   return alei.getRAM().get(99);
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
        
        if (BallX_LastTick < ballX)
        {
            direction = 0.0;
        }
        if (BallX_LastTick > ballX)
        {
            direction = 1.0;
        }

        Mat inputs(1, 4);
        Mat outputs(1, 2);

        inputs.set(0, 0, ballX);
        inputs.set(0, 1, direction);
        inputs.set(0, 2, playerX);
        inputs.set(0, 3, wide);

        outputs = nn.forwardPropagation(inputs);

        int prediction = 0;
        double maxValue = numeric_limits<double>::lowest();

        for (int i = 0; i < outputs.cols(); i++)
        {
            if (outputs.get(0, i) > maxValue)
            {
                maxValue = outputs.get(0, i);
                prediction = i;
            }
        }
        
        switch (prediction)
        {
            case 0:
                reward += alei.act(PLAYER_A_RIGHT);
                break;
            case 1:
                reward += alei.act(PLAYER_A_LEFT);
                break;
            default:
                cout << "Breakout: unknown action." << endl;
                break;
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
    alei.loadROM(Player::BOXING_ROM);

    int score = 0;
    int step = 0;

    for (step = 0; !alei.game_over() && step < maxSteps; ++step) 
    {   
        float reward = 0;
        
        Mat inputs(1, 4);
        Mat outputs(1, 5);

        inputs.set(0, 0, (double)alei.getRAM().get(32) / 255.0);    // X coordinate of P1
        inputs.set(0, 1, (double)alei.getRAM().get(33) / 255.0);    // X coordinate of P2
        inputs.set(0, 2, (double)alei.getRAM().get(34) / 255.0);    // Y coordinate of P1
        inputs.set(0, 3, (double)alei.getRAM().get(35) / 255.0);    // Y coordinate of P2

        outputs = nn.forwardPropagation(inputs);

        int prediction = 0;
        double maxValue = numeric_limits<double>::lowest();

        for (int i = 0; i < outputs.cols(); i++)
        {
            if (outputs.get(0, i) > maxValue)
            {
                maxValue = outputs.get(0, i);
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
            case 3:
                reward += alei.act(PLAYER_A_UP);
                break;
            case 4:
                reward += alei.act(PLAYER_A_DOWN);
                break;
            default:
                cout << "Boxing: unknown action." << endl;
                break;
        }

        reward = (reward + alei.act(PLAYER_A_NOOP));
        totalReward += reward;
    }

    score = (int)totalReward;

    res.push_back(score);
    res.push_back(step);
    res.push_back(alei.getRAM().get(18));   // Final score of P1
    res.push_back(alei.getRAM().get(19));   // Final score of P2

    return res;
}

vector<int> Player::playDemonAttack(NeuralNetwork &nn, bool displayScreen)
{
    vector<int> res;
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
    alei.loadROM(Player::DEMONATTACK_ROM);

    int score = 0;
    int step = 0;
    int moveLeft = 0;
    int moveRight = 0;
    int notMoving = 0;

    int lastShoot = alei.getRAM().get(52);

    for (step = 0; !alei.game_over() && step < maxSteps; ++step) 
    {   
        float reward = 0;
        
        Mat inputs(1, 11);
        Mat outputs(1, 3);

        int currentShoot = lastShoot = alei.getRAM().get(52);

        if (currentShoot != lastShoot || currentShoot != 0)
        {
            inputs.set(0, 9, 1.0);
        }
        else
        {
            inputs.set(0, 9, 0.0);
        }

        inputs.set(0, 0, (double)alei.getRAM().get(13) / 255.0);
        inputs.set(0, 1, (double)alei.getRAM().get(14) / 255.0);
        inputs.set(0, 2, (double)alei.getRAM().get(15) / 255.0);
        inputs.set(0, 3, (double)alei.getRAM().get(16) / 255.0);
        inputs.set(0, 4, (double)alei.getRAM().get(17) / 255.0);
        inputs.set(0, 5, (double)alei.getRAM().get(18) / 255.0);
        inputs.set(0, 6, (double)alei.getRAM().get(19) / 255.0);
        inputs.set(0, 7, (double)alei.getRAM().get(20) / 255.0);
        inputs.set(0, 8, (double)alei.getRAM().get(22) / 255.0);
        //inputs.set(0, 9, (double)alei.getRAM().get(52) / 255.0);
        inputs.set(0, 10, (double)alei.getRAM().get(72) / 255.0);

        outputs = nn.forwardPropagation(inputs);

        int prediction = 0;
        double maxValue = numeric_limits<double>::lowest();

        for (int i = 0; i < outputs.cols(); i++)
        {
            if (outputs.get(0, i) > maxValue)
            {
                maxValue = outputs.get(0, i);
                prediction = i;
            }
        }
        
        switch (prediction)
        {
            case 0:
                notMoving++;
                reward += alei.act(PLAYER_A_FIRE);
                break;
            case 1:
                moveLeft++;
                reward += alei.act(PLAYER_A_LEFT);
                break;
            case 2:
                moveRight++;
                reward += alei.act(PLAYER_A_RIGHT); 
                break;
            default:
                cout << "Demon Attack: unknown action." << endl;
                break;
        }

        reward = (reward + alei.act(PLAYER_A_NOOP));
        totalReward += reward;

        lastShoot = alei.getRAM().get(52);
    }

    score = (int)totalReward;

    res.push_back(score);
    res.push_back(step);
    res.push_back(moveLeft);
    res.push_back(moveRight);
    res.push_back(notMoving);

    return res;
}

vector<int> Player::playStarGunner(NeuralNetwork &nn, bool displayScreen)
{
    vector<int> res;
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
    alei.loadROM(Player::STARGUNNER_ROM);

    int score = 0;
    int step = 0;

    for (step = 0; !alei.game_over() && step < maxSteps; ++step) 
    {   
        float reward = 0;
        
        Mat inputs(1, 12);
        Mat outputs(1, 5);

        inputs.set(0, 0, (double)alei.getRAM().get(14) / 255.0);    // Y coordinate of player
        inputs.set(0, 1, (double)alei.getRAM().get(21) / 255.0);    // X coordinate of player
        inputs.set(0, 2, (double)alei.getRAM().get(31) / 255.0);    // X coordinate of enemy
        inputs.set(0, 3, (double)alei.getRAM().get(32) / 255.0);    // X coordinate offset of enemy
        inputs.set(0, 4, (double)alei.getRAM().get(33) / 255.0);    // X coordinate of enemy's bullet
        inputs.set(0, 5, (double)alei.getRAM().get(34) / 255.0);    // Y coordinate of enemy's bullet
        inputs.set(0, 6, (double)alei.getRAM().get(71) / 255.0);    // Y coordinate of 1st objective
        inputs.set(0, 7, (double)alei.getRAM().get(72) / 255.0);    // Y coordinate of 2nd objective
        inputs.set(0, 8, (double)alei.getRAM().get(73) / 255.0);    // Y coordinate of 3rd objective
        inputs.set(0, 9, (double)alei.getRAM().get(74) / 255.0);    // X coordinate of 1st objective
        inputs.set(0, 10, (double)alei.getRAM().get(75) / 255.0);   // X coordinate of 2nd objective
        inputs.set(0, 11, (double)alei.getRAM().get(76) / 255.0);   // X coordinate of 3rd objective

        outputs = nn.forwardPropagation(inputs);

        int prediction = 0;
        double maxValue = numeric_limits<double>::lowest();

        for (int i = 0; i < outputs.cols(); i++)
        {
            if (outputs.get(0, i) > maxValue)
            {
                maxValue = outputs.get(0, i);
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
            case 3:
                reward += alei.act(PLAYER_A_UP);
                break;
            case 4:
                reward += alei.act(PLAYER_A_DOWN);
                break;
            default:
                cout << "StarGunner: unknown action." << endl;
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