#include "Logger.h"

#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <json.hpp>
#include <iostream>

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

using json = nlohmann::json;

int main()
{
    std::ifstream f("test.json");
    if (!f.is_open())
    {
        std::cout << "Could not open file" << std::endl;
        return 1;
    }

    json data = json::parse(f);

    f.close();

    if (data.find("entities") != data.end() && data["entities"].is_array())
    {
        for (const auto& entity : data["entities"])
        {
            std::cout << entity << std::endl;
        }
    }
    else
    {
        std::cout << "Could not find entities array" << std::endl;
        return 1;
    }

    const int windowWidth = 1280;
    const int windowHeight = 720;

    SetTraceLogCallback(Logger::rayLog);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, "Mist Runtime - v0.0.1");
    SetWindowMinSize(windowWidth, windowHeight);
    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(windowWidth, windowHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    Logger::setDebug(true);

    while (!WindowShouldClose())
    {
        float scale = MIN((float)GetScreenWidth() / windowWidth, (float)GetScreenHeight() / windowHeight);

        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (windowWidth * scale)) * 0.5f) / scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (windowHeight * scale)) * 0.5f) / scale;
        virtualMouse = Vector2Clamp(virtualMouse, Vector2{ 0, 0 }, Vector2{ (float)windowWidth, (float)windowHeight });

        BeginTextureMode(target);

            ClearBackground(SKYBLUE);

            for (const auto& entity : data["entities"])
            {
                std::string name = entity.get<std::string>();

                DrawText(name.c_str(), GetRandomValue(0, GetScreenWidth()), GetRandomValue(0, GetScreenHeight()), 50, WHITE);
            }

        EndTextureMode();

        BeginDrawing();

            ClearBackground(BLACK);

            DrawTexturePro(target.texture, Rectangle{ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           Rectangle{ (GetScreenWidth() - ((float)windowWidth * scale)) * 0.5f, (GetScreenHeight() - ((float)windowHeight * scale)) * 0.5f,
                           (float)windowWidth * scale, (float)windowHeight * scale }, Vector2{ 0, 0 }, 0.0f, WHITE);

        EndDrawing();
    }

    UnloadRenderTexture(target);

    CloseWindow();

    return 0;
}
