#include "Entity.h"

#include "Logger.h"
#include "TransformComponent.h"


Entity::Entity(std::string name)
{
    this->name = name;

    TransformComponent transform = TransformComponent();
    addComponent(transform);

    Logger::info("Created entity: " + name);
}

void Entity::addComponent(Component component)
{
    components.push_back(component);
}

void Entity::removeComponent(Component component)
{
    for (int i = 0; i < components.size(); i++)
    {
        if (components[i].name == component.name)
        {
            components.erase(components.begin() + i);
        }
    }
}
