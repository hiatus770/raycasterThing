#include <cstdlib>
#include <iostream>
#include <cmath>
#include <raylib.h>
#include <vector>
#include <array>
#include <thread> 
#define WIDTHGAME 30
#define HEIGHTGAME 30
#define toDegree(x) (x * 180.0 / 3.14159)
#define toRadian(x) (x * 3.14159 / 180.0)
#define gameFPS 100
#define getDecimal(x) (x - (int)x)
#define sgn(x) (x < 0 ? -1 : 1)

#define gravCoef 0.9
#define dragCoef 0.99
#define elasticCoef 0.8
#define collisionCoef 0.4
float iterAmount = 1;

using namespace std;

const int screenWidth = 1920/2; 
const int screenHeight = 1080/2; 
const int maxWidth = screenWidth;
const int maxHeight = screenHeight;
const int minWidth = 0;
const int minHeight = 0;

int colorCounter = 0; 

class Ball
{
public:
    float x, y, velX, velY, pastX, pastY, radius;
    Color col = RED;

    Ball(float xPos, float yPos, float xvelX, float yvelY, float r)
    {
        x = xPos;
        y = yPos;
        radius = r;
        velX = xvelX;
        velY = yvelY;
        pastX = x - velX;
        pastY = y - velY;
        col = ColorFromHSV(sin(colorCounter/10)*135/2 + 180, 1.0f, 1.0f); 
    }

    void bounds()
    {
        if (x > (maxWidth - radius))
        {
            x = maxWidth - radius;
            // velX += sgn(velX)*0.1;
            pastX = x + velX * elasticCoef;
        }
        else if (x < radius + minWidth)
        {
            x = radius + minWidth;
            // velX += sgn(velX)*0.1;
            pastX = x + velX * elasticCoef;
        }
        if (y > (maxHeight - radius))
        {
            y = maxHeight - radius;
            // velY += sgn(velY)*0.2;
            pastY = y + velY * elasticCoef;
        }
        else if (y < radius + minHeight)
        {
            y = minHeight + radius;
            // velY += sgn(velY)*0.2;
            pastY = y + velY * elasticCoef;
        }
    }

    void collision();

    void draw()
    {
        DrawCircle((int)x, (int)y, radius, col);
    }

    void update()
    {
        collision(); 
        bounds(); 
        float tempX = x;
        float tempY = y;
        velX = (x - pastX) * dragCoef;
        velY = (y - pastY) * dragCoef + gravCoef;
        for (int i = 0; i < iterAmount; i++)
        {
            x += velX * (1 / iterAmount);
            y += velY * (1 / iterAmount);
        }

        pastX = tempX;
        pastY = tempY;
    }
};

vector<Ball> globalBalls;

class metaBall{
public: 
    float x, y, scale, xVel, yVel; 
    metaBall(float X, float Y, float Scale, float xvel, float yvel){
        x = X; y = Y; scale = Scale; xVel = xvel; yVel = yvel; 
    }

    void updatePosition(float dt){
        x = x + dt * xVel; 
        y = y + dt * yVel; 
        if ( x > screenWidth - scale) {
            x = screenWidth - scale; 
            xVel *= -1; 
        } 
        if (x < 0 + scale){
            x = scale; 
            xVel *= -1; 
        }
        if ( y > screenHeight - scale) {
            y = screenHeight - scale; 
            yVel *= -1; 
        } 
        if (y < 0 + scale){
            y = scale; 
            yVel *= -1; 
        }
    }
};

void Ball::collision()
{

    for (int i = 0; i < globalBalls.size(); i++)
    {
        Ball *b = &globalBalls.at(i);
        if (b != this)
        {

            float length = sqrtf((b->x - x) * (b->x - x) + (b->y - y) * (b->y - y));
            if (b->radius + radius > length && length != 0)
            {
                float overlap = b->radius + radius - length;
                if (overlap > 0)
                {
                    float dx; float dy; float ang; 
                    if (x - b->x == 0){
                        dx = 0; 
                        dy = overlap/2;   
                    } else {
                        ang = atan((y - b->y) / (x - b->x));
                        dx = abs(cos(ang) * overlap / 2);
                        dy = abs(sin(ang) * overlap / 2);
                    }  
                    x += -1*sgn(b->x - x)*dx;
                    y += -1*sgn(b->y- y)*dy; 
                    b->x += sgn(b->x - x)*dx; 
                    b->y += sgn(b->y - y)*dy; 
                }
            }
        }
    }
}


vector<metaBall> metaBalls; 


float sums[screenWidth][screenHeight]; 

float length(float x1, float y1, float x2, float y2){
    return sqrt(pow((x1-x2), 2) + pow((y1-y2),2)); 
}

int convertXY(int x, int y){
    return x + y * screenWidth; 
}

int convertArrX(int pos){
    return pos % screenWidth;  
}
int convertArrY(int pos){
    return (pos - pos%screenWidth) / screenWidth; 
}

void calculatePowerThread(int start, int end){
    for (int i = start; i <= end; i++){
        int x = convertArrX(i); 
        int y = convertArrY(i); 

        float xPos = x - 0.5; 
        float yPos = y - 0.5; 
        for(int i = 0; i < metaBalls.size(); i++){
            sums[x][y] += metaBalls[i].scale/(length(metaBalls[i].x, metaBalls[i].y, x, y)); 
            // sums[x][y] += metaBalls[i].scale;  
        }
    }
}

void calculatePower(){
    for(int i = 0; i < screenWidth; i++){
        for(int j = 0; j < screenHeight; j++){
            sums[i][j] = 0; 
        }
    }
    // Now start the threads 
    // convert x and y into actual coords
    int numThreads = 8; 
    int totalSize = screenHeight * screenWidth;
    int countPerThread = totalSize / numThreads; 

    vector<thread> calculationThreads; 
    for (int i = 0; i < numThreads; i++){
        calculationThreads.push_back(thread(calculatePowerThread, i*countPerThread, (i+1)*countPerThread)); 
    } 

    for (int i = 0; i < numThreads; i++){
        calculationThreads[i].join(); 
    }

    // thread t1 = thread(calculatePowerThread, 0, 1036800);  
    // thread t2 = thread(calculatePowerThread, 1036800, 2073600); 
    // t1.join(); 
    // t2.join(); 

    // for(int xArr = 0; xArr < screenWidth; xArr++){
    //     for(int yArr = 0; yArr < screenHeight; yArr++){
    //         float x = xArr - 0.5; 
    //         float y = yArr - 0.5; 
    //         for(int i = 0; i < metaBalls.size(); i++){
    //             sums[xArr][yArr] += metaBalls[i].scale/(length(metaBalls[i].x, metaBalls[i].y, x, y)); 
    //         }
    //     }
    // }
}

// Main function
int main()
{

    InitWindow(screenWidth, screenHeight, "Balluh");
    SetTargetFPS(gameFPS);

    float lastTime = 0;

    metaBalls.push_back(metaBall(100, 100, 30, 10, 100)); 
    metaBalls.push_back(metaBall(100, 300, 30, -10, 10)); 
    
    while (WindowShouldClose() == false)
    {
        
        BeginDrawing();
        ClearBackground(BLACK);
        colorCounter = globalBalls.size(); 

        for(int i = 0; i < metaBalls.size(); i++){
            metaBall* b = &metaBalls.at(i);
            b->updatePosition(GetFrameTime());  
        }

        calculatePower(); 
        
        // Draw all rectangles
        for(int i = 0; i < screenWidth; i++){
            for(int j = 0; j < screenHeight; j++){
                if (sums[i][j] > 0.9 && sums[i][j] < 1.1){
                    // Color rectColor = ColorFromHSV((int)((0.5 * ((0, 0, i, j)/length(0, 0, 1920, 1080)) - 1) * 40 + 240)%360, 1, 1); 
                    DrawRectangle(i, j, 1, 1, BLUE);
                    std::cout << "TESTING!";  
                    
                    // Color rectColor ColorFromHSV()
                }
                // Color rectColor = ColorFromHSV(50 + (100) * pow(2.718281828, -pow(sums[i][j]-1, 2)), 1, pow(2.718281828, -(1000) * pow(sums[i][j]-1, 2))); 
                // Color rectColor = ColorFromHSV(240 + pow(1 + 5 * abs(sums[i][j]-1), 3), 1, pow(10, -(100) * pow(sums[i][j]-1, 2))); 
                // DrawRectangle(i, j, 1, 1, rectColor);

            }
        }

        if (IsMouseButtonDown(0) && GetTime() - lastTime > 0.05)
        {
            // globalBalls.push_back(Ball(100, 100, 0, 7, 10));
            metaBalls.push_back(metaBall(GetMouseX(), GetMouseY(), rand()%30+5, -50 + rand()%150, -50 + rand()%150)); 
        }

        // for (int i = 0; i < (int)globalBalls.size(); i++)
        // {
        //     Ball *b = &globalBalls.at(i);
        //     b->draw();
        // }

        // if (GetTime() - lastTime > 0.05)
        // {
        //     lastTime = GetTime();
        //     for (int i = 0; i < globalBalls.size(); i++)
        //     {
        //         Ball *b = &globalBalls.at(i);
        //         b->update();
        //     }

        // }

        // Display the FPS currently
        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
};
