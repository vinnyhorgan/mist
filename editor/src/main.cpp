#include "Logger.h"

#include <raylib.h>
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <fstream>
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;

int main()
{
    const int windowWidth = 1280;
    const int windowHeight = 720;

    SetTraceLogCallback(Logger::rayLog);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, "Mist Editor - v0.0.1");
    SetWindowMinSize(windowWidth, windowHeight);
    SetTargetFPS(60);

    RenderTexture2D viewport = LoadRenderTexture(200, 200);
    SetTextureFilter(viewport.texture, TEXTURE_FILTER_POINT);

    Logger::setDebug(true);

    bool sceneWindowActive = true;
    bool newEntityButtonPressed = false;

    while (!WindowShouldClose())
    {
        BeginTextureMode(viewport);

            ClearBackground(SKYBLUE);

        EndTextureMode();

        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            if (sceneWindowActive)
            {
                sceneWindowActive = !GuiWindowBox((Rectangle){ 0, 0, 192, GetScreenHeight() }, "SCENE");
                newEntityButtonPressed = GuiButton((Rectangle){ 24, 48, 144, 24 }, "NEW ENTITY");
                GuiLine((Rectangle){ 24, 88, 144, 16 }, NULL);
                GuiLabel((Rectangle){ 24, 120, 144, 24 }, "ENTITIES");
            }

            DrawTexture(viewport.texture, 200, 200, WHITE);

        EndDrawing();
    }

    UnloadRenderTexture(viewport);

    CloseWindow();

    return 0;
}
