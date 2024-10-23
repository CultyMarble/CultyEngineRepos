#pragma once
#include "Component.h"

namespace CultyEngine
{
    class CameraComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::Camera);

        void Initialize() override;
        void Terminate() override;
        void DebugUI() override;

        Graphics::Camera& GetCamera();
        const Graphics::Camera& GetCamera() const;

    private:
        Graphics::Camera mCamera;
    };
}