#pragma once

namespace CultyEngine
{
    enum class ComponentID
    {
        Invalid,    // default value
        Transform,  // transform component for location data
        Camera,     // contain a Camera for viewing
        FPSCamera,  // move Camera with FPS controller
        Mesh,       // create a mesh for render object
        Count
    };

    enum class ServiceID
    {
        Invalid,
        Camera,
        Render,
        Count
    };
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypeID() { return static_cast<uint32_t>(id); }\
    uint32_t GetTypeID() const override { return StaticGetTypeID(); }