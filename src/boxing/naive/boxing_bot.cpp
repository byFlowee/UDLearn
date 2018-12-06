#include <iostream>
#include <cmath>
#include "ale_interface.hpp"
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "SDL.h"

using namespace std;

// Global vars
const int maxSteps(7500);
float totalReward;
ALEInterface alei;
bool manualInput(false);
int lastLives;
time_t lastTimeChangedMode(std::time(0));
vector<int> lastRAM(128);
ofstream csv;
bool toCSV = false;

// refactor
const int thresold_y(0);
const int thresold_x(25);

/**
* Player position struct
**/
struct player_pos {
    int x;
    int y;

    player_pos(int nx, int ny) : 
    x(nx), y(ny) { }
};

void write(double d)
{
    if (toCSV)
    {
        csv << to_string(d);
    }
}

void write(string s)
{
    if (toCSV)
    {
        csv << s;
    }
}


///////////////////////////////////////////////////////////////////////////////
/// Print usage and exit
///////////////////////////////////////////////////////////////////////////////
void usage(char* pname)
{
   std::cerr
      << "\nUSAGE:\n" 
      << "   " << pname << " <romfile> "<< "<display media?([0],1)> <Print ram?([0],1)> <Dump output?([0],1)>\n";
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

    if(keystate[SDLK_SPACE])
    {
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
    if(keystate[SDLK_UP])
    {
        reward += alei.act(PLAYER_A_UP);
    }
    if(keystate[SDLK_DOWN])
    {
        reward += alei.act(PLAYER_A_DOWN);
    }

    return (reward + alei.act(PLAYER_A_NOOP));
}

///////////////////////////////////////////////////////////////////////////////
/// Get info from RAM
///////////////////////////////////////////////////////////////////////////////
int getP1_X()
{
   return alei.getRAM().get(32) + ((rand() % 2) - 1);
}

int getP1_Y() // refactor
{
   return alei.getRAM().get(34) + ((rand() % 5) - 1);
}

int getP2_X()
{
   return alei.getRAM().get(33) + ((rand() % 2) - 1);
}

int getP2_Y() // refactor
{
   return alei.getRAM().get(35) + ((rand() % 5) - 1);
}

///////////////////////////////////////////////////////////////////////////////
/// Do Next Agent Step
///////////////////////////////////////////////////////////////////////////////
float agentStep()
{
    float reward = 0;

    // Apply rules.
    player_pos p1(getP1_X(), getP1_Y()); // cout << "P1 x: " << p1.x << " y: " << p1.y << endl;
    player_pos p2(getP2_X(), getP2_Y()); // cout << "P2 x: " << p2.x << " y: " << p2.y << endl;
    const int abs_p1_p2_x(abs(p1.x - p2.x));
    const int abs_p1_p2_y(abs(p1.y - p2.y));

    if(abs_p1_p2_y > 3 && abs_p1_p2_y < 20) 
    {
        reward += alei.act(PLAYER_A_FIRE);
    }
    else 
    {
        if(abs_p1_p2_x > 25 && abs_p1_p2_x < 40) 
        {
            reward += alei.act(PLAYER_A_RIGHT); //cout << "ABS: " << (abs_p1_p2_x > 25) << endl;
        } 
        else 
        {
            reward += (p1.y > p2.y) ? alei.act(PLAYER_A_UP) : alei.act(PLAYER_A_DOWN);
        }
    }
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
    const bool printRam(argc >= 4 ? atoi(argv[3])==1 : false);
    toCSV = argc == 5 ? atoi(argv[4])==1 : false;

	if (toCSV)
    {
        csv.open("../boxing.csv");
        // write("ballX,ballDirection,playerX,playerWidth,userInput\n"); TODO
    }
	
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
    int step;

    /*
    * Bot expl: 
    **/
    for (step = 0; !alei.game_over() && step < maxSteps; ++step) 
    {
        // Debug mode ***********************************
        if(printRam) printRAM();
        if(display_media) checkKeys();
        // **********************************************

        // Total reward summation
        totalReward += manualInput ? manualMode() : agentStep();
   }

   std::cout << "Steps: " << step << std::endl;
   std::cout << "Reward: " << totalReward << std::endl;
   
	csv.close();
   
   return 0;
}
