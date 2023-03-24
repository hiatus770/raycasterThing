#include <iostream>
#include <cmath> 
#include <raylib.h>
#define WIDTHGAME 10
#define HEIGHTGAME 10
#define FOV 3.14159/2.0
#define rayStep 0.01
#define toDegree(x) (x*180.0/3.14159)
#define toRadian(x) (x*3.14159/180.0)
#define brightMax -0.1
#define brightMin -0.9
#define gameFPS 200

using namespace std;

const int screenWidth = 1422;
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

        // cout << "(" << playerX << "," << playerY << ")" << endl; 

        BeginDrawing();
        ClearBackground(WHITE);
        // start a for loop for each column of pixels on the screen
        for (int i = 0; i < screenWidth; i++){
            // Calculate the ray angle by removing half of the FOV from the player angle and then slowly adding the FOV to the ray angle
            double rayAngle = (playerA - fov/2.0) + ((double)i/(double)screenWidth) * fov;
            bool hitWall = false;
            double rayDistance = 0; 
            double eyeX = cos(rayAngle);
            double eyeY = sin(rayAngle);

            // find out when the ray angle intersects
            int testX  = (int)(playerX + eyeX*rayDistance); 
            int testY = (int)(playerY + eyeY*rayDistance); 

            // Start the ray casting loop
            while (!hitWall){
                rayDistance += rayStep * sqrt(eyeX*eyeX + eyeY*eyeY); 
                // create a plane of the player that the rays get casted out of  
                testX  = (int)(playerX + eyeX*rayDistance); 
                testY = (int)(playerY + eyeY*rayDistance); 
                // We multiply by ry distance ot increase the distance that it has travelled
                if (testX < 0 || testX > WIDTHGAME || testY > HEIGHTGAME || testY < 0){
                    hitWall = true;
                    rayDistance = 20.0;
                } else {
                    if (map[testX][testY] > 0){
                        hitWall = true; 
                    }
                }
            }

            // calculate the distance to the ceiling and floor
            int ceiling = (screenHeight/2.0) - screenHeight/rayDistance*0.65; 
            int floor = screenHeight - ceiling;

            // draw the ceiling
            DrawRectangle(i, 0, 1, ceiling, BLACK);
            // draw the floor
            DrawRectangle(i, floor, 1, screenHeight - floor, BLACK);
            // draw the walls colored
            DrawRectangle(i , ceiling, 1, floor - ceiling, ColorBrightness(colorNum[map[(int)testX][(int)testY]], brightness(rayDistance))); 
            }

        // Display the FPS currently 
        DrawFPS(10, 10);

        // Display the FOV 
        DrawText(TextFormat("FOV: %f", toDegree(fov)), 10, 30, 20, WHITE);
        
        // DrawCircle(playerX*screenWidth/WIDTHGAME, playerY*screenHeight/HEIGHTGAME, 5, RED);
        // // draw the walls of the map
        // for (int i = 0; i < WIDTHGAME; i++){
        //     for (int j = 0; j < HEIGHTGAME; j++){
        //         if (map[i][j] > 0){
        //             DrawRectangle(i*screenWidth/WIDTHGAME, j*screenHeight/HEIGHTGAME, screenWidth/WIDTHGAME, screenHeight/HEIGHTGAME, BLACK);
        //         }
        //     }
        // }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}