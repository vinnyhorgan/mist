#include "Logger.h"

#include <raylib.h>
#include <fstream>
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;

int main()
{
    SetTraceLogCallback(Logger::rayLog);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Mist Editor");
    SetTargetFPS(60);

    Logger::setDebug(true);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
