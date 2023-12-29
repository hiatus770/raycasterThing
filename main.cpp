#include <iostream>
#include <cmath>
#include <raylib.h>
#include <vector>
#include <array>
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

const int screenWidth = 400;
const int screenHeight = 400;
const int maxWidth = screenWidth;
const int maxHeight = screenHeight;
const int minWidth = 0;
const int minHeight = 0;

double timeShift = 1; 
double globalMult = 0.2; 

class waveSource  {
    public: 
        double amplitude = 1; 
        double sinMult; 
        double posX; 
        double posY; 
        waveSource(int xPos, int yPos, double amplitude = 1){
            posX = xPos; 
            posY = yPos;
            amplitude = amplitude; 
        }

        double getTimeMagnitude(int x, int y){ 
            return sqrt(pow((x-posX), 2) + pow((y-posY), 2)); 
        }

        double getMagnitude(int x, int y){
            return sin((getTimeMagnitude(x, y) - timeShift)*globalMult) * amplitude; 
        }

        void setMult(double newMult){
            sinMult = newMult; 
        }
}; 

// Draw point 
void drawPoint(int x, int y, vector<waveSource> sources){
    double sumAtPoint = 0; 
    for (auto source : sources){
        double sourceMag = source.getMagnitude(x, y); 
        source.setMult(globalMult);  
        sumAtPoint += sourceMag; 
        // Do individual coloring per wave source 
        if (sourceMag > 0.99){
            DrawPixel(x, y, RED); 
        }
    }
    double maxValue = 2; 
    double allowedError = 0.01; 
    // Draw peaks only 
    if (abs(sumAtPoint) > maxValue - allowedError){
        DrawPixel(x,y, WHITE); 
    } 
}

// Main function
int main()
{

    InitWindow(screenWidth, screenHeight, "Balluh");
    SetTargetFPS(gameFPS);

    vector<waveSource> mainSources; 
    mainSources.push_back(waveSource(10, 175)); 
    mainSources.push_back(waveSource(10, 225)); 

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyDown(KEY_A)){
            timeShift += 1; 
        }
        if (IsKeyDown(KEY_UP)){
            globalMult += 0.1; 
        } else if (IsKeyDown(KEY_DOWN)){
            globalMult -= 0.5; 
        }

        for (int x = 0; x < screenWidth; x++){
            for (int y = 0; y < screenHeight; y++){
                drawPoint(x, y, mainSources); 
            }
        }

        // Display the FPS currently
        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
};