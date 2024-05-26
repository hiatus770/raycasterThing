#include <cstdlib>
#include <iostream>
#include <cmath>
#include <raylib.h>
#include <vector>
#include <array>
#include <thread> 
#define toDegree(x) (x * 180.0 / 3.14159)
#define toRadian(x) (x * 3.14159 / 180.0)
#define gameFPS 100
#define getDecimal(x) (x - (int)x)
#define sgn(x) (x < 0 ? -1 : 1)
using namespace std;

const int screenWidth = 1920/2; 
const int screenHeight = 1080/2; 
const int maxWidth = screenWidth;
const int maxHeight = screenHeight;
const int minWidth = 0;
const int minHeight = 0;

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

        for(int i = 0; i < metaBalls.size(); i++){
            metaBall* b = &metaBalls.at(i);
            b->updatePosition(GetFrameTime());  
        }

        calculatePower(); 
        
        // Draw all rectangles
        for(int i = 0; i < screenWidth; i++){
            for(int j = 0; j < screenHeight; j++){
                if (sums[i][j] > 0.99 && sums[i][j] < 1.01){
                    DrawRectangle(i, j, 1, 1, BLUE);
                }
            }
        }

        if (IsMouseButtonDown(0) && GetTime() - lastTime > 0.05)
        {
            // globalBalls.push_back(Ball(100, 100, 0, 7, 10));
            metaBalls.push_back(metaBall(GetMouseX(), GetMouseY(), rand()%30+5, -50 + rand()%150, -50 + rand()%150)); 
        }

        // Display the FPS currently
        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
};
