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
const int LINE_WIDTH(160);
const int SHIP_WIDTH(10);
vector<unsigned char> grayscale;


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

void dumpLine(int TARGET_LINE) {
    int const FILTER_LINE(LINE_WIDTH*TARGET_LINE);
    for(int i = FILTER_LINE; i < FILTER_LINE + LINE_WIDTH; ++i) {
        int a(grayscale[i]);
        cout << a << ((i%160==0) ? "\n" : "");
    }
    system("clear");
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

///////////////////////////////////////////////////////////////////////////////
/// Get info from RAM
///////////////////////////////////////////////////////////////////////////////
int getP1_X()
{
    int val(alei.getRAM().get(16));
    // player position hex is inverted so we turn it around
    return (((val & 0xf0) >> 4) + ((val & 0x0f)*16)) /* + ((rand() % 2) - 1)*/;
}

///////////////////////////////////////////////////////////////////////////////
/// 1D Collision check given a screen line
///////////////////////////////////////////////////////////////////////////////
bool isBlockingHit(int line) {
    int const FILTER_LINE(LINE_WIDTH*line);
    for(int i = 0; i < SHIP_WIDTH; ++i) {
        if(grayscale[FILTER_LINE + getP1_X() + i] != 0) {
            // cout << "Detected shoot";
            int hola(grayscale[FILTER_LINE + getP1_X() + i]);
            cout << hola << endl;
            return true;
        }
    }
    return false;
}

float manualMode()
{
    alei.getScreenGrayscale(grayscale);
    for(int line = 170; line > 30; --line) {
        int const FILTER_LINE(LINE_WIDTH*line);
        if (isBlockingHit(line))
        {
            break;
        } 
    }


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
/// Do Next Agent Step
///////////////////////////////////////////////////////////////////////////////
bool flipflop(true);
float agentStep()
{
    // get screen information
    alei.getScreenGrayscale(grayscale);
    float reward = 0;

    for(int line = 170; line > 30; --line) {
        int const FILTER_LINE(LINE_WIDTH*line);
        if (isBlockingHit(line))
        {
            if(grayscale[FILTER_LINE + getP1_X() - 1] == 0) {
                reward+=alei.act(PLAYER_A_LEFT);    
            }
            else if(grayscale[FILTER_LINE + getP1_X() + SHIP_WIDTH + 1] == 0) {
                reward+=alei.act(PLAYER_A_RIGHT);    
            } 
            break;
        } else {
            flipflop = !flipflop;    
            if(flipflop) {
                reward+=alei.act(PLAYER_A_FIRE);
            } else {
                //reward+= (rand()%2) ? alei.act(PLAYER_A_LEFT) : alei.act(PLAYER_A_RIGHT);    
            }
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
    alei.setBool("sound", false);
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
