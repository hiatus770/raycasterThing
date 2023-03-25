#include <iostream>
#include <cmath> 
#include <raylib.h>
#define WIDTHGAME 10
#define HEIGHTGAME 10
#define FOV 3.14159/2 //3.14159/2.0
#define rayStep 0.01
#define toDegree(x) (x*180.0/3.14159)
#define toRadian(x) (x*3.14159/180.0)
#define brightMax -0.1
#define brightMin -0.9
#define gameFPS 500
#define getDecimal(x) (x - (int)x)
#define sgn(x) (x < 0 ? -1 : 1)
#define test true // if true then the program will do 2d raycasting instead of 3d

using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;

/*TODO LIST 
- implement a better ray block finding algorithm that is O(1)
- implement a texture mapping thing
- implement a better brightness for the walls
- fix the fisheye lens by spreading out where the rays are shot*/




// Calculates the brightess of a wall given the distance from the player to the wall 
double brightness(double rayDist) {
    if (brightMin + 1/rayDist > brightMax){
        return brightMax;
    } else {
        return brightMin + 1/rayDist;
    }
}

const int map[WIDTHGAME][HEIGHTGAME] = { 
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,6,0,0,0,1},
    {1,0,0,0,0,5,0,0,0,1},
    {1,0,0,0,0,4,0,0,0,1},
    {1,0,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,2,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
}; 


int whatBlock(double x, double y){
    // given some x and y coordinate return the block that the coordinate is in
    double blockX = (x);
    double blockY = (y);
    for (int i = 0; i < WIDTHGAME; i++){
        for (int j = 0; j < HEIGHTGAME; j++){
            if (blockX >= i-1 && blockX < i+1 && blockY >= j-1 && blockY < j+1){
                return map[i][j];
            }
        }
    }
}

Color colorNum[11] = {
    BLACK, 
    GRAY, 
    RED, 
    ORANGE, 
    YELLOW, 
    GREEN, 
    BLUE, 
    VIOLET, 
    PURPLE,
    PINK, 
    BROWN
};

int main () {
    double playerX = 2.0;
    double playerY = 2.0;
    double playerA = 0; 
    double fov = FOV; // This is how many degrees of freedom the view has 

    cout << "Beginning program!\n"; 
    InitWindow(screenWidth, screenHeight, "Raylib Pseudo 3d shader");
    SetTargetFPS(gameFPS);

    while (WindowShouldClose() == false){
        // Update the player position
        if (playerA > 2*3.14159){
            playerA -= 2*3.14159;
        } else if (playerA < 0){
            playerA += 2*3.14159;
        }
        if (IsKeyDown(KEY_W)){
            playerX += cos(playerA) * 0.01;
            playerY += sin(playerA) * 0.01; 
        }
        if (IsKeyDown(KEY_S)){
            playerX -= cos(playerA) * 0.01; 
            playerY -= sin(playerA) * 0.01; 
        }
        if (IsKeyDown(KEY_A)){
            playerX -= cos(playerA + 3.14159/2.0) * 0.01;
            playerY -= sin(playerA + 3.14159/2.0) * 0.01;
        }
        if (IsKeyDown(KEY_D)){
            playerX += cos(playerA + 3.14159/2.0) * 0.01;
            playerY += sin(playerA + 3.14159/2.0) * 0.01;
        }

        if (IsKeyDown(KEY_LEFT)){
            playerA -= 0.05; 
        }
        if (IsKeyDown(KEY_RIGHT)){
            playerA += 0.05; 
        }

        // FOV changer 
        if (IsKeyDown(KEY_UP)){
            fov += 0.05; 
        }   
        if (IsKeyDown(KEY_DOWN)){
            fov -= 0.05; 
        }

        BeginDrawing();
        ClearBackground(WHITE);

        if (test){
                // display the grid
                for (int i = 0; i < WIDTHGAME; i++){
                    for (int j = 0; j < HEIGHTGAME; j++){
                        DrawRectangle(i*screenWidth/WIDTHGAME, j*screenHeight/HEIGHTGAME, screenWidth/WIDTHGAME, screenHeight/HEIGHTGAME, colorNum[map[i][j]]);
                    }
                }

                // draw gridlines
                for (int i = 0; i < WIDTHGAME; i++){
                    DrawLine(i*screenWidth/WIDTHGAME, 0, i*screenWidth/WIDTHGAME, screenHeight, WHITE);
                }
                for (int i = 0; i < HEIGHTGAME; i++){
                    DrawLine(0, i*screenHeight/HEIGHTGAME, screenWidth, i*screenHeight/HEIGHTGAME, WHITE);
                }

                // display player
                DrawCircle(playerX*screenWidth/WIDTHGAME, playerY*screenHeight/HEIGHTGAME, 5, RED);
        }
        // start a for loop for each column of pixels on the screen
        int rayCount = 500000; // its usually screenWidth but this is for testing purposes
        for (int i = 0; i < rayCount; i++){
            // Calculate the ray angle by removing half of the FOV from the player angle and then slowly adding the FOV to the ray angle
            double rayAngle = playerA - fov/2.0 + fov * (double)i / (double)rayCount;
            double dY = sin(rayAngle);
            double dX = cos(rayAngle);

            // Draw the dx line 
            // DrawLine(playerX*screenWidth/WIDTHGAME, playerY*screenHeight/HEIGHTGAME, (playerX + dX)*screenWidth/WIDTHGAME, (playerY + dY)*screenHeight/HEIGHTGAME, WHITE);

            bool hitWall = false;
            double distance = 0.0;
            double rayX = playerX;
            double rayY = playerY;
            double xPercent = 0.0;
            double yPercent = 0.0;
            double sX = sqrt (1 + (dY*dY)/(dX*dX));
            double sY = sqrt (1 + (dX*dX)/(dY*dY));
            double rayLengthX = 0.0;
            double rayLengthY = 0.0;
            double xInt = 0.0;
            double yInt = 0.0;

            if (dX > 0){
                xPercent = 1.0 - (playerX - (int)playerX);
            }  else {
                xPercent = playerX - (int)playerX;
            }
            if (dY > 0){
                yPercent = 1.0 - (playerY - (int)playerY);
            } else {
                yPercent = playerY - (int)playerY;
            }

            // increment rayX in the right direciton to an integer location

            rayLengthX = sX * xPercent;
            rayLengthY = sY * yPercent;

            if (rayLengthX < rayLengthY){
                rayX += dX > 0 ? 1*xPercent : -1*xPercent;
                xInt = rayX;
                yInt = rayY + (rayX - playerX) * dY/dX;
            } else {
                rayY += dY > 0 ? 1*yPercent : -1*yPercent;
                xInt =  rayX + (rayY - playerY) * dX/dY;
                yInt = rayY;
            }

            
            // draw ray
            // DrawLine(playerX*screenWidth/WIDTHGAME, playerY*screenHeight/HEIGHTGAME, xInt*screenWidth/WIDTHGAME, yInt*screenHeight/HEIGHTGAME, WHITE);

            // // display the values 
            // DrawRectangle(0, 0, 200, 300, BLACK);
            // DrawText(TextFormat("xPercent: %f", xPercent), 10, 10, 20, WHITE);
            // DrawText(TextFormat("yPercent: %f", yPercent), 10, 30, 20, WHITE);
            // DrawText(TextFormat("dx: %f", dX), 10, 50, 20, WHITE);
            // DrawText(TextFormat("dy: %f", dY), 10, 70, 20, WHITE);
            // DrawText(TextFormat("rayLengthX: %f", rayLengthX), 10, 90, 20, WHITE);
            // DrawText(TextFormat("rayLengthY: %f", rayLengthY), 10, 110, 20, WHITE);

            

            // DrawCircle (xInt*screenWidth/WIDTHGAME, yInt*screenHeight/HEIGHTGAME, 5, BLUE);
            // display dx and dyt valueds and a rectangle behind it as a background
            

       }
        // Display the FPS currently 
        DrawFPS(10, 10);

        // Display the FOV 
        DrawText(TextFormat("FOV: %f", toDegree(fov)), 10, 30, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}