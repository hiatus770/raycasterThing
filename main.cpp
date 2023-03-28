#include <iostream>
#include <cmath> 
#include <raylib.h>
#define WIDTHGAME 20
#define HEIGHTGAME 20
#define rayStep 0.01
#define toDegree(x) (x*180.0/3.14159)
#define toRadian(x) (x*3.14159/180.0)
#define brightMax 0.1
#define brightMin -0.9
#define gameFPS 220
#define getDecimal(x) (x - (int)x)
#define sgn(x) (x < 0 ? -1 : 1)
#define test 0 // if true then the program will do 2d raycasting instead of 3d
#define FOV toRadian(60)
#define testCode if(test)

#include "maps.h" // this is the file that contains the map and other textures 

using namespace std;

const int screenWidth = 1600;
const int screenHeight = 800;

// The player's position and angle
double playerX = 3.0;
double playerY = 3.0;
double playerA = toRadian(90); 
double fov = FOV; // This is how many degrees of freedom the view has 

/*TODO LIST 
- implement a texture mapping thing
- implement a better brightness for the walls
- fix the fisheye lens by spreading out where the rays are shot
*/

// Calculates the brightess of a wall given the distance from the player to the wall 
double brightness(double rayDist, double multiplier) {
    if (brightMin + (multiplier)*1/rayDist > brightMax){
        return brightMax;
    } else {
        return brightMin + (multiplier)*1/rayDist;
    }
}

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

int whatX(double x, double y, double xheading, double yheading){
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
        return blockX; 
    } else if (abs((int)x - x) < 0.0000001 && xheading < 0){
        blockX = (int)(x)-1; 
        for(int i = 0; i < HEIGHTGAME; i++){
            if (y >= i && y < i+1){
                blockY = i; 
                break; 
            }
        }
        return blockX; 
    } else if (abs((int)y - y) < 0.0000001 && yheading < 0) {
        blockY = (int)(y)-1; 
        for(int i = 0; i < WIDTHGAME; i++){
            if (x >= i && x < i+1){
                blockX = i; 
                break; 
            }
        }  
        return blockX; 
    } else {
        // y is an intenger
        blockY = (int)(y); 
        for(int i = 0; i < WIDTHGAME; i++){
            if (x >= i && x < i+1){
                blockX = i; 
                break; 
            }
        }  
        return blockX; 
    }
}

int whatY(double x, double y, double xheading, double yheading){
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
        return blockY; 
    } else if (abs((int)x - x) < 0.0000001 && xheading < 0){
        blockX = (int)(x)-1; 
        for(int i = 0; i < HEIGHTGAME; i++){
            if (y >= i && y < i+1){
                blockY = i; 
                break; 
            }
        }
        return blockY; 
    } else if (abs((int)y - y) < 0.0000001 && yheading < 0) {
        blockY = (int)(y)-1; 
        for(int i = 0; i < WIDTHGAME; i++){
            if (x >= i && x < i+1){
                blockX = i; 
                break; 
            }
        }  
        return blockY; 
    } else {
        // y is an intenger
        blockY = (int)(y); 
        for(int i = 0; i < WIDTHGAME; i++){
            if (x >= i && x < i+1){
                blockX = i; 
                break; 
            }
        }  
        return blockY; 
    }
}

/**
 * @brief Calculates what block the ray is intersecting given some information
 * 
 * @param x    The x coordinate of the ray
 * @param y     The y coordinate of the ray
 * @param xheading  The DX of the ray
 * @param yheading  The DY of the ray
 * @return Color  This is the value of the color of the block that the ray is in
 */
Color whatBlock(double x, double y, double xheading, double yheading){
    // given some x and y coordinate return the block that the coordinate is in
    // only subtract to the non integer number
    // calculate the x and y coordinate of the block that the ray is in
    int blockX = 0; 
    int blockY = 0; 
    // check if out of bounds
    if (x < 0 || x >= WIDTHGAME || y < 0 || y >= HEIGHTGAME){
        return BLACK; 
    }
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

/**
 * @brief Calculates what block the ray is intersecting given some information
 * 
 * @param x The x coordinate of the ray
 * @param y  The y coordinate of the ray
 * @param xheading The DX of the ray 
 * @param yheading The DY of the ray 
 * @return int This is the value of the color of the block that the ray is in
 */
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

/** 
 * @brief This function is used to update the player's position and angle
 * @param playerX The x coordinate of the player
 * @param playerY The y coordinate of the player
 * @param playerA The angle of the player
*/
void playerMovementUpdate(double* playerX, double* playerY, double* playerA){
    if (*playerA > 2*PI){
        *playerA -= 2*PI; 
    } else if (*playerA < 0){
        *playerA += 2*PI; 
    }
    if (IsKeyDown(KEY_W)){
        *playerX += cos(*playerA) * 0.01;
        *playerY += sin(*playerA) * 0.01; 
    }
    if (IsKeyDown(KEY_S)){
        *playerX -= cos(*playerA) * 0.01;
        *playerY -= sin(*playerA) * 0.01; 
    }
    if (IsKeyDown(KEY_A)){
        // strafe
        *playerX -= cos(*playerA + PI/2) * 0.01;
        *playerY -= sin(*playerA + PI/2) * 0.01;
    }
    if (IsKeyDown(KEY_D)){
        *playerX += cos(*playerA + PI/2) * 0.01;
        *playerY += sin(*playerA + PI/2) * 0.01;
    }
    if (IsKeyDown(KEY_LEFT)){
        *playerA -= 0.01; 
    }
    if (IsKeyDown(KEY_RIGHT)){
        *playerA += 0.01; 
    }
    
}



// Main function 
int main () {
    InitWindow(screenWidth, screenHeight, "Raycastuh");
    SetTargetFPS(gameFPS);

    while (WindowShouldClose() == false){
        // Making sure the angle isnt greater than the threshold set
        playerMovementUpdate(&playerX, &playerY, &playerA);

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

        // Draw the map
        BeginDrawing();
        ClearBackground(WHITE);

        // Ray casting loop behins here 
        int rayCount = screenWidth;
        for (int i = 0; i < rayCount; i++){
            // Calculate the ray angle by removing half of the FOV from the player angle and then slowly adding the FOV to the ray angle
            double rayAngle = playerA - fov/2.0 + fov * (double)i / (double)rayCount;
            double dY = sin(rayAngle);
            double dX = cos(rayAngle);
            // make sure we dont divide by 0
            if (dX == 0){
                continue; 
            }
            if (dY == 0){
                continue; 
            }

            // Draw the dx lne 
            bool hitWall = false;
            double distance = 0.0;
            double rayX = playerX;
            double rayY = playerY;

            // Important variables 
            double sX = sqrt (1 + (dY*dY)/(dX*dX));
            double sY = sqrt (1 + (dX*dX)/(dY*dY));
            double rayLengthX = 0.0;
            double rayLengthY = 0.0;
            double xRay[2] = {0.0, 0.0};
            double yRay[2] = {0.0, 0.0};

            // Placing the first intersection between the x and y axises 
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

            // RAY CASTING LOOP 
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

            // Check which way is shorter 
            if (rayLengthX < rayLengthY){
                distance = rayLengthX;
                rayX = xRay[0];
                rayY = xRay[1];
            } else {
                distance = rayLengthY;
                rayX = yRay[0];
                rayY = yRay[1];
            }

            // Correct for the fish eye effect
            distance = (distance * cos(rayAngle - playerA));
            
            // 3d rendering
            int ceiling = ((screenHeight/2.0) - screenHeight/(distance));
            int floor = screenHeight - ceiling;
            
            // Draw the ray for testing 
            testCode DrawLine(playerX*screenWidth/WIDTHGAME, playerY*screenHeight/HEIGHTGAME, rayX*screenWidth/WIDTHGAME, rayY*screenHeight/HEIGHTGAME, whatBlock(rayX, rayY, dX, dY));

            // Drawing each column for each ray 
            DrawRectangle(i*screenWidth/rayCount, ceiling, screenWidth/rayCount, floor - ceiling, ColorBrightness( whatBlock(rayX, rayY, dX, dY), brightness(distance, 3)));
            DrawRectangle(i*screenWidth/rayCount, 0, screenWidth/rayCount, ceiling, BLACK);
            DrawRectangle(i*screenWidth/rayCount, floor, screenWidth/rayCount, screenHeight - floor, ColorBrightness( GRAY, brightness(distance, 1)));
       }

        // Display the FPS currently 
        DrawFPS(10, 10); 
        EndDrawing();
    }
    CloseWindow();
    return 0;
};