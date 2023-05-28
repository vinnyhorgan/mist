#pragma warning(disable : 4018)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4065)

#include "Logger.h"
#include "Component.h"
#include "Entity.h"
#include "Scene.h"

#include <raylib.h>
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

RenderTexture2D viewport;
bool darkTheme = true;
Scene currentScene;
Entity* selectedEntity = nullptr;

std::vector<std::string> consoleOutput;

Rectangle panelContentRec;
Vector2 panelScroll;

void log(std::string message)
{
    consoleOutput.push_back(message);
    panelContentRec.height += 30;
    panelScroll.y -= 30;
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

    Rectangle panelRec = { 240, (float)GetScreenHeight() - 272 - 24 + 24, (float)GetScreenWidth() - 240 - 240, 272 };
    panelContentRec = { 0, 0, (float)GetScreenWidth() - 240 - 240, 0 };
    panelScroll = { 0, 0 };

    while (!WindowShouldClose())
    {
        panelRec = { 240, (float)GetScreenHeight() - 272 - 24 + 24, (float)GetScreenWidth() - 240 - 240, 272 };

        if (IsWindowResized())
        {
            createViewport();
        }

        if (IsMouseButtonPressed(0))
        {
            log(std::to_string(GetRandomValue(0, 100)));
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
                windowConsoleActive = !GuiWindowBox(Rectangle{ 240, (float)GetScreenHeight() - 272 - 24, (float)GetScreenWidth() - 240 - 240, 272 }, "CONSOLE");

                Rectangle panelView = GuiScrollPanel(panelRec, NULL, panelContentRec, &panelScroll);
                panelContentRec = { 0, 0, panelView.width, panelContentRec.height };

                BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);

                for (int i = 0; i < consoleOutput.size(); i++)
                {
                    GuiLabel(Rectangle{ panelRec.x + panelScroll.x, panelRec.y + panelScroll.y + 30 * i, panelView.width - 10, 30 }, consoleOutput[i].c_str());
                }

                EndScissorMode();
            }

            DrawTexture(viewport.texture, 240, 48, WHITE);

        EndDrawing();
    }

    UnloadRenderTexture(viewport);

    CloseWindow();

    return 0;
}
