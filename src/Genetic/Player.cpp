
#include "Player.h"
#include "ale_interface.hpp"
#include "SDL.h"

const string Player::BREAKOUT_ROM = "../breakout/breakout.bin";
const string Player::BOXING_ROM = "../boxing/boxing.bin";
const string Player::DEMONATTACK_ROM = "../dattack/demon_attack.bin";
const string Player::STARGUNNER_ROM = "../strgunner/star_gunner.bin";

ALEInterface alei;

int Player::breakoutGetPlayerX()
{
   return alei.getRAM().get(72);// + ((rand() % 3) - 1);
}

int Player::breakoutGetBallX()
{
   return alei.getRAM().get(99);// + ((rand() % 3) - 1);
}

vector<int> Player::playBreakout(NeuralNetwork &nn, bool displayScreen)
{
    vector<int> res;
    int lastLives = 0;
    float totalReward = .0f;
    int maxSteps = 15000;

    alei.disableBufferedIO();

    // Init rand seed
    srand(time(NULL));

    // Create alei object.
    alei.setInt("random_seed", rand()%1000);
    alei.setFloat("repeat_action_probability", 0);
    alei.setBool("sound", false);
    alei.setBool("display_screen", displayScreen);
    alei.loadROM(Player::BREAKOUT_ROM);

    lastLives = alei.lives();

    int score = 0;
    int step = 0;
    int BallX_LastTick = Player::breakoutGetBallX();

    alei.act(PLAYER_A_FIRE);

    for (step = 0; !alei.game_over() && step < maxSteps; ++step) 
    {
        int wide = alei.getRAM().get(108);
        float reward = 0;

        if (alei.lives() != lastLives)
        {
            --lastLives;
            alei.act(PLAYER_A_FIRE);
        }

        int playerX = Player::breakoutGetPlayerX();
        int ballX = Player::breakoutGetBallX();
        double direction = 0.5;
        
        if (BallX_LastTick < ballX) {
            direction = 0.0;
        }
        if (BallX_LastTick > ballX) {
            direction = 1.0;
        }

        Mat inputs(1, 4);
        Mat outputs(1, 2);

        inputs.set(0, 0, ballX);
        inputs.set(0, 1, direction);
        inputs.set(0, 2, playerX);
        inputs.set(0, 3, wide);

        outputs = nn.forwardPropagation(inputs);

        if (outputs.get(0, 0) > 0.5)
        {
            reward += alei.act(PLAYER_A_RIGHT);
        }
        else if (outputs.get(0, 1) > 0.5)
        {
            reward += alei.act(PLAYER_A_LEFT);
        }

        BallX_LastTick = Player::breakoutGetBallX();

        reward = (reward + alei.act(PLAYER_A_NOOP));
        totalReward += reward;
    }

    score = (int)totalReward;

    res.push_back(score);
    res.push_back(step);

    return res;
}

vector<int> Player::playBoxing(NeuralNetwork &nn, bool displayScreen)
{
    vector<int> res;

    return res;
}

// DEMON ATTACK

enum BlockingHit {
    EMoveRight,
    EMoveLeft,
    ENotBlocking
};

// non modificable
const int LINE_WIDTH(160);          // Amount of pixels in a line
const int SHIP_WIDTH(7);            // Ship width in pixels
const int LEFT_THRESOLD(21);        // MIN X non-pixel coordinate the ship can move 25
const int RIGHT_THRESOLD(138);      // MAX X non-pixel coordinate the ship can move 135
const int P1_BULLETS_COLOR(174);
const int P1_SHIP_COLOR(115);
const int EN_BULLETS_COLOR(176);


// modifiable params
const int VISION_THRESOLD(10);
const int SECOND_VISION_LINE_THRESOLD(168);

int getChar_X(int char_id)
{
    // player position hex nibbles are inverted so we turn it around, same with the first nibble which seems to be desync with the second nibble
    int const val(alei.getRAM().get(char_id));
    int const rawFirstNibble((7-((val & 0xF0)>>4)) & 0x0F);
    int const rawSecondNibble(val & 0x0F);
    return (rawSecondNibble*16) + rawFirstNibble; 
}

int getP1_X(const vector<unsigned char> &grayscale, bool pixel=false)
{
    if(pixel) {
        // 185 is the ship position line we are tracking
        int const FILTER_LINE(LINE_WIDTH * 185);
        
        for(int i = FILTER_LINE; i < FILTER_LINE + LINE_WIDTH; ++i) {
            int a(grayscale[i]);
            if(a == P1_SHIP_COLOR){
                return (i - FILTER_LINE);
            }
        }
    }

    return getChar_X(16);
}

int firing_frames(0);
int last_firing_pos(0);
int getFiringEnemy_X() { 
    const int current_pos = getChar_X(15);
    if(last_firing_pos == current_pos) firing_frames++;
    else firing_frames = 0;
    last_firing_pos = current_pos;
    return current_pos; 
}

int mid_frames(0);
int last_mid_pos(0);
int getMidEnemy_X() { 
    const int current_pos = getChar_X(14);
    if(last_mid_pos == current_pos) mid_frames++;
    else mid_frames = 0;
    last_mid_pos = current_pos;
    return current_pos; 
}

int further_frames(0);
int last_further_pos(0);
int getFurtherEnemy_X() { 
    const int current_pos = getChar_X(13);
    if(last_further_pos == current_pos) further_frames++;
    else further_frames = 0;
    last_further_pos = current_pos;
    return current_pos;
}

int EnemyHandler() {
    int a = getFurtherEnemy_X();
    int b = getMidEnemy_X();
    int c = getFiringEnemy_X();

    if(firing_frames > 4) {
        if(mid_frames > 4) {
            return a;
        } else {
            return b;
        }
    } else {
        return c;
    }
}

void EnemyHandler3(int &a1, int &b1, int &c1) {
    int a = getFurtherEnemy_X();
    int b = getMidEnemy_X();
    int c = getFiringEnemy_X();

    a1 = a;
    b1 = b;
    c1 = c;

    if (further_frames > 4)
    {
        a1 = 0;
    }
    if (mid_frames > 4)
    {
        b1 = 0;
    }
    if (firing_frames > 4)
    {
        c1 = 0;
    }
}



bool ImpactValIsAnEnemy(int impact_pixel_val){
    return (impact_pixel_val != EN_BULLETS_COLOR && 
    impact_pixel_val != P1_SHIP_COLOR && 
    impact_pixel_val != P1_BULLETS_COLOR &&
    impact_pixel_val != 0);
}

BlockingHit isBlockingHit(int line, int &pixelToAvoid, vector<unsigned char> grayscale) {
    int const FILTER_LINE(LINE_WIDTH*line);
    int const VISION_X_AREA(SHIP_WIDTH + (VISION_THRESOLD*2));
    for(int i = 0; i < VISION_X_AREA; ++i) {
        const int impact_pixel_val(grayscale[FILTER_LINE + (getP1_X(grayscale, true)-VISION_THRESOLD) + i]);
        // Avoid flies
        if (line > SECOND_VISION_LINE_THRESOLD && ImpactValIsAnEnemy(impact_pixel_val)) {
            if(i < (VISION_X_AREA/2))
            {
                pixelToAvoid = i;

                return EMoveRight;
            }
            else if(i >= (VISION_X_AREA/2))
            {
                pixelToAvoid = i;

                return EMoveLeft;
            }
        }
        // Avoid pew pew's
        if(impact_pixel_val == EN_BULLETS_COLOR) {
            // Blocking hit, now we have to determine the direction we want to  based on the impact point
            if(i < (VISION_X_AREA/2))
            {
                pixelToAvoid = i;

                return EMoveRight;
            }
            else if(i >= (VISION_X_AREA/2))
            {
                pixelToAvoid = i;

                return EMoveLeft;
            }
        }
    }

    pixelToAvoid = 0;
    return ENotBlocking;
}

struct Point
{
    int x;
    int y;

    Point(int x, int y) :
        x(x),
        y(y)
    {

    }
};

Point agentStep(vector<unsigned char> &grayscale)
{
    // get screen information
    //alei.getScreenGrayscale(grayscale);
    
    // Reseting variables
    float reward = 0;    
    BlockingHit eBH = ENotBlocking;

    int pixelToAvoidCoordinateX = 0;
    int pixelToAvoidCoordinateY = 0;

    // Iterating from bottom line to top line which defines a vision rectangle (see is BlockingHit)
    for(int line = 185; line > 60; --line) {
        
        pixelToAvoidCoordinateY = line;

        eBH = isBlockingHit(line, pixelToAvoidCoordinateX, grayscale); 

        if (eBH != ENotBlocking)
        {
            break;
        }
    }

    Point res(pixelToAvoidCoordinateX, pixelToAvoidCoordinateY);

    return res;

    /*
    if(eBH != ENotBlocking) 
    {
        // Left thresold
        if(getP1_X() == LEFT_THRESOLD) {
            // If we cannot move more to the left we change direction
            ds.Direction = EMoveRight;
        }
        // Right thresold
        else if(getP1_X() == RIGHT_THRESOLD) {
            // If we cannot move more to the right we change direction
            ds.Direction = EMoveLeft;   
        }
        switch(ds.Direction){
            case EMoveLeft:
                reward+= alei.act(PLAYER_A_LEFT);
                ms.Direction = EMoveLeft;       
                break;
            case EMoveRight:
                reward+= alei.act(PLAYER_A_RIGHT);
                ms.Direction = EMoveRight;
                break;
            default:
                reward+= reward+=alei.act(PLAYER_A_FIRE);
                ms.Direction = ENotBlocking;                //action fire
        }
        
    } else {
        // Here we are safe, don't move but keep shooting
        reward+=alei.act(PLAYER_A_FIRE);
        ds.Dirty = false;
        const int mypos = getP1_X();
        const int en_pos = EnemyHandler();
        if(mypos < en_pos) {
            reward+= alei.act(PLAYER_A_RIGHT);
            ms.Direction = EMoveRight;
        } else if (mypos > en_pos) {
            reward+= alei.act(PLAYER_A_LEFT);
            ms.Direction = EMoveLeft;    
        } else {
            reward+=alei.act(PLAYER_A_FIRE);
            ms.Direction = ENotBlocking;
        }
    }   
    return (reward + alei.act(PLAYER_A_NOOP));
    */
}

vector<int> Player::playDemonAttack(NeuralNetwork &nn, bool displayScreen)
{
    vector<int> res;
    int lastLives = 0;
    float totalReward = .0f;
    int maxSteps = 7500;

    alei.disableBufferedIO();

    // Init rand seed
    srand(time(NULL));

    // Create alei object.
    alei.setInt("random_seed", rand()%1000);
    alei.setFloat("repeat_action_probability", 0);
    alei.setBool("sound", false);
    alei.setBool("display_screen", displayScreen);
    alei.loadROM(Player::DEMONATTACK_ROM);

    lastLives = alei.lives();

    int score = 0;
    int step = 0;


    /*
    double maxDangerousPointX = 0;
    double maxDangerousPointY = 0;
    double maxFurther = 0;
    double maxMid = 0;
    double maxFiring = 0;

    double minDangerousPointX = 100;
    double minDangerousPointY = 100;
    double minFurther = 100;
    double minMid = 100;
    double minFiring = 100;
    */


    for (step = 0; !alei.game_over() && step < maxSteps; ++step) 
    {   
        float reward = 0;


        vector<unsigned char> grayscale;

        alei.getScreenGrayscale(grayscale);

        Point danger = agentStep(grayscale);       // Dangerous point.
        int dangerousPointX = danger.x;
        int dangerousPointY = danger.y;
        //int relevantEnemy = EnemyHandler();         // X coordinate of relenvant enemy.
        int further = 0, mid = 0, firing = 0;
        int position = getChar_X(16);

        EnemyHandler3(further, mid, firing);

        double dangerousPointXd = dangerousPointX;
        double dangerousPointYd = dangerousPointY;
        double furtherd = further;
        double midd = mid;
        double firingd = firing;
        double positiond = position;

        dangerousPointXd /= 160.0;
        dangerousPointYd /= 190.0;
        //relevantEnemy /= 160;
        furtherd /= 160.0;
        midd /= 160.0;
        firingd /= 160.0;
        positiond /= 160.0;

        /*
        if (dangerousPointXd > maxDangerousPointX)
        {
            maxDangerousPointX = dangerousPointXd;
        }
        if (dangerousPointXd < minDangerousPointX)
        {
            minDangerousPointX = dangerousPointXd;
        }
        if (dangerousPointYd > maxDangerousPointY)
        {
            maxDangerousPointY = dangerousPointYd;
        }
        if (dangerousPointYd < minDangerousPointY)
        {
            minDangerousPointY = dangerousPointYd;
        }

        if (furtherd > maxFurther)
        {
            maxFurther = furtherd;
        }
        if (furtherd < minFurther)
        {
            minFurther = furtherd;
        }
        if (midd > maxMid)
        {
            maxMid = midd;
        }
        if (midd < minMid)
        {
            minMid = midd;
        }
        if (firingd > maxFiring)
        {
            maxFiring = firingd;
        }
        if (firingd < minFiring)
        {
            minFiring = firingd;
        }
        */
        
        /*
        Mat inputs(1, 133);
        Mat outputs(1, 3);

        for (unsigned i = 0; i < 128; ++i) {
            inputs.set(0, i, (double)alei.getRAM().get(i) / 255);                                                            
        }

        inputs.set(0, 128, dangerousPointXd);
        inputs.set(0, 129, dangerousPointYd);
        inputs.set(0, 130, furtherd);
        inputs.set(0, 131, midd);
        inputs.set(0, 132, firingd);
        */

        Mat inputs(1, 6);
        Mat outputs(1, 3);

        inputs.set(0, 0, dangerousPointXd);
        inputs.set(0, 1, dangerousPointYd);
        inputs.set(0, 2, furtherd);
        inputs.set(0, 3, midd);
        inputs.set(0, 4, firingd);
        inputs.set(0, 5, positiond);

        outputs = nn.forwardPropagation(inputs);

        int prediction = 2;

        for (int i = 0; i < 2; i++)
        {
            if (outputs.get(0, i) > outputs.get(0, i + 1))
            {
                prediction = i;
            }
        }
        
        switch (prediction)
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
                cout << "debug" << endl;
                break;
        }

        reward = (reward + alei.act(PLAYER_A_NOOP));
        totalReward += reward;
    }

    /*
    cout << "Max:" << endl;
    cout << maxDangerousPointX << endl;
    cout << maxDangerousPointY << endl;
    cout << maxFurther << endl;
    cout << maxMid << endl;
    cout << maxFiring << endl;
    cout << "-----------" << endl;
    cout << "Min:" << endl;
    cout << minDangerousPointX << endl;
    cout << minDangerousPointY << endl;
    cout << minFurther << endl;
    cout << minMid << endl;
    cout << minFiring << endl;
    cout << endl;
    */

    score = (int)totalReward;

    res.push_back(score);
    res.push_back(step);

    return res;
}

vector<int> Player::playStarGunner(NeuralNetwork &nn, bool displayScreen)
{
    vector<int> res;

    return res;
}