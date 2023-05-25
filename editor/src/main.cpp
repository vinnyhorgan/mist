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

    json serialize()
    {
        json transform;
        transform["name"] = name;
        json position;
        position["x"] = this->position.x;
        position["y"] = this->position.y;
        position["z"] = this->position.z;
        json rotation;
        rotation["x"] = this->rotation.x;
        rotation["y"] = this->rotation.y;
        rotation["z"] = this->rotation.z;
        json scale;
        scale["x"] = this->scale.x;
        scale["y"] = this->scale.y;
        scale["z"] = this->scale.z;
        transform["position"] = position;
        transform["rotation"] = rotation;
        transform["scale"] = scale;

        return transform;
    }

    void deserialize(json transform)
    {
        json position = transform["position"];
        json rotation = transform["rotation"];
        json scale = transform["scale"];
        this->position = Vector3{ position["x"], position["y"], position["z"] };
        this->rotation = Vector3{ rotation["x"], rotation["y"], rotation["z"] };
        this->scale = Vector3{ scale["x"], scale["y"], scale["z"] };
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

class Scene
{
public:
    std::vector<Entity> entities;

    Scene()
    {
        Logger::info("Created scene!");
    }

    void addEntity(Entity entity)
    {
        entities.push_back(entity);
    }

    void removeEntity(Entity entity)
    {
        for (int i = 0; i < entities.size(); i++)
        {
            if (entities[i].name == entity.name)
            {
                entities.erase(entities.begin() + i);
            }
        }
    }

    void save()
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

    void load()
    {
        Logger::info("Loading scene...");

        std::ifstream file("scene.json");
        json scene;
        file >> scene;
        file.close();

        for (int i = 0; i < scene["entities"].size(); i++)
        {
            Entity entity = Entity(scene["entities"][i]["name"]);

            for (int j = 0; j < scene["entities"][i]["components"].size(); j++)
            {
                if (scene["entities"])
                {
                    if (scene["entities"][i]["components"][j]["name"] == "Transform")
                    {
                        TransformComponent transform = TransformComponent();
                        transform.deserialize(scene["entities"][i]["components"][j]);
                        entity.addComponent(transform);
                    }
                }
            }
        }
    }
};

RenderTexture2D viewport;
bool darkTheme = true;
Scene currentScene;
Entity* selectedEntity = nullptr;

std::vector<std::string> consoleOutput;

void log(std::string message)
{
    consoleOutput.push_back(message);
}

void createViewport()
{
    viewport = LoadRenderTexture(GetScreenWidth() - 240 - 240, GetScreenHeight() - 48 - 24 - 272);
    SetTextureFilter(viewport.texture, TEXTURE_FILTER_POINT);
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

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            createViewport();
        }

        if (IsMouseButtonPressed(0))
        {
            log("Hello world!");
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
                currentScene.save();
            }

            if (windowSceneActive)
            {
                windowSceneActive = !GuiWindowBox(Rectangle{ 0, 48, 240, ((float)GetScreenHeight() - 48 - 24) / 2 }, "SCENE");

                if (GuiButton(Rectangle{ 5, 48 + 30, 230, 30 }, GuiIconText(ICON_FILE_SAVE, "Create new entity")))
                {
                    Entity entity = Entity("Entity " + std::to_string(currentScene.entities.size() + 1));
                    currentScene.addEntity(entity);

                    selectedEntity = &currentScene.entities[currentScene.entities.size() - 1]; // mysterious crash without this line
                }

                for (int i = 0; i < currentScene.entities.size(); i++)
                {
                    if (GuiButton(Rectangle{ 5, (float)(48 + 65 + 30 * i), 230, 30 }, currentScene.entities[i].name.c_str()))
                    {
                        selectedEntity = &currentScene.entities[i];
                    }
                }
            }

            if (windowInspectorActive)
            {
                windowInspectorActive = !GuiWindowBox(Rectangle{ (float)GetScreenWidth() - 240, 48, 240, (float)GetScreenHeight() - 48 - 24}, "INSPECTOR");

                if (selectedEntity != nullptr)
                {
                    GuiLabel(Rectangle{ (float)GetScreenWidth() - 240 + 5, 48 + 30, 230, 30 }, selectedEntity->name.c_str());

                    for (int i = 0; i < selectedEntity->components.size(); i++)
                    {
                        GuiLabel(Rectangle{ (float)GetScreenWidth() - 240 + 5, (float)(48 + 65 + 30 * i), 230, 30 }, selectedEntity->components[i].name.c_str());
                    }

                    if (GuiButton(Rectangle{ (float)GetScreenWidth() - 240 + 5, (float)(48 + 65 + 30 * selectedEntity->components.size()), 230, 30 }, GuiIconText(ICON_CROSS, "Delete entity")))
                    {
                        currentScene.removeEntity(*selectedEntity);
                        selectedEntity = nullptr;
                    }
                }
            }

            if (windowProjectActive)
            {
                windowProjectActive = !GuiWindowBox(Rectangle{ 0, 48 + ((float)GetScreenHeight() - 48 - 24) / 2, 240, ((float)GetScreenHeight() - 48 - 24) / 2 }, "PROJECT");

                static std::string currentPath = ".";

                FilePathList files = LoadDirectoryFiles(currentPath.c_str());

                if (GuiButton(Rectangle{ 5, 48 + 30 + ((float)GetScreenHeight() - 48 - 24) / 2, 230, 30 }, GuiIconText(ICON_ARROW_LEFT, "Back")))
                {
                    currentPath = ".";
                }

                for (int i = 0; i < files.count; i++)
                {
                    if (IsFileExtension(files.paths[i], ".json"))
                    {
                        if (GuiButton(Rectangle{ 5, (float)(48 + 60 + ((float)GetScreenHeight() - 48 - 24) / 2 + 30 * i), 230, 30 }, files.paths[i]))
                        {

                        }
                    }
                    else if (IsPathFile(files.paths[i]))
                    {
                        GuiLabel(Rectangle{ 5, (float)(48 + 60 + ((float)GetScreenHeight() - 48 - 24) / 2 + 30 * i), 230, 30 }, files.paths[i]);
                    }
                    else
                    {
                        if (GuiButton(Rectangle{ 5, (float)(48 + 60 + ((float)GetScreenHeight() - 48 - 24) / 2 + 30 * i), 230, 30 }, files.paths[i]))
                        {
                            currentPath = files.paths[i];
                        }
                    }
                }
            }

            if (windowConsoleActive)
            {
                windowConsoleActive = !GuiWindowBox(Rectangle{ 240, (float)GetScreenHeight() - 272 - 24, (float)GetScreenWidth() - 240 - 240, 272}, "CONSOLE");

                for (int i = 0; i < consoleOutput.size(); i++)
                {
                    GuiLabel(Rectangle{ 240 + 5, (float)(GetScreenHeight() - 272 - 24 + 30 + 30 * i), (float)GetScreenWidth() - 240 - 240 - 10, 30 }, consoleOutput[i].c_str());

                    GuiLine(Rectangle{ 250, (float)(GetScreenHeight() - 272 - 24 + 30 + 30 * i + 30), (float)GetScreenWidth() - 240 - 260, 1 }, "");
                }
            }

            DrawTexture(viewport.texture, 240, 48, WHITE);

        EndDrawing();
    }

    UnloadRenderTexture(viewport);

    CloseWindow();

    return 0;
}
