#ifndef _Player_
#define _Player_

#include <string>

#include "../NeuralNetwork/neuralNetwork.h"

class Player
{
    private:
        static const string BREAKOUT_ROM;

        static int breakoutGetPlayerX();
        static int breakoutGetBallX();

    public:
        static int playBreakout(NeuralNetwork&);
    
};

#endif