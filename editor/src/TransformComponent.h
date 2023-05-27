#pragma once

#include "Component.h"

#include <raymath.h>
#include <json.hpp>

using json = nlohmann::json;

class TransformComponent : public Component
{
public:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    TransformComponent();

    json serialize();
    void deserialize(json transform);
};
