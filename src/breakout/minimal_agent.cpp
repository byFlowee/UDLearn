#include <iostream>
#include <cmath>
#include "ale_interface.hpp"
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>

#include "SDL.h"
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
ofstream csv;
bool toCSV = false;

std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str)
{
    std::vector<std::string>   result;
    std::string                line;
    std::getline(str,line);

    std::stringstream          lineStream(line);
    std::string                cell;

    while(std::getline(lineStream,cell, ','))
    {
        result.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty())
    {
        // If there was a trailing comma then add an empty element.
        result.push_back("");
    }
    return result;
}

void printVector(const vector<string>& v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }

    cout << endl;
}

NeuralNetwork trainNN()
{
    ifstream file;

    file.open("breakout.csv");

    vector<string> line = getNextLineAndSplitIntoTokens(file);

    vector<int> nodes;
    
    nodes.push_back(4);
    nodes.push_back(4);
    nodes.push_back(1);

    NeuralNetwork nn(nodes);

    nn.setLearningRate(0.1);

    vector<Mat> inputs;
    vector<Mat> expectedOutputs;

    for (int i = 0; i < 7500; i++)
    {
        line.clear();
        line = getNextLineAndSplitIntoTokens(file);

        Mat inputs1(1, 4);
        Mat expectedOutputs1(1, 1);

        inputs1.set(0, 0, atof(line[0].c_str()));
        inputs1.set(0, 1, atof(line[1].c_str()));
        inputs1.set(0, 2, atof(line[2].c_str()));
        inputs1.set(0, 3, atof(line[3].c_str()));
        expectedOutputs1.set(0, 0, atof(line[4].c_str()));

        inputs.push_back(inputs1);
        expectedOutputs.push_back(expectedOutputs1);
    }

    cout << "Training!" << endl;

    nn.train(inputs, expectedOutputs, 100);

    file.close();

    return nn;
}

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

        //if (toCSV) write(PLAYER_A_FIRE);
    }

    if(keystate[SDLK_LEFT])
    {
        reward += alei.act(PLAYER_A_LEFT);

        //if (toCSV) write(PLAYER_A_LEFT);
    }
    if(keystate[SDLK_RIGHT])
    {
        reward += alei.act(PLAYER_A_RIGHT);

        //if (toCSV) write(PLAYER_A_RIGHT);
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
/// Do Next Agent Step
///////////////////////////////////////////////////////////////////////////////
float agentStep()
{
    int wide = alei.getRAM().get(108);
    float reward = 0;

    if (alei.lives() != lastLives)
    {
        --lastLives;
        alei.act(PLAYER_A_FIRE);
    }

    // Apply rules.
    int playerX = getPlayerX();
    int ballX = getBallX();
    double direction = 0.5;
    
    if (BallX_LastTick < ballX) {
        direction = 0.0;
        ballX += ((rand() % 2) + 2);
    }
    if (BallX_LastTick > ballX) {
        direction = 1.0;
        ballX -= ((rand() % 2) + 2);
    }

    write((double)(ballX) / 201.0);
    write(",");
    write(direction);
    write(",");
    write((double)playerX / 188.0);
    write(",");
    write((double)wide / 8.0);
    write(",");

    BallX_LastTick = getBallX();

    if (ballX < playerX + wide)
    {
        reward += alei.act(PLAYER_A_LEFT);

        write(1.0);
    }
    else if ((ballX > playerX + wide) && (playerX + wide < 188))
    {
        reward += alei.act(PLAYER_A_RIGHT);
        
        write(0.0);
    }
    else
    {
        write(0.5);
    }


    write("\n");
   
   return (reward + alei.act(PLAYER_A_NOOP));
}

float NN(NeuralNetwork &nn)
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

    Mat values(1, 4);

    values.set(0, 0, ballX);
    values.set(0, 1, direction);
    values.set(0, 2, playerX);
    values.set(0, 3, wide);

    Mat m = nn.forwardPropagation(values);

    if (m.get(0, 0) < 0.45)
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

    /** 
    * argv[1] : rom
    * argv[2] : media? true/>false<
    * argv[3] : print_ram? true/>false<
    * argv[4] : to csv? true/>false<
    **/
    const bool display_media(argc >= 3 ? atoi(argv[2])==1 : false);
    const bool printRam(argc >= 4 ? atoi(argv[3])==1 : false);
    toCSV = argc == 5 ? atoi(argv[4])==1 : false;

    if (toCSV)
    {
        csv.open("breakout.csv");

        write("ballX,ballDirection,playerX,playerWidth,userInput\n");
    }

    NeuralNetwork asd = trainNN();

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
        //totalReward += manualInput ? manualMode() : agentStep();
        totalReward += NN(asd);
   }

   std::cout << "Steps: " << step << std::endl;
   std::cout << "Reward: " << totalReward << std::endl;

    csv.close();

   return 0;
}
