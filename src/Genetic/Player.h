#ifndef _Player_
#define _Player_

#include "../NeuralNetwork/neuralNetwork.h"

class Player
{
    private:
        static int breakoutGetPlayerX();
        static int breakoutGetBallX();

    public:
        static int playBreakout(NeuralNetwork&);
    
};

#endif