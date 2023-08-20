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



// Main function
int main()
{

    InitWindow(screenWidth, screenHeight, "Balluh");
    SetTargetFPS(gameFPS);

    float lastTime = 0;

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        colorCounter = globalBalls.size(); 

        if (IsMouseButtonDown(0) && globalBalls.size() < 200 && GetTime() - lastTime > 0.05)
        {
            globalBalls.push_back(Ball(100, 100, 0, 7, 10));
        }

        for (int i = 0; i < (int)globalBalls.size(); i++)
        {
            Ball *b = &globalBalls.at(i);
            b->draw();
        }

        if (GetTime() - lastTime > 0.05)
        {
            lastTime = GetTime();
            for (int i = 0; i < globalBalls.size(); i++)
            {
                Ball *b = &globalBalls.at(i);
                b->update();
            }

        }

        // Display the FPS currently
        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
};