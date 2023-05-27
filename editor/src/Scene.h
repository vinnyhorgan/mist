#pragma once

#include "Entity.h"

#include <vector>

class Scene
{
public:
    std::vector<Entity> entities;

    Scene();

    void addEntity(Entity entity);
    void removeEntity(Entity entity);

    void save();
    void load();
};
