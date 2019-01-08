
#include <limits>

#include "Player.h"
#include "UtilG.h"

#include "ale_interface.hpp"
#include "SDL.h"

const string Player::BREAKOUT_ROM = "../../breakout/breakout.bin";
const string Player::BOXING_ROM = "../../boxing/boxing.bin";
const string Player::DEMONATTACK_ROM = "../../dattack/demon_attack.bin";
const string Player::STARGUNNER_ROM = "../../strgunner/star_gunner.bin";

vector<int> Player::enemyBulletCoordinateXHistory;
int Player::enemyBulletCoordinateXHistoryIndex = 0;

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
    int maxSteps = 7500;

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
    int movementLeft = 0;
    int movementUp = 0;
    int movementRight = 0;
    int movementDown = 0;

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
                movementLeft++;
                reward += alei.act(PLAYER_A_LEFT);
                break;
            case 2:
                movementRight++;
                reward += alei.act(PLAYER_A_RIGHT);
                break;
            case 3:
                movementUp++;
                reward += alei.act(PLAYER_A_UP);
                break;
            case 4:
                movementDown++;
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
    res.push_back(movementLeft);
    res.push_back(movementUp);
    res.push_back(movementRight);
    res.push_back(movementDown);

    return res;
}

int Player::demonAttackGetCoordinate(int position)
{
    int const val(alei.getRAM().get(position));
    int const rawFirstNibble((7 - ((val & 0xF0) >> 4)) & 0x0F);
    int const rawSecondNibble(val & 0x0F);

    return (rawSecondNibble * 16) + rawFirstNibble;
}

bool Player::demonAttackIsEnemyAlive(Player::DemonAttackEnemy enemy)
{
    int sprite = 255;

    switch (enemy)
    {
        case Player::LeftFlyFarthest:
            sprite = alei.getRAM().get(29);
            break;
        case Player::LeftFlyMiddle:
            sprite = alei.getRAM().get(30);
            break;
        case Player::LeftFlyClosest:
            sprite = alei.getRAM().get(31);
            break;
        case Player::RightFlyFarthest:
            sprite = alei.getRAM().get(33);
            break;
        case Player::RightFlyMiddle:
            sprite = alei.getRAM().get(34);
            break;
        case Player::RightFlyClosest:
            sprite = alei.getRAM().get(35);
            break;
        default:
            cout << "ERROR: unknown Demon Attack enemy." << endl;
            return false;
    }

    if (sprite <= 3)
    {
        return false;
    }

    return true;
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
    int shooting = 0;
    int playerTargetingEnemy = 0;

    for (step = 0; !alei.game_over() && step < maxSteps; ++step) 
    {
        float reward = 0;
        
        Mat inputs(1, 3);
        Mat outputs(1, 3);

        double leftFlyFarthestEnemyCoordinateX = (double)Player::demonAttackGetCoordinate(13) / 255.0;
        double leftFlyMiddleEnemyCoordinateX = (double)Player::demonAttackGetCoordinate(14) / 255.0;
        double leftFlyClosestEnemyCoordinateX = (double)Player::demonAttackGetCoordinate(15) / 255.0;

        double playerCoordinateX = (double)Player::demonAttackGetCoordinate(16) / 255.0;

        double rightFlyFarthestEnemyCoordinateX = (double)Player::demonAttackGetCoordinate(17) / 255.0;
        double rightFlyMiddleEnemyCoordinateX = (double)Player::demonAttackGetCoordinate(18) / 255.0;
        double rightFlyClosestEnemyCoordinateX = (double)Player::demonAttackGetCoordinate(19) / 255.0;

        double enemyBulletCoordinateX = (double)Player::demonAttackGetCoordinate(20) / 255.0;

        double currentObjectiveCoordinateX = 0.0;

        if (Player::demonAttackIsEnemyAlive(Player::LeftFlyClosest) && 
            Player::demonAttackIsEnemyAlive(Player::RightFlyClosest))
        {
            int distanceBetweenFlies = Player::demonAttackGetCoordinate(19) - Player::demonAttackGetCoordinate(15);

            if (distanceBetweenFlies == 8 || distanceBetweenFlies == 9)
            {
                // There are no flies or they are very close, so there is no problem in focus them like they were just 1 enemy

                currentObjectiveCoordinateX = (leftFlyClosestEnemyCoordinateX + rightFlyClosestEnemyCoordinateX) / 2.0;
            }
        }
        else if (Player::demonAttackIsEnemyAlive(Player::LeftFlyMiddle) && 
                 Player::demonAttackIsEnemyAlive(Player::RightFlyMiddle))
        {
            int distanceBetweenFlies = Player::demonAttackGetCoordinate(18) - Player::demonAttackGetCoordinate(14);

            if (distanceBetweenFlies == 8 || distanceBetweenFlies == 9)
            {
                // There are no flies or they are very close, so there is no problem in focus them like they were just 1 enemy

                currentObjectiveCoordinateX = (leftFlyMiddleEnemyCoordinateX + rightFlyMiddleEnemyCoordinateX) / 2.0;
            }
        }
        else if (Player::demonAttackIsEnemyAlive(Player::LeftFlyFarthest) && 
                 Player::demonAttackIsEnemyAlive(Player::RightFlyFarthest))
        {
            int distanceBetweenFlies = Player::demonAttackGetCoordinate(17) - Player::demonAttackGetCoordinate(13);

            if (distanceBetweenFlies == 8 || distanceBetweenFlies == 9)
            {
                // There are no flies or they are very close, so there is no problem in focus them like they were just 1 enemy

                currentObjectiveCoordinateX = (leftFlyFarthestEnemyCoordinateX + rightFlyFarthestEnemyCoordinateX) / 2.0;
            }
        }
        else if (Player::demonAttackIsEnemyAlive(Player::LeftFlyClosest))
        {
            currentObjectiveCoordinateX = leftFlyClosestEnemyCoordinateX;
        }
        else if (Player::demonAttackIsEnemyAlive(Player::RightFlyClosest))
        {
            currentObjectiveCoordinateX = rightFlyClosestEnemyCoordinateX;
        }
        else if (Player::demonAttackIsEnemyAlive(Player::LeftFlyMiddle))
        {
            currentObjectiveCoordinateX = leftFlyMiddleEnemyCoordinateX;
        }
        else if (Player::demonAttackIsEnemyAlive(Player::RightFlyMiddle))
        {
            currentObjectiveCoordinateX = rightFlyMiddleEnemyCoordinateX;
        }
        else if (Player::demonAttackIsEnemyAlive(Player::LeftFlyFarthest))
        {
            currentObjectiveCoordinateX = leftFlyFarthestEnemyCoordinateX;
        }
        else if (Player::demonAttackIsEnemyAlive(Player::RightFlyFarthest))
        {
            currentObjectiveCoordinateX = rightFlyFarthestEnemyCoordinateX;
        }

        /*
        double closestObjective = numeric_limits<double>::max();

        if (Player::demonAttackIsEnemyAlive(Player::LeftFlyClosest) || 
            Player::demonAttackIsEnemyAlive(Player::RightFlyClosest))
        {
            if (Player::demonAttackIsEnemyAlive(Player::LeftFlyClosest) && 
                Player::demonAttackIsEnemyAlive(Player::RightFlyClosest))
            {
                int distanceBetweenFlies = Player::demonAttackGetCoordinate(19) - Player::demonAttackGetCoordinate(15);

                if (distanceBetweenFlies == 8 || distanceBetweenFlies == 9)
                {
                    // There are no flies or they are very close, so there is no problem in focus them like they were just 1 enemy

                    //closestObjective = min(closestObjective, abs(playerCoordinateX - (leftFlyClosestEnemyCoordinateX + rightFlyClosestEnemyCoordinateX) / 2.0));
                    if (abs(playerCoordinateX - (leftFlyClosestEnemyCoordinateX + rightFlyClosestEnemyCoordinateX) / 2.0) < closestObjective)
                    {
                        closestObjective = abs(playerCoordinateX - (leftFlyClosestEnemyCoordinateX + rightFlyClosestEnemyCoordinateX) / 2.0);
                        currentObjectiveCoordinateX = (leftFlyClosestEnemyCoordinateX + rightFlyClosestEnemyCoordinateX) / 2.0;
                    }
                }
                else
                {
                    //closestObjective = min(closestObjective, abs(playerCoordinateX - leftFlyClosestEnemyCoordinateX));
                    if (abs(playerCoordinateX - leftFlyClosestEnemyCoordinateX) < closestObjective)
                    {
                        closestObjective = abs(playerCoordinateX - leftFlyClosestEnemyCoordinateX);
                        currentObjectiveCoordinateX = leftFlyClosestEnemyCoordinateX;
                    }
                    //closestObjective = min(closestObjective, abs(playerCoordinateX - rightFlyClosestEnemyCoordinateX));
                    if (abs(playerCoordinateX - rightFlyClosestEnemyCoordinateX) < closestObjective)
                    {
                        closestObjective = abs(playerCoordinateX - rightFlyClosestEnemyCoordinateX);
                        currentObjectiveCoordinateX = rightFlyClosestEnemyCoordinateX;
                    }
                }
            }
            else if (Player::demonAttackIsEnemyAlive(Player::LeftFlyClosest))
            {
                //closestObjective = min(closestObjective, abs(playerCoordinateX - leftFlyClosestEnemyCoordinateX));
                if (abs(playerCoordinateX - leftFlyClosestEnemyCoordinateX) < closestObjective)
                {
                    closestObjective = abs(playerCoordinateX - leftFlyClosestEnemyCoordinateX);
                    currentObjectiveCoordinateX = leftFlyClosestEnemyCoordinateX;
                }
            }
            else if (Player::demonAttackIsEnemyAlive(Player::RightFlyClosest))
            {
                //closestObjective = min(closestObjective, abs(playerCoordinateX - rightFlyClosestEnemyCoordinateX));
                if (abs(playerCoordinateX - rightFlyClosestEnemyCoordinateX) < closestObjective)
                {
                    closestObjective = abs(playerCoordinateX - rightFlyClosestEnemyCoordinateX);
                    currentObjectiveCoordinateX = rightFlyClosestEnemyCoordinateX;
                }
            }
        }
        if (Player::demonAttackIsEnemyAlive(Player::LeftFlyMiddle) || 
            Player::demonAttackIsEnemyAlive(Player::RightFlyMiddle))
        {
            if (Player::demonAttackIsEnemyAlive(Player::LeftFlyMiddle) && 
                Player::demonAttackIsEnemyAlive(Player::RightFlyMiddle))
            {
                int distanceBetweenFlies = Player::demonAttackGetCoordinate(18) - Player::demonAttackGetCoordinate(14);

                if (distanceBetweenFlies == 8 || distanceBetweenFlies == 9)
                {
                    // There are no flies or they are very close, so there is no problem in focus them like they were just 1 enemy

                    //closestObjective = min(closestObjective, abs(playerCoordinateX - (leftFlyMiddleEnemyCoordinateX + rightFlyMiddleEnemyCoordinateX) / 2.0));
                    if (abs(playerCoordinateX - (leftFlyMiddleEnemyCoordinateX + rightFlyMiddleEnemyCoordinateX) / 2.0) < closestObjective)
                    {
                        closestObjective = abs(playerCoordinateX - (leftFlyMiddleEnemyCoordinateX + rightFlyMiddleEnemyCoordinateX) / 2.0);
                        currentObjectiveCoordinateX = (leftFlyMiddleEnemyCoordinateX + rightFlyMiddleEnemyCoordinateX) / 2.0;
                    }
                }
                else
                {
                    //closestObjective = min(closestObjective, abs(playerCoordinateX - leftFlyMiddleEnemyCoordinateX));
                    if (abs(playerCoordinateX - leftFlyMiddleEnemyCoordinateX) < closestObjective)
                    {
                        closestObjective = abs(playerCoordinateX - leftFlyMiddleEnemyCoordinateX);
                        currentObjectiveCoordinateX = leftFlyMiddleEnemyCoordinateX;
                    }
                    //closestObjective = min(closestObjective, abs(playerCoordinateX - rightFlyMiddleEnemyCoordinateX));
                    if (abs(playerCoordinateX - rightFlyMiddleEnemyCoordinateX) < closestObjective)
                    {
                        closestObjective = abs(playerCoordinateX - rightFlyMiddleEnemyCoordinateX);
                        currentObjectiveCoordinateX = rightFlyMiddleEnemyCoordinateX;
                    }
                }
            }
            else if (Player::demonAttackIsEnemyAlive(Player::LeftFlyMiddle))
            {
                //closestObjective = min(closestObjective, abs(playerCoordinateX - leftFlyMiddleEnemyCoordinateX));
                if (abs(playerCoordinateX - leftFlyMiddleEnemyCoordinateX) < closestObjective)
                {
                    closestObjective = abs(playerCoordinateX - leftFlyMiddleEnemyCoordinateX);
                    currentObjectiveCoordinateX = leftFlyMiddleEnemyCoordinateX;
                }
            }
            else if (Player::demonAttackIsEnemyAlive(Player::RightFlyMiddle))
            {
                //closestObjective = min(closestObjective, abs(playerCoordinateX - rightFlyMiddleEnemyCoordinateX));
                if (abs(playerCoordinateX - rightFlyMiddleEnemyCoordinateX) < closestObjective)
                {
                    closestObjective = abs(playerCoordinateX - rightFlyMiddleEnemyCoordinateX);
                    currentObjectiveCoordinateX = rightFlyMiddleEnemyCoordinateX;
                }
            }
        }
        if (Player::demonAttackIsEnemyAlive(Player::LeftFlyFarthest) || 
            Player::demonAttackIsEnemyAlive(Player::RightFlyFarthest))
        {
            if (Player::demonAttackIsEnemyAlive(Player::LeftFlyFarthest) && 
                Player::demonAttackIsEnemyAlive(Player::RightFlyFarthest))
            {
                int distanceBetweenFlies = Player::demonAttackGetCoordinate(17) - Player::demonAttackGetCoordinate(13);

                if (distanceBetweenFlies == 8 || distanceBetweenFlies == 9)
                {
                    // There are no flies or they are very close, so there is no problem in focus them like they were just 1 enemy

                    //closestObjective = min(closestObjective, abs(playerCoordinateX - (leftFlyFarthestEnemyCoordinateX + rightFlyFarthestEnemyCoordinateX) / 2.0));
                    if (abs(playerCoordinateX - (leftFlyFarthestEnemyCoordinateX + rightFlyFarthestEnemyCoordinateX) / 2.0) < closestObjective)
                    {
                        closestObjective = abs(playerCoordinateX - (leftFlyFarthestEnemyCoordinateX + rightFlyFarthestEnemyCoordinateX) / 2.0);
                        currentObjectiveCoordinateX = (leftFlyFarthestEnemyCoordinateX + rightFlyFarthestEnemyCoordinateX) / 2.0;
                    }
                }
                else
                {
                    //closestObjective = min(closestObjective, abs(playerCoordinateX - leftFlyFarthestEnemyCoordinateX));
                    if (abs(playerCoordinateX - leftFlyFarthestEnemyCoordinateX) < closestObjective)
                    {
                        closestObjective = abs(playerCoordinateX - leftFlyFarthestEnemyCoordinateX);
                        currentObjectiveCoordinateX = leftFlyFarthestEnemyCoordinateX;
                    }
                    //closestObjective = min(closestObjective, abs(playerCoordinateX - rightFlyFarthestEnemyCoordinateX));
                    if (abs(playerCoordinateX - rightFlyFarthestEnemyCoordinateX) < closestObjective)
                    {
                        closestObjective = abs(playerCoordinateX - rightFlyFarthestEnemyCoordinateX);
                        currentObjectiveCoordinateX = rightFlyFarthestEnemyCoordinateX;
                    }
                }
            }
            else if (Player::demonAttackIsEnemyAlive(Player::LeftFlyFarthest))
            {
                //closestObjective = min(closestObjective, abs(playerCoordinateX - leftFlyFarthestEnemyCoordinateX));
                if (abs(playerCoordinateX - leftFlyFarthestEnemyCoordinateX) < closestObjective)
                {
                    closestObjective = abs(playerCoordinateX - leftFlyFarthestEnemyCoordinateX);
                    currentObjectiveCoordinateX = leftFlyFarthestEnemyCoordinateX;
                }
            }
            else if (Player::demonAttackIsEnemyAlive(Player::RightFlyFarthest))
            {
                //closestObjective = min(closestObjective, abs(playerCoordinateX - rightFlyFarthestEnemyCoordinateX));
                if (abs(playerCoordinateX - rightFlyFarthestEnemyCoordinateX) < closestObjective)
                {
                    closestObjective = abs(playerCoordinateX - rightFlyFarthestEnemyCoordinateX);
                    currentObjectiveCoordinateX = rightFlyFarthestEnemyCoordinateX;
                }
            }
        }

        if (UtilG::compareDouble(closestObjective, numeric_limits<double>::max()))
        {
            closestObjective = 0.0;
        }

        //currentObjectiveCoordinateX = closestObjective;
        */

        if (Player::enemyBulletCoordinateXHistory.size() < 3)
        {
            Player::enemyBulletCoordinateXHistory.push_back(Player::demonAttackGetCoordinate(20));
            Player::enemyBulletCoordinateXHistoryIndex = (Player::enemyBulletCoordinateXHistoryIndex + 1) % 3;
        }
        else
        {
            Player::enemyBulletCoordinateXHistory[Player::enemyBulletCoordinateXHistoryIndex] = Player::demonAttackGetCoordinate(20);
            Player::enemyBulletCoordinateXHistoryIndex = (Player::enemyBulletCoordinateXHistoryIndex + 1) % 3;
        }

        if (Player::enemyBulletCoordinateXHistory[0] != Player::enemyBulletCoordinateXHistory[1] && 
            Player::enemyBulletCoordinateXHistory[0] != Player::enemyBulletCoordinateXHistory[2] && 
            Player::enemyBulletCoordinateXHistory[1] != Player::enemyBulletCoordinateXHistory[2]
            )
        {
            // The fly that is a bullet is alive

            currentObjectiveCoordinateX = enemyBulletCoordinateX;

            enemyBulletCoordinateX = 0.0;
        }
        else if (!Player::demonAttackIsEnemyAlive(Player::LeftFlyClosest) && !Player::demonAttackIsEnemyAlive(Player::RightFlyClosest))
        {
            // The only enemy that shoots is dead

            enemyBulletCoordinateX = 0.0;
        }

        if (currentObjectiveCoordinateX > 1.0 || currentObjectiveCoordinateX < -1.0 ||
            playerCoordinateX > 1.0 || playerCoordinateX < -1.0 ||
            enemyBulletCoordinateX > 1.0 || enemyBulletCoordinateX < -1.0)
        {
            cout << "ERROR: some variable is not in the expected range!" << endl;
            cout << "currentObjectiveCoordinateX = " << currentObjectiveCoordinateX << endl;
            cout << "playerCoordinateX = " << playerCoordinateX << endl;
            cout << "enemyBulletCoordinateX = " << enemyBulletCoordinateX << endl;
        }

        int distanceFactor = 10;

        if (!UtilG::compareDouble(0.0, currentObjectiveCoordinateX) && 
            abs(currentObjectiveCoordinateX * 255.0 - playerCoordinateX * 255.0) < distanceFactor)
        {
            playerTargetingEnemy++;
        }

        //cout << "currentObjectiveCoordinateX = " << currentObjectiveCoordinateX * 255.0 << endl;

        inputs.set(0, 0, currentObjectiveCoordinateX);
        inputs.set(0, 1, playerCoordinateX);
        inputs.set(0, 2, enemyBulletCoordinateX);

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
                shooting++;
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

        // Early stopping
        if (step > maxSteps / 2 && (shooting == 0 || min(moveLeft, moveRight) == 0))
        {
            break;
        }
    }

    score = (int)totalReward;

    res.push_back(score);
    res.push_back(step);
    res.push_back(moveLeft);
    res.push_back(moveRight);
    res.push_back(shooting);
    res.push_back(playerTargetingEnemy);

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