#ifndef _Player_
#define _Player_

#include <vector>
#include <string>

#include "../NeuralNetwork/neuralNetwork.h"

class Player
{
    private:
        static const string BREAKOUT_ROM;
        static const string BOXING_ROM;
        static const string DEMONATTACK_ROM;
        static const string STARGUNNER_ROM;

        static int breakoutGetPlayerX();
        static int breakoutGetBallX();

        enum DemonAttackEnemy
        {
            LeftFlyFarthest,
            LeftFlyMiddle,
            LeftFlyClosest,
            RightFlyFarthest,
            RightFlyMiddle,
            RightFlyClosest
        };

        static vector<int> enemyBulletCoordinateXHistory;
        static int enemyBulletCoordinateXHistoryIndex;

        static int demonAttackGetCoordinate(int);
        static bool demonAttackIsEnemyAlive(DemonAttackEnemy);

    public:
        static vector<int> playBreakout(NeuralNetwork&, bool = false, int = 7500);
        static vector<int> playBoxing(NeuralNetwork&, bool = false, int = 7500);
        static vector<int> playDemonAttack(NeuralNetwork&, bool = false, int = 7500);
        static vector<int> playStarGunner(NeuralNetwork&, bool = false, int = 7500);
    
};

#endif