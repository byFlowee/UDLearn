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
vector<unsigned char> grayscale;
const int LINE_WIDTH(160);          // Amount of pixels in a line
const int SHIP_WIDTH(10);           // Approximate width in pixels of the ship
const int LEFT_THRESOLD(18);        // MIN X coordinate the ship can move
const int RIGHT_THRESOLD(141);      // MAX X coordinate the ship can move

// Handlers for dirty state
#define RIGHT_DIR true              
#define LEFT_DIR false


void write(double d)
{
    if (toCSV) csv << to_string(d);
}

void write(string s)
{
    if (toCSV) csv << s;
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
    int const val(alei.getRAM().get(16));
    // player position hex nibbles are inverted so we turn it around
    return (((val & 0xf0) >> 4) + ((val & 0x0f)*16)); // + ((rand() % 2) - 1);
}

///////////////////////////////////////////////////////////////////////////////
/// 1D Collision check given a screen line
///////////////////////////////////////////////////////////////////////////////
bool isBlockingHit(int line) {
    int const FILTER_LINE(LINE_WIDTH*line);
    for(int i = 0; i < SHIP_WIDTH; ++i) {
        if(grayscale[FILTER_LINE + getP1_X() + i] != 0) {
            return true;
        }
    }
    return false;
}

float manualMode()
{
    system("clear");
    bool bBlockingHit(false);
    for(int line = 150; line > 60; --line) {
        bBlockingHit = isBlockingHit(line); 
        if(bBlockingHit) break;
    }
    //cout << "Is Blocking Hit: " << ((bBlockingHit) ? "true" : "false") << endl;

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

    //cout << "Player position = " << getP1_X() << endl;

    return (reward + alei.act(PLAYER_A_NOOP));
}




///////////////////////////////////////////////////////////////////////////////
/// Do Next Agent Step
///////////////////////////////////////////////////////////////////////////////
struct DirtyState {
    bool Dirty;
    bool Direction; // 0 left 1 right
    DirtyState(bool bDirty, bool bDirection) 
    : Dirty(bDirty), Direction(bDirection) { }
};

DirtyState ds(false, false);
bool flipflop(true);
float agentStep()
{
    // get screen information
    alei.getScreenGrayscale(grayscale);
    
    // Reseting variables
    float reward = 0;
    ds.Dirty = false;
    flipflop = !flipflop;

    // Iterating from bottom line to top line which defines a vision rectangle (see is BlockingHit)
    for(int line = 170; line > 60; --line) {
        bool const bBlockingHit(isBlockingHit(line)); 
        //cout << "Is Blocking Hit: " << ((bBlockingHit) ? "true" : "false") << endl;
        if (bBlockingHit && !ds.Dirty)
        {   
            /**
            * Dirtying the warning state, now we have to avoid every enemy and undesired object
            * Random direction for the bot to pick when the dirty state is set
            **/
            ds.Dirty = true; ds.Direction = rand()%2; 
            break;
        } else if (bBlockingHit) {
            /**
            * Theorically at this point the direction is already set so we don't have to re-set it here
            * we just mark the dirty state to true
            **/
            ds.Dirty = true; 
            break;
        }
    }

    /***************************************************
    * Blocking hit detected at this point
    ***************************************************/

    // Frame intercaling for firing
    if(flipflop) 
    {
        reward+=alei.act(PLAYER_A_FIRE);
    } 
    else 
    {
        if(ds.Dirty) 
        {
            // Left thresold
            if(getP1_X() == LEFT_THRESOLD) {
                // If we cannot move more to the left we change direction
                ds.Direction = RIGHT_DIR;
            }
            // Right thresold
            else if(getP1_X() == RIGHT_THRESOLD) {
                // If we cannot move more to the right we change direction
                ds.Direction = LEFT_DIR;   
            }
            reward+= ds.Direction ? alei.act(PLAYER_A_RIGHT) : alei.act(PLAYER_A_LEFT);      
        } else {
            // Here we are safe, don't move but keep shooting
            reward+=alei.act(PLAYER_A_FIRE);
            // Here we would need to mark dirty a false but it is defaulted in every iteration to false
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
