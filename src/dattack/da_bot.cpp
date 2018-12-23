#include <iostream>
#include <cmath>
#include "ale_interface.hpp"
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#include "SDL.h"

#include "../DataLoader/DataLoader.h"

using namespace std;

// Global vars
const int maxSteps(7500);
int lastLives;
float totalReward;
ALEInterface alei;
bool manualInput(false);
time_t lastTimeChangedMode(std::time(0));
vector<int> lastRAM(128);
int BallX_LastTick(0);
vector<int> topology = {123, 20, 10, 3};
int epochs = 3;



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

int currentValueOfRAM = 0;
int stepsInitialization = 1;

void checkAllValuesOfRAM()
{
    if (currentValueOfRAM >= 128)
    {
        currentValueOfRAM = 0;
    }

    byte_t *byte = alei.getRAM().array() + sizeof(byte_t) * currentValueOfRAM;
    string next = "";
    int newValue = alei.getRAM().get(currentValueOfRAM);
    int steps = stepsInitialization;
    int oldValue = alei.getRAM().get(currentValueOfRAM);;
    bool skip = false;
    bool changeValue = true;

    stepsInitialization = 1;

    do
    {
        if (newValue >= 256)
        {
            newValue = 0;
        }

        cout << endl;
        cout << "RAM position = " << currentValueOfRAM << endl;
        cout << "RAM(" << currentValueOfRAM << ") = " << (int)*byte << endl;

        if (changeValue)
        {
            *byte = (byte_t)newValue;
            alei.processBackRAM();
        }
        
        alei.act(PLAYER_A_NOOP);

        cout << "New value of RAM(" << currentValueOfRAM << ") = " << (int)alei.getRAM().get(currentValueOfRAM) << endl;

        --steps;

        if (steps <= 0)
        {
            skip = false;
            changeValue = true;

            cout << endl;
            cout << "Write \"next\", \"exit\", \"reset\", \"skip<NUMBER>\" or \"goto<NUMBER>\" to exit or a number of steps: ";
            cin >> next;

            if (next.substr(0, 4) != "next" && next != "exit" && next.substr(0, 4) != "goto" && next != "reset" && next.substr(0, 4) != "skip")
            {
                steps = atoi(next.c_str());
            }
            else if (next.substr(0, 4) == "next" && next.size() > 4)
            {
                stepsInitialization = atoi(next.substr(4, next.size()).c_str());
            }
            else if (next.substr(0, 4) == "skip" && next.size() > 4)
            {
                steps = atoi(next.substr(4, next.size()).c_str());
                skip = true;
            }
            else if (next == "reset")
            {
                *byte = (byte_t)oldValue;
                skip = true;
                changeValue = false;
                stepsInitialization = 1;
                newValue = oldValue;
                alei.processBackRAM();
            }
        }

        if (!skip)
        {
            newValue++;
        }
    }
    while (next.substr(0, 4) != "next" && next != "exit" && next.substr(0, 4) != "goto");

    if (next == "exit")
    {
        exit(0);
    }
    else if (next.substr(0, 4) == "goto")
    {
        currentValueOfRAM = atoi(next.substr(4, next.size()).c_str()) - 1;
    }

    *byte = (byte_t)oldValue;
    alei.processBackRAM();
    alei.act(PLAYER_A_NOOP);

    currentValueOfRAM++;
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
        reward += alei.act(PLAYER_A_RIGHT);
    }

    //checkAllValuesOfRAM();

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
/// Do Next Agent Step
///////////////////////////////////////////////////////////////////////////////
float NN(DataLoader d)
{
    int reward = 0;
    vector<double> ramValues;
    vector<double> outputs;

    for(unsigned i = 5; i < 128; ++i) {
        ramValues.push_back(alei.getRAM().get(i));
    }

    cout << "Getting prediction..." << endl;
    outputs = d.getPrediction(ramValues);
    //cout << outputs.size() << " " << outputs[0] << " " << outputs[1] << " " << outputs[2] << endl;
    
    //int maxElementIndex = max_element(ramValues.begin(),ramValues.end()) - ramValues.begin();
    
    
    switch (4)
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
            break;
    }
    

    cout << "Reward: " << reward << endl;

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

    /** 
    * argv[1] : rom
    * argv[2] : media? true/>false<
    * argv[3] : print_ram? true/>false<
    **/
    const bool display_media(argc >= 3 ? atoi(argv[2])==1 : false);
    const bool printRam(argc == 4 ? atoi(argv[3])==1 : false);

    DataLoader d("../dattack/demon_attack.csv", topology);
    d.trainJNet(topology.front(),topology.back(),epochs);
    cout << "Training completed, starting game..." << endl;

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

    /*
    * Bot expl: This bot will try to adjust the player location based on the 
    * last tick ball location and the current tick location
    **/
    BallX_LastTick = getBallX();   
    for (step = 0; !alei.game_over() && step < maxSteps; ++step) 
    {
        // Debug mode ***********************************
        if(printRam) printRAM();
        if(display_media) checkKeys();
        // **********************************************

        // Total reward summation
        totalReward += NN(d);
        //totalReward += manualMode();
   }

   std::cout << "Steps: " << step << std::endl;
   std::cout << "Reward: " << totalReward << std::endl;
   return 0;
}
