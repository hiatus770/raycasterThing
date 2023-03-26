#include <iostream>
#include <cmath> 
#include <raylib.h>
#define WIDTHGAME 10
#define HEIGHTGAME 10
#define rayStep 0.01
#define toDegree(x) (x*180.0/3.14159)
#define toRadian(x) (x*3.14159/180.0)
#define brightMax -0.1
#define brightMin -0.9
#define gameFPS 200
#define getDecimal(x) (x - (int)x)
#define sgn(x) (x < 0 ? -1 : 1)
#define test 1 // if true then the program will do 2d raycasting instead of 3d
#define FOV toRadian(60)

using namespace std;

const int screenWidth = 1600;
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
    {1,3,1,2,1,2,1,3,1,1},
    {1,0,0,0,0,0,0,0,0,2},
    {3,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,2},
    {3,0,0,0,0,2,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,2},
    {3,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,2},
    {3,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,3,1,4,1,2}
}; 

// 0 = black, 1 = gray, 2 = red, 3 = orange, 4 = yellow, 5 = green, 6 = blue, 7 = violet, 8 = purple, 9 = pink, 10 = brown
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

Color whatBlock(double x, double y, double xheading, double yheading){
    // given some x and y coordinate return the block that the coordinate is in
    // only subtract to the non integer number
    // calculate the x and y coordinate of the block that the ray is in
    int blockX = 0; 
    int blockY = 0; 
    if (abs((int)x - x) < 0.0000001 && xheading > 0){
        // x is an integer 
        blockX = (int)x; 
        for(int i = 0; i < HEIGHTGAME; i++){
            if (y >= i && y < i+1){
                blockY = i; 
                break; 
            }
        }
        return colorNum[map[blockY][blockX]]; 
    } else if (abs((int)x - x) < 0.0000001 && xheading < 0){
        blockX = (int)(x)-1; 
        for(int i = 0; i < HEIGHTGAME; i++){
            if (y >= i && y < i+1){
                blockY = i; 
                break; 
            }
        }
        return colorNum[map[blockY][blockX]]; 
    } else if (abs((int)y - y) < 0.0000001 && yheading < 0) {
        blockY = (int)(y)-1; 
        for(int i = 0; i < WIDTHGAME; i++){
            if (x >= i && x < i+1){
                blockX = i; 
                break; 
            }
        }  
        return colorNum[map[blockY][blockX]]; 
    } else {
        // y is an intenger
        blockY = (int)(y); 
        for(int i = 0; i < WIDTHGAME; i++){
            if (x >= i && x < i+1){
                blockX = i; 
                break; 
            }
        }  
        return colorNum[map[blockY][blockX]]; 
    }
}

int whatBlockNumber(double x, double y, double xheading, double yheading){
    // given some x and y coordinate return the block that the coordinate is in
    // only subtract to the non integer number
    // calculate the x and y coordinate of the block that the ray is in
    int blockX = 0; 
    int blockY = 0; 
    if (abs((int)x - x) < 0.0000001 && xheading > 0){
        // x is an integer 
        blockX = (int)x; 
        for(int i = 0; i < HEIGHTGAME; i++){
            if (y >= i && y < i+1){
                blockY = i; 
                break; 
            }
        }
        return map[blockY][blockX]; 
    } else if (abs((int)x - x) < 0.0000001 && xheading < 0){
        blockX = (int)(x)-1; 
        for(int i = 0; i < HEIGHTGAME; i++){
            if (y >= i && y < i+1){
                blockY = i; 
                break; 
            }
        }
        return map[blockY][blockX]; 
    } else if (abs((int)y - y) < 0.0000001 && yheading < 0) {
        blockY = (int)(y)-1; 
        for(int i = 0; i < WIDTHGAME; i++){
            if (x >= i && x < i+1){
                blockX = i; 
                break; 
            }
        }  
        return map[blockY][blockX]; 
    } else {
        // y is an intenger
        blockY = (int)(y); 
        for(int i = 0; i < WIDTHGAME; i++){
            if (x >= i && x < i+1){
                blockX = i; 
                break; 
            }
        }  
        return map[blockY][blockX]; 
    }
}



int main () {
    int count = 0; // counts the iterations of the raycasting loop
    double playerX = 3.0;
    double playerY = 3.0;
    double playerA = toRadian(90); 
    double fov = FOV; // This is how many degrees of freedom the view has 
 
    InitWindow(screenWidth, screenHeight, "Raycastuh");
    SetTargetFPS(gameFPS);

    while (WindowShouldClose() == false){
        // Making sure the angle isnt greater than the threshold set
        cout << playerX << " " << playerY << endl; 
        if (playerA > 2*3.14159){
            playerA -= 2*3.14159;
        } else if (playerA < 0){
            playerA += 2*3.14159;
        }
        // Player movement 
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

        // fov changer
        if (IsKeyDown(KEY_UP)){
            fov += 0.1; 
        }
        if (IsKeyDown(KEY_DOWN)){
            fov -= 0.1; 
        }

        // check if out of bounds 
        if (playerX < 0){
            playerX = 0; 
        } else if (playerX > WIDTHGAME){
            playerX = WIDTHGAME; 
        }
        if (playerY < 0){
            playerY = 0; 
        } else if (playerY > HEIGHTGAME){
            playerY = HEIGHTGAME; 
        }
        

        // Do not let player go inside walls by extending a ray in front, if the ray touches then you are inside a block
        // if the ray is inside a block then move the player out of the block
        if (whatBlockNumber(playerX + cos(playerA) * 0.1, playerY + sin(playerA) * 0.1, cos(playerA), sin(playerA)) != 0){
            playerX -= cos(playerA) * 0.01;
            playerY -= sin(playerA) * 0.01; 
        }
        // extend backwards
        if (whatBlockNumber(playerX - cos(playerA) * 0.1, playerY - sin(playerA) * 0.1, -cos(playerA), -sin(playerA)) != 0){
            playerX += cos(playerA) * 0.01;
            playerY += sin(playerA) * 0.01; 
        }
        // also shoot rays to the sides 
        if (whatBlockNumber(playerX + cos(playerA + 3.14159/2.0) * 0.1, playerY + sin(playerA + 3.14159/2.0) * 0.1, cos(playerA + 3.14159/2.0), sin(playerA + 3.14159/2.0)) != 0){
            playerX -= cos(playerA + 3.14159/2.0) * 0.01;
            playerY -= sin(playerA + 3.14159/2.0) * 0.01;
        }
        if (whatBlockNumber(playerX - cos(playerA + 3.14159/2.0) * 0.1, playerY - sin(playerA + 3.14159/2.0) * 0.1, -cos(playerA + 3.14159/2.0), -sin(playerA + 3.14159/2.0)) != 0){
            playerX += cos(playerA + 3.14159/2.0) * 0.01;
            playerY += sin(playerA + 3.14159/2.0) * 0.01;
        }



        BeginDrawing();
        ClearBackground(WHITE);

        // if (test){
        //         // display the grid
        //         for (int i = 0; i < WIDTHGAME; i++){
        //             for (int j = 0; j < HEIGHTGAME; j++){
        //                 DrawRectangle(i*screenWidth/WIDTHGAME, j*screenHeight/HEIGHTGAME, screenWidth/WIDTHGAME, screenHeight/HEIGHTGAME, colorNum[map[j][i]]);
        //             }
        //         }

        //         // draw gridlines
        //         for (int i = 0; i < WIDTHGAME; i++){
        //             DrawLine(i*screenWidth/WIDTHGAME, 0, i*screenWidth/WIDTHGAME, screenHeight, WHITE);
        //         }
        //         for (int i = 0; i < HEIGHTGAME; i++){
        //             DrawLine(0, i*screenHeight/HEIGHTGAME, screenWidth, i*screenHeight/HEIGHTGAME, WHITE);
        //         }

        //         // display player
        //         DrawCircle(playerX*screenWidth/WIDTHGAME, playerY*screenHeight/HEIGHTGAME, 5, RED);
        // }

        // start a for loop for each column of pixels on the screen
        int rayCount = screenWidth; // its usually screenWidth but this is for testing purposes
        for (int i = 0; i < rayCount; i++){
            // Calculate the ray angle by removing half of the FOV from the player angle and then slowly adding the FOV to the ray angle
            count++;
            double rayAngle = playerA - fov/2.0 + fov * (double)i / (double)rayCount;
            double dY = sin(rayAngle);
            double dX = cos(rayAngle);

            // Draw the dx lne 
            bool hitWall = false;
            double distance = 0.0;
            double rayX = playerX;
            double rayY = playerY;

            // make sure we dont divide by 0
            if (dX == 0){
                continue; 
            }
            if (dY == 0){
                continue; 
            }

            double sX = sqrt (1 + (dY*dY)/(dX*dX));
            double sY = sqrt (1 + (dX*dX)/(dY*dY));
            double rayLengthX = 0.0;
            double rayLengthY = 0.0;
            double xRay[2] = {0.0, 0.0};
            double yRay[2] = {0.0, 0.0};

            if (dX < 0){
                rayLengthX = sX*abs(rayX - floor(rayX));
                // place the x on the next integer
                xRay[0] = floor(rayX); 
                xRay[1] = rayY - dY/dX * abs(rayX - floor(rayX));
            }
            if (dX > 0){
                rayLengthX = sX*abs(rayX - ceil(rayX));
                // place the x on the next integer
                xRay[0] = ceil(rayX); 
                xRay[1] = rayY + dY/dX * abs(rayX - ceil(rayX));
            }

            if (dY < 0){
                rayLengthY = sY*abs(rayY - floor(rayY));
                // place the y on the next integer
                yRay[0] = rayX - dX/dY * abs(rayY - floor(rayY));
                yRay[1] = floor(rayY);
            }
            if (dY > 0){
                rayLengthY = sY*abs(rayY - ceil(rayY));
                // place the y on the next integer
                yRay[0] = rayX + dX/dY * abs(rayY - ceil(rayY));
                yRay[1] = ceil(rayY);
            }

            // iterate the x and y together until we hit a wall
            while (!hitWall && dX != 0 && dY != 0){
                // check if the x ray is shorter
                if (rayLengthX < rayLengthY){
                    // check if the x ray is out of bounds
                    if (xRay[0] < 0 || xRay[0] > WIDTHGAME || xRay[1] < 0 || xRay[1] > HEIGHTGAME){
                        hitWall = true;
                        continue; 
                    }
                    // check if the x ray is hitting a wall
                    if (map[(int)(xRay[1])][(int)(xRay[0]-1)] > 0 && dX < 0){
                        hitWall = true;
                        distance = rayLengthX;
                        continue; 
                    } else if (map[(int)(xRay[1])][(int)(xRay[0])] > 0 && dX > 0){
                        hitWall = true;
                        distance = rayLengthX;
                        continue; 
                    }
                    // if not then iterate the x ray
                    xRay[0] += sgn(dX);
                    xRay[1] += dY/dX * sgn(dX); 
                    rayLengthX += sX;
                } else {
                    // check if the y ray is out of bounds
                    if (yRay[0] < 0 || yRay[0] > WIDTHGAME || yRay[1] < 0 || yRay[1] > HEIGHTGAME){
                        hitWall = true;
                        continue; 
                    }
                    // check if the y ray is hitting a wall
                    if (map[(int)yRay[1]-1][(int)yRay[0]] > 0 && dY < 0){
                        hitWall = true;
                        distance = rayLengthY;
                        continue; 
                    } else if (map[(int)yRay[1]][(int)yRay[0]] > 0 && dY > 0){
                        hitWall = true;
                        distance = rayLengthY;
                        continue; 
                    }
                    // if not then iterate the y ray
                    yRay[0] += dX/dY * sgn(dY);
                    yRay[1] += sgn(dY);
                    rayLengthY += sY;
                }
            }



            if (rayLengthX < rayLengthY){
                distance = rayLengthX;
                rayX = xRay[0];
                rayY = xRay[1];
            } else {
                distance = rayLengthY;
                rayX = yRay[0];
                rayY = yRay[1];
            }

            distance = (distance * cos(rayAngle - playerA));
            // 3d rendering
            int ceiling = ((screenHeight/2.0) - screenHeight/(distance));
            int floor = screenHeight - ceiling;
            
            // DrawLine(playerX*screenWidth/WIDTHGAME, playerY*screenHeight/HEIGHTGAME, rayX*screenWidth/WIDTHGAME, rayY*screenHeight/HEIGHTGAME, whatBlock(rayX, rayY, dX, dY));

            if (rayLengthX > rayLengthY){
                 DrawRectangle(i*screenWidth/rayCount, ceiling, screenWidth/rayCount, floor - ceiling, whatBlock(rayX, rayY, dX, dY));
            } else {
                 DrawRectangle(i*screenWidth/rayCount, ceiling, screenWidth/rayCount, floor - ceiling, ColorBrightness( whatBlock(rayX, rayY, dX, dY),-0.4));
            }

            DrawRectangle(i*screenWidth/rayCount, 0, screenWidth/rayCount, ceiling, BLACK);
            DrawRectangle(i*screenWidth/rayCount, floor, screenWidth/rayCount, screenHeight - floor, BROWN);
            // Draw the ray
        

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