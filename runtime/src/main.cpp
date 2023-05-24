#include <raylib.h>
#include <fstream>
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;

int main()
{
    std::ifstream f("test.json");
    if (!f.is_open())
    {
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
        return 1;
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Mist Runtime");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        for (const auto& entity : data["entities"])
        {
            std::string name = entity.get<std::string>();

            DrawText(name.c_str(), GetRandomValue(0, GetScreenWidth()), GetRandomValue(0, GetScreenHeight()), 50, WHITE);
        }

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
