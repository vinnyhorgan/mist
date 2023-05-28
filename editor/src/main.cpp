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

Rectangle consolePanelContentRec;
Vector2 consolePanelScroll;

void log(std::string message)
{
    consoleOutput.push_back(message);
    consolePanelContentRec.height += 30;
    consolePanelScroll.y -= 30;
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

    Rectangle consolePanelRec = { 240, (float)GetScreenHeight() - 272 - 24 + 24, ((float)GetScreenWidth() - 240 - 240) / 2, 272 - 24 };
    consolePanelContentRec = { 0, 0, ((float)GetScreenWidth() - 240 - 240) / 2, 0 };
    consolePanelScroll = { 0, 0 };

    Rectangle projectPanelRec = { 240 + ((float)GetScreenWidth() - 240 - 240) / 2, (float)GetScreenHeight() - 272 - 24 + 24, ((float)GetScreenWidth() - 240 - 240) / 2, 272 - 24 };
    Rectangle projectPanelContentRec = { 0, 0, ((float)GetScreenWidth() - 240 - 240) / 2, 0 };
    Vector2 projectPanelScroll = { 0, 0 };

    while (!WindowShouldClose())
    {
        consolePanelRec = { 240, (float)GetScreenHeight() - 272 - 24 + 24, ((float)GetScreenWidth() - 240 - 240) / 2, 272 - 24 };
        consolePanelContentRec = { 0, 0, consolePanelRec.width - 14, consolePanelContentRec.height };
        projectPanelRec = { 240 + ((float)GetScreenWidth() - 240 - 240) / 2, (float)GetScreenHeight() - 272 - 24 + 24, ((float)GetScreenWidth() - 240 - 240) / 2, 272 - 24 };
        projectPanelContentRec = { 0, 0, projectPanelRec.width - 14, projectPanelContentRec.height };

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
                windowSceneActive = !GuiWindowBox(Rectangle{ 0, 48, 240, (float)GetScreenHeight() - 48 - 24 }, "SCENE");

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
                windowProjectActive = !GuiWindowBox(Rectangle{ 240 + ((float)GetScreenWidth() - 240 - 240) / 2, (float)GetScreenHeight() - 272 - 24, ((float)GetScreenWidth() - 240 - 240) / 2, 272 }, "PROJECT");

                Rectangle projectPanelView = GuiScrollPanel(projectPanelRec, NULL, projectPanelContentRec, &projectPanelScroll);

                static std::string currentPath = "../../../..";

                FilePathList files = LoadDirectoryFiles(currentPath.c_str());

                int columns = 2;

                if (IsWindowMaximized())
                {
                    columns = 4;
                }

                BeginScissorMode(projectPanelView.x, projectPanelView.y, projectPanelView.width, projectPanelView.height);

                for (int i = 0; i < files.count; i++)
                {
                    projectPanelContentRec.height = 35 * (i / columns) + 35;

                    std::string displayName = files.paths[i];

                    if (displayName.substr(0, 2) == "./")
                    {
                        displayName = displayName.substr(2, displayName.size() - 2);
                    }

                    if (displayName.size() > 16)
                    {
                        displayName = displayName.substr(0, 16) + "...";
                    }

                    if (IsFileExtension(files.paths[i], ".json"))
                    {
                        if (GuiButton(Rectangle{ projectPanelRec.x + 5 + (((float)GetScreenWidth() - 240 - 240) / 2) / columns * (i % columns), projectPanelScroll.y + (float)(projectPanelRec.y + 5 + 35 * (i / columns)), (((float)GetScreenWidth() - 240 - 240) / 2) / columns - 24, 30 }, GuiIconText(ICON_FILE_SAVE, displayName.c_str())))
                        {
                        }
                    }
                    else if (IsPathFile(files.paths[i]))
                    {
                        if (GuiButton(Rectangle{ projectPanelRec.x + 5 + (((float)GetScreenWidth() - 240 - 240) / 2) / columns * (i % columns), projectPanelScroll.y + (float)(projectPanelRec.y + 5 + 35 * (i / columns)), (((float)GetScreenWidth() - 240 - 240) / 2) / columns - 24, 30 }, GuiIconText(ICON_FILE_SAVE, displayName.c_str())))
                        {
                        }
                    }
                    else
                    {
                        if (GuiButton(Rectangle{ projectPanelRec.x + 5 + (((float)GetScreenWidth() - 240 - 240) / 2) / columns * (i % columns), projectPanelScroll.y + (float)(projectPanelRec.y + 5 + 35 * (i / columns)), (((float)GetScreenWidth() - 240 - 240) / 2) / columns - 24, 30 }, GuiIconText(ICON_FOLDER, displayName.c_str())))
                        {
                            currentPath = files.paths[i];
                        }
                    }
                }

                EndScissorMode();
            }

            if (windowConsoleActive)
            {
                windowConsoleActive = !GuiWindowBox(Rectangle{ 240, (float)GetScreenHeight() - 272 - 24, ((float)GetScreenWidth() - 240 - 240) / 2, 272 }, "CONSOLE");

                Rectangle consolePanelView = GuiScrollPanel(consolePanelRec, NULL, consolePanelContentRec, &consolePanelScroll);

                BeginScissorMode(consolePanelView.x, consolePanelView.y, consolePanelView.width, consolePanelView.height);

                for (int i = 0; i < consoleOutput.size(); i++)
                {
                    GuiLabel(Rectangle{ consolePanelRec.x + consolePanelScroll.x, consolePanelRec.y + consolePanelScroll.y + 30 * i, consolePanelView.width - 10, 30 }, consoleOutput[i].c_str());
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
