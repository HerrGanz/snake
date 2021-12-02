/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

int main()

{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib");

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 40.0f, 0.01f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 00.0f, 2.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    SetCameraMode(camera, CAMERA_ORBITAL);

    Vector3 cubePosition = { 0 };

    SetTargetFPS(60);         // Set our game to run at 60 frames-per-second
    
    int direction = 1;
    direction = 2;


    

   
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) cubePosition.x += 0.5f;
        if (IsKeyDown(KEY_LEFT)) cubePosition.x -= 0.5f;
        if (IsKeyDown(KEY_UP)) cubePosition.z -= 0.5f;
        if (IsKeyDown(KEY_DOWN)) cubePosition.z += 0.5f;
        if (IsKeyDown(KEY_W)) cubePosition.y += 0.5f;
        if (IsKeyDown(KEY_S)) cubePosition.y -= 0.5f;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                
                DrawCube(cubePosition, 1.0f, 1.0f, 1.0f, GREEN);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                DrawGrid(20, 2.0f);

            EndMode3D();

            DrawText("Snake", 610, 10, 20, DARKGRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

int function() {

    

    return 2;
}