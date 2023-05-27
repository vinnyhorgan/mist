#include "Scene.h"

#include "Logger.h"
#include "Entity.h"
#include "TransformComponent.h"

#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

Scene::Scene()
{
    Logger::info("Created scene!");
}

void Scene::addEntity(Entity entity)
{
    entities.push_back(entity);
}

void Scene::removeEntity(Entity entity)
{
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i].name == entity.name)
        {
            entities.erase(entities.begin() + i);
        }
    }
}

void Scene::save()
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

void Scene::load()
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
