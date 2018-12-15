#include <iostream>
#include <cmath>
#include "ale_interface.hpp"
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>

#include "SDL.h"

#include "../DataLoader/DataLoader.h"
#include "../NeuralNetwork/neuralNetwork.h"

using namespace std;

// Global vars
const int maxSteps(15000);
int lastLives;
float totalReward;
ALEInterface alei;
bool manualInput(false);
time_t lastTimeChangedMode(std::time(0));
vector<int> lastRAM(128);
int BallX_LastTick(0);
int tpl[] = { 4, 1 };
int epochs = 1;
vector<int> topology(tpl, tpl + sizeof(tpl)/sizeof(int));

///////////////////////////////////////////////////////////////////////////////
/// Print usage and exit
///////////////////////////////////////////////////////////////////////////////
void usage(char* pname)
{
   std::cerr
      << "\nUSAGE:\n" 
      << "   " << pname << " <romfile> "<< "<display media?([0],1)> <Print ram?([0],1)>\n";
   exit(-1);
}

void printRAM()
{
    system("clear");

    for (int i = 0; i < 128; i++)
    {
        if (i % 8 == 0)
        {
            std::cout << std::endl;
        }

        cout << "[";

        if (i < 10)
        {
            cout << "  ";
        }
        else if (i < 100)
        {
            cout << " ";
        }

        cout << i << "]";

        stringstream ramValue;
        int intRamValue = (int)alei.getRAM().get(i);
        ramValue << hex << intRamValue;

        // ANSI colour codes
        if (intRamValue == lastRAM[i])
        {
            // Red color
            cout << "\033[1;31m";
        }
        else
        {
            // Green color
            cout << "\033[1;32m";
        }

        // Same length for al cases
        if (intRamValue < 0x10)
        {
            cout << "0";
        }

        cout << ramValue.str() << "\t";

        // ANSI colour code close
        cout << "\033[0m";

        lastRAM[i] = intRamValue;
    }
}

// Keys: https://wiki.libsdl.org/SDL_Keycode
void checkKeys()
{
    Uint8* keystate = SDL_GetKeyState(NULL);

    // It checks if key 'e' has been pressed to change to manual mode
    if(keystate[SDLK_e] && (time(0) - lastTimeChangedMode) >= 1)
    {
        if (manualInput)
        {
            cout << "[UDL] Bot Control" << endl;
        }
        else
        {
            cout << "[UDL] Manual Control" << endl;
        }

        manualInput = !manualInput;

        lastTimeChangedMode = time(0);
    }
}

float manualMode()
{
    Uint8* keystate = SDL_GetKeyState(NULL);
    float reward = 0;

    if(keystate[SDLK_SPACE] && alei.lives() != lastLives)
    {
        --lastLives;
        alei.act(PLAYER_A_FIRE);
    }

    if(keystate[SDLK_LEFT])
    {
        reward += alei.act(PLAYER_A_LEFT);
    }
    if(keystate[SDLK_RIGHT])
    {
        reward += alei.act(PLAYER_A_RIGHT);;
    }

    return (reward + alei.act(PLAYER_A_NOOP));
}

///////////////////////////////////////////////////////////////////////////////
/// Get info from RAM
///////////////////////////////////////////////////////////////////////////////
int getPlayerX()
{
   return alei.getRAM().get(72);// + ((rand() % 3) - 1);
}

int getBallX()
{
   return alei.getRAM().get(99);// + ((rand() % 3) - 1);
}

///////////////////////////////////////////////////////////////////////////////
/// Do next agent step predicted by the Network
///////////////////////////////////////////////////////////////////////////////
float NN(DataLoader &d)
{
    int wide = alei.getRAM().get(108);
    float reward = 0;

    if (alei.lives() != lastLives)
    {
        --lastLives;
        alei.act(PLAYER_A_FIRE);
    }

    int playerX = getPlayerX();
    int ballX = getBallX();
    double direction = 0.5;
    
    if (BallX_LastTick < ballX) {
        direction = 0.0;
    }
    if (BallX_LastTick > ballX) {
        direction = 1.0;
    }


    vector<double> inputs;
    vector<double> outputs;

    inputs.push_back(ballX);
    inputs.push_back(direction);
    inputs.push_back(playerX);
    inputs.push_back(wide);

    outputs = d.getPredictionJNet(inputs);

    //Mat m = nn.forwardPropagation(values);

    if (outputs[0] < 0.5)
    {
        reward += alei.act(PLAYER_A_RIGHT);
    }
    else
    {
        reward += alei.act(PLAYER_A_LEFT);
    }

    BallX_LastTick = getBallX();

    return (reward + alei.act(PLAYER_A_NOOP));
}



///////////////////////////////////////////////////////////////////////////////
/// MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Check input parameter
    if (argc < 2)
        usage(argv[0]);

    ////////////////////////////////////////
    // argv[1] : rom
    // argv[2] : media? true/>false<
    // argv[3] : print_ram? true/>false<
    ////////////////////////////////////////

    const bool display_media(argc >= 3 ? atoi(argv[2])==1 : false);
    const bool printRam(argc >= 4 ? atoi(argv[3])==1 : false);

    DataLoader d("../breakout/breakout.csv", topology);
    d.trainJNet(topology.front(),topology.back(),epochs);
    //NeuralNetwork &nn = *d.getNN();

    // Init rand seed
    srand(time(NULL));

    // Create alei object.
    alei.setInt("random_seed", rand()%1000);
    alei.setFloat("repeat_action_probability", 0);
    alei.setBool("sound", display_media);
    alei.setBool("display_screen", display_media);
    alei.loadROM(argv[1]);

    // Init
    lastLives = alei.lives();
    totalReward = .0f;

    // Main loop
    alei.act(PLAYER_A_FIRE);
    int step;

    //////////////////////////////////////////////////////////////////////////
    // Bot expl: This bot will try to adjust the player location based on the 
    // last tick ball location and the current tick location
    //////////////////////////////////////////////////////////////////////////
    BallX_LastTick = getBallX();   
    for (step = 0; !alei.game_over() && step < maxSteps; ++step) 
    {
        // Debug mode ***********************************
        if(printRam) printRAM();
        if(display_media) checkKeys();
        // **********************************************

        // Total reward summation
        totalReward += NN(d);
    }

    std::cout << "Steps: " << step << std::endl;
    std::cout << "Reward: " << totalReward << std::endl;

    return 0;
}
