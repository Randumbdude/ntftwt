#include "benchmarks.h"
#include <iostream>
#include <string>
#include "raylib.h"
#include "globals.h"

int32_t raylib_test() {
	std::cout << "Starting raylib test..." << std::endl;

    // title the window
    std::string window_title = std::string("ntftwt_v_") + VERSION;

    // Create a 800x450 window
    InitWindow(800, 450, window_title.c_str());

    // Limit the game loop to 60 FPS
    SetTargetFPS(60);

    // Main loop
    while (!WindowShouldClose())
    {
        // Start drawing
        BeginDrawing();

        // Clear the screen
        ClearBackground(RAYWHITE);

        // Draw text
        DrawText("Hello, raylib!", 300, 100, 30, DARKGRAY);

        // Draw a rectangle
        DrawRectangle(300, 180, 200, 100, BLUE);

        // Draw a circle
        DrawCircle(400, 350, 50, RED);

        // Finish drawing
        EndDrawing();
    }

    // Close the window and clean up
    CloseWindow();

	std::cout << "Raylib test completed." << std::endl;

	return 0;
}
