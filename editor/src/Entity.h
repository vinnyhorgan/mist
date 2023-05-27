#pragma once

#include "Component.h"

#include <string>
#include <vector>

class Entity
{
public:
    std::string name;
    std::vector<Component> components;

    Entity(std::string name);

    void addComponent(Component component);
    void removeComponent(Component component);
};
