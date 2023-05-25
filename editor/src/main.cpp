#pragma warning(disable : 4018)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4065)

#include "Logger.h"

#include <raylib.h>
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <fstream>
#include <json.hpp>
#include <iostream>
#include <string>
#include <vector>

using json = nlohmann::json;

class Component
{
public:
    std::string name;

    Component(std::string name)
    {
        this->name = name;
    }
};

class TransformComponent : public Component
{
public:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    TransformComponent() : Component("Transform")
    {
        position = Vector3{ 0, 0, 0 };
        rotation = Vector3{ 0, 0, 0 };
        scale = Vector3{ 1, 1, 1 };
    }

    TransformComponent(Vector3 position, Vector3 rotation, Vector3 scale) : Component("Transform")
    {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
    }
};

class Entity
{
public:
    std::string name;
    std::vector<Component> components;

    Entity(std::string name)
    {
        this->name = name;

        TransformComponent transform = TransformComponent();
        addComponent(transform);

        Logger::info("Created entity: " + name);
    }

    void addComponent(Component component)
    {
        components.push_back(component);
    }

    void removeComponent(Component component)
    {
        for (int i = 0; i < components.size(); i++)
        {
            if (components[i].name == component.name)
            {
                components.erase(components.begin() + i);
            }
        }
    }
};

RenderTexture2D viewport;
bool darkTheme = true;

std::vector<Entity> entities;

void createViewport()
{
    viewport = LoadRenderTexture(GetScreenWidth() - 240 - 240, GetScreenHeight() - 48 - 24 - 272);
    SetTextureFilter(viewport.texture, TEXTURE_FILTER_POINT);
}

void saveScene()
{
    Logger::info("Saving scene...");

    json scene;

    for (int i = 0; i < entities.size(); i++)
    {
        json entity;

        entity["name"] = entities[i].name;

        for (int j = 0; j < entities[i].components.size(); j++)
        {
            json component;

            component["name"] = entities[i].components[j].name;

            if (entities[i].components[j].name == "Transform")
            {
                TransformComponent transform = static_cast<TransformComponent&>(entities[i].components[j]);

                json position;
                position["x"] = transform.position.x;
                position["y"] = transform.position.y;
                position["z"] = transform.position.z;

                json rotation;
                rotation["x"] = transform.rotation.x;
                rotation["y"] = transform.rotation.y;
                rotation["z"] = transform.rotation.z;

                json scale;
                scale["x"] = transform.scale.x;
                scale["y"] = transform.scale.y;
                scale["z"] = transform.scale.z;

                component["position"] = position;
                component["rotation"] = rotation;
                component["scale"] = scale;
            }

            entity["components"].push_back(component);
        }

        scene["entities"].push_back(entity);
    }

    std::ofstream file("scene.json");
    file << scene;
    file.close();

    Logger::info("Scene saved!");
}

int main()
{
    const int windowWidth = 1280;
    const int windowHeight = 720;

    SetTraceLogCallback(Logger::rayLog);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(windowWidth, windowHeight, "Mist Editor - v0.0.1");
    SetWindowMinSize(windowWidth, windowHeight);
    SetTargetFPS(60);

    createViewport();

    Logger::setDebug(true);

    GuiLoadStyle("dark.rgs");

    bool windowSceneActive = true;
    bool windowInspectorActive = true;
    bool windowProjectActive = true;
    bool windowConsoleActive = true;

    Entity testEntity = Entity("Test Entity");
    entities.push_back(testEntity);

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            createViewport();
        }

        BeginTextureMode(viewport);

            ClearBackground(Color{ 78, 76, 79, 255 });

        EndTextureMode();

        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            GuiPanel(Rectangle{ 0, 0, (float)GetScreenWidth(), 48}, NULL);
            GuiStatusBar(Rectangle{ 0, (float)GetScreenHeight() - 24, (float)GetScreenWidth(), 24}, "Mist Editor - v0.0.1");

            if (darkTheme)
            {
                if (GuiButton(Rectangle{ 5, 5, 38, 38 }, GuiIconText(ICON_EXPLOSION, "")))
                {
                    GuiLoadStyleDefault();

                    darkTheme = false;
                }
            }
            else
            {
                if (GuiButton(Rectangle{ 5, 5, 38, 38 }, GuiIconText(ICON_STAR, "")))
                {
                    GuiLoadStyle("dark.rgs");

                    darkTheme = true;
                }
            }

            if (GuiButton(Rectangle{ 48, 5, 38, 38 }, GuiIconText(ICON_FILE_SAVE, "")))
            {
                saveScene();
            }

            if (windowSceneActive)
            {
                windowSceneActive = !GuiWindowBox(Rectangle{ 0, 48, 240, ((float)GetScreenHeight() - 48 - 24) / 2 }, "SCENE");
            }

            if (windowInspectorActive)
            {
                windowInspectorActive = !GuiWindowBox(Rectangle{ (float)GetScreenWidth() - 240, 48, 240, (float)GetScreenHeight() - 48 - 24}, "INSPECTOR");
            }

            if (windowProjectActive)
            {
                windowProjectActive = !GuiWindowBox(Rectangle{ 0, 48 + ((float)GetScreenHeight() - 48 - 24) / 2, 240, ((float)GetScreenHeight() - 48 - 24) / 2 }, "PROJECT");
            }

            if (windowConsoleActive)
            {
                windowConsoleActive = !GuiWindowBox(Rectangle{ 240, (float)GetScreenHeight() - 272 - 24, (float)GetScreenWidth() - 240 - 240, 272}, "CONSOLE");
            }

            DrawTexture(viewport.texture, 240, 48, WHITE);

        EndDrawing();
    }

    UnloadRenderTexture(viewport);

    CloseWindow();

    return 0;
}
