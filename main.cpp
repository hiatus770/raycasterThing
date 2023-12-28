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

const int screenWidth = 800;
const int screenHeight = 800;
const int maxWidth = screenWidth;
const int maxHeight = screenHeight;
const int minWidth = 0;
const int minHeight = 0;


class solenoid  {
    public: 
        int polarity = 1; // Can be set to 0 
        int posX; 
        int posY; 
        solenoid(int xPos, int yPos, int pol = 1){
            posX = xPos; 
            posY = yPos;
            polarity = pol; 
        }

        void drawSolenoid(float cur){
            Color col; 
            cur *= polarity; 
            if (cur > 0){
                col = (Color){255*cur, 0, 0, 255}; 
            } else if (cur < 0){
                col = (Color){0, 0, 255 * -1 * cur, 255};
            }
            DrawCircle(posX, posY, 10, col);
        }


}; 

// Main function
int main()
{

    InitWindow(screenWidth, screenHeight, "Balluh");
    SetTargetFPS(gameFPS);


    // Setup the 6 main solenoids 
    solenoid A1(500, 400, 1); 
    solenoid A2(300, 400, 1); 
    solenoid B1(400 + 50, 400 - 86, 1); 
    solenoid B2(400 - 50, 400 + 86, 1);     
    solenoid C1(400 - 50, 400 - 86, 1);
    solenoid C2(400 + 50, 400 + 86, 1); 

    int time = 0; 

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyPressed(KEY_A)){
            time += 30; 
        }
        if (IsKeyDown(KEY_S)){
            time += 2; 
        }
        if (IsKeyPressed(KEY_R)){
            time = 0; 
        }


        A1.drawSolenoid(sin(toRadian(time))); 
        A2.drawSolenoid(sin(toRadian(time))); 
        B1.drawSolenoid(sin(toRadian((time + 120))));
        B2.drawSolenoid(sin(toRadian(time) + ((2 * PI)/(3)))); 
        C1.drawSolenoid(sin(toRadian((time + 240)))); 
        C2.drawSolenoid(sin(toRadian((time + 240)))); 

        // Display the FPS currently
        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
};