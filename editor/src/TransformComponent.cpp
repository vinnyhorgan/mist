#include "TransformComponent.h"


TransformComponent::TransformComponent() : Component("Transform")
{
    position = Vector3{ 0, 0, 0 };
    rotation = Vector3{ 0, 0, 0 };
    scale = Vector3{ 1, 1, 1 };
}

json TransformComponent::serialize()
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

void TransformComponent::deserialize(json transform)
{
    json position = transform["position"];
    json rotation = transform["rotation"];
    json scale = transform["scale"];
    this->position = Vector3{ position["x"], position["y"], position["z"] };
    this->rotation = Vector3{ rotation["x"], rotation["y"], rotation["z"] };
    this->scale = Vector3{ scale["x"], scale["y"], scale["z"] };
}
