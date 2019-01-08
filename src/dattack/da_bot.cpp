#include <iostream>
#include <cmath>
#include "ale_interface.hpp"
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#include "SDL.h"

#include "../Technologies/DataLoader/DataLoader.h"

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
vector<int> topology = {123, 20, 3};
int epochs = 10;



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

/*
 ***************************************************************************************
 * *************************************************************************************
 * *************************************************************************************
 *  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
 *  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
 *  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
 * *************************************************************************************
 * *************************************************************************************
 ***************************************************************************************
 *
int currentValueOfRAM = 0;
int stepsInitialization = 1;
vector<int> freezePositions;
vector<int> freezeValues;
bool checkRAM = false;
int positionToDisplay = -1;

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
    int oldValue = alei.getRAM().get(currentValueOfRAM);
    bool skip = false;
    bool changeValue = true;

    stepsInitialization = 1;
    positionToDisplay = -1;

    do
    {
        if (newValue >= 256)
        {
            newValue = 0;
        }

        for (size_t i = 0; i < freezePositions.size(); i++)
        {
            byte_t *freeze = alei.getRAM().array() + sizeof(byte_t) * freezePositions[i];

            *freeze = freezeValues[i];

            alei.processBackRAM();
        }

        cout << endl;
        cout << "RAM position = " << currentValueOfRAM << endl;
        cout << "RAM[" << currentValueOfRAM << "] = " << (int)*byte << endl;

        if (changeValue)
        {
            *byte = (byte_t)newValue;
            alei.processBackRAM();
        }
        
        alei.act(PLAYER_A_NOOP);

        cout << "New value of RAM[" << currentValueOfRAM << "] = " << (int)*byte << endl;

        --steps;

        if (steps <= 0)
        {
            skip = false;
            changeValue = true;

            cout << endl;
            cout << "Write \"next\", \"exit\", \"set<VALUE>\" \"play[POSITION_TO_DISPLAY]\", \"reset\", \"freeze<POSITION>\" \"skip<NUMBER>\", \"shoot\" or \"goto<NUMBER>\" or a number of steps: " << endl;
            cin >> next;

            if (next.substr(0, 4) != "play" && next.substr(0, 3) != "set" && next.substr(0, 6) != "freeze" && next.substr(0, 4) != "next" && next != "exit" && next.substr(0, 4) != "goto" && next != "shoot" && next != "reset" && next.substr(0, 4) != "skip")
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
            else if (next == "shoot")
            {
                alei.act(PLAYER_A_FIRE);
                steps = 1;
                skip = true;
            }
            else if (next.substr(0, 6) == "freeze")
            {
                //int freezePosition = atoi(next.substr(6, next.size()).c_str());
                int freezePosition = next.find("-");
                int endFreezePosition = 1;

                if (freezePosition != string::npos)
                {
                    endFreezePosition =  atoi(next.substr(freezePosition + 1, next.size()).c_str()) - atoi(next.substr(6, freezePosition).c_str()) + 1;
                    freezePosition = atoi(next.substr(6, freezePosition).c_str());
                }
                else
                {
                    freezePosition = atoi(next.substr(6, next.size()).c_str());
                }

                for (int i = 0; i < endFreezePosition; i++)
                {
                    if(std::find(freezePositions.begin(), freezePositions.end(), freezePosition + i) == freezePositions.end() && freezePosition + i >= 0 && freezePosition + i <= 255)
                    {
                        freezePositions.push_back(freezePosition + i);
                        freezeValues.push_back(alei.getRAM().get(freezePosition + i));

                        cout << "RAM(" << freezePosition + i << ") is frozen" << endl;
                    }
                }

                steps = 1;
                skip = true;
            }
            else if (next.substr(0, 3) == "set")
            {
                int valueToSet = atoi(next.substr(3, next.size()).c_str());

                if (valueToSet >= 0 && valueToSet <= 255)
                {
                    *byte = (byte_t)valueToSet;
                    alei.processBackRAM();

                    steps = 1;
                    changeValue = false;
                    skip = true;

                    newValue = valueToSet;
                }
            }
        }

        if (!skip)
        {
            newValue++;
        }
    }
    while (next.substr(0, 4) != "play" && next.substr(0, 4) != "next" && next != "exit" && next.substr(0, 4) != "goto");

    if (next == "exit")
    {
        exit(0);
    }
    else if (next.substr(0, 4) == "goto")
    {
        currentValueOfRAM = atoi(next.substr(4, next.size()).c_str()) - 1;
    }
    else if (next.substr(0, 4) == "play")
    {
        checkRAM = false;
        currentValueOfRAM = 0;
        stepsInitialization = 1;

        if (next.size() > 4)
        {
            positionToDisplay = atoi(next.substr(4, next.size()).c_str());
        }
    }

    *byte = (byte_t)oldValue;
    alei.processBackRAM();
    alei.act(PLAYER_A_NOOP);

    currentValueOfRAM++;
}
*
***************************************************************************************
* *************************************************************************************
* *************************************************************************************
*  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
*  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
*  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
* *************************************************************************************
* *************************************************************************************
***************************************************************************************
*
*/

int getChar_X(int char_id)
{
    // player position hex nibbles are inverted so we turn it around, same with the first nibble which seems to be desync with the second nibble
    int const val(alei.getRAM().get(char_id));
    int const rawFirstNibble((7-((val & 0xF0)>>4)) & 0x0F);
    int const rawSecondNibble(val & 0x0F);
    return (rawSecondNibble*16) + rawFirstNibble; 
}

int min123 = 99999;
int max123 = -min123;
vector<int> enemyBulletCoordinateXHistory = {};
int enemyBulletCoordinateXHistoryIndex = 0;

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

    /*
     ***************************************************************************************
     * *************************************************************************************
     * *************************************************************************************
     *  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
     *  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
     *  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
     * *************************************************************************************
     * *************************************************************************************
     ***************************************************************************************
     *
    if (positionToDisplay != -1 && positionToDisplay >= 0 && positionToDisplay <= 127)
    {
        if (positionToDisplay == 72)
        {
            int value123 = alei.getRAM().get(positionToDisplay);

            if (value123 > max123)
            {
                max123 = value123;
            }
            if (value123 < min123)
            {
                min123 = value123;
            }

            cout << "RAM[" << positionToDisplay << "] = " << (int)alei.getRAM().get(positionToDisplay) << endl;
        }
        else if (positionToDisplay == 111 || positionToDisplay >= 13 && positionToDisplay <= 20)
        {
            int fix = getChar_X(positionToDisplay);

            cout << "FIXED" << endl;
            cout << "RAM[" << positionToDisplay << "] = " << fix << endl;

            if (fix > max123)
            {
                max123 = fix;
            }
            if (fix < min123)
            {
                min123 = fix;
            }
        }
        else
        {
            cout << "RAM[" << positionToDisplay << "] = " << (int)alei.getRAM().get(positionToDisplay) << endl;
        }
    }

    if(checkRAM || keystate[SDLK_k])
    {
        cout << "min123 = " << min123 << endl;
        cout << "max123 = " << max123 << endl;

        min123 = 99999;
        max123 = -min123;

        checkRAM = true;
        checkAllValuesOfRAM();
    }

    if (!checkRAM)
    {
        for (size_t i = 0; i < freezePositions.size(); i++)
        {
            byte_t *freeze = alei.getRAM().array() + sizeof(byte_t) * freezePositions[i];

            *freeze = freezeValues[i];

            alei.processBackRAM();
        }
    }
    *
    ***************************************************************************************
    * *************************************************************************************
    * *************************************************************************************
    *  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
    *  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
    *  NOT ERASE: code only available when method alei.processBackRAM() is implemented! ***
    * *************************************************************************************
    * *************************************************************************************
    ***************************************************************************************
    */

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
    d.trainNN(topology.front(),topology.back(), epochs);
    //cout << "Training completed, starting game..." << endl;

    // Init rand seed
    srand(time(NULL));

    // Create alei object.
    alei.setInt("random_seed", rand()%1000);
    alei.setFloat("repeat_action_probability", 0);
    alei.setBool("sound", false);
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
        //totalReward += NN(d);
        totalReward += manualMode();
   }

   std::cout << "Steps: " << step << std::endl;
   std::cout << "Reward: " << totalReward << std::endl;
   return 0;
}
