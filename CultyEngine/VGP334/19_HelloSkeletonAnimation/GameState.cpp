#include "GameState.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;
using namespace CultyEngine::Input;

namespace
{
    void CameraControl(float deltaTime, Camera& mCamera)
    {
        auto input = Input::InputSystem::Get();
        const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
        const float turnSpeed = 0.1f;

        if (input->IsKeyDown(KeyCode::W))
            mCamera.Walk(moveSpeed * deltaTime);
        else if (input->IsKeyDown(KeyCode::S))
            mCamera.Walk(-moveSpeed * deltaTime);

        if (input->IsKeyDown(KeyCode::A))
            mCamera.Strafe(-moveSpeed * deltaTime);
        else if (input->IsKeyDown(KeyCode::D))
            mCamera.Strafe(moveSpeed * deltaTime);

        if (input->IsKeyDown(KeyCode::Q))
            mCamera.Rise(-moveSpeed * deltaTime);
        else if (input->IsKeyDown(KeyCode::E))
            mCamera.Rise(moveSpeed * deltaTime);

        if (input->IsMouseDown(MouseButton::RBUTTON))
        {
            mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
            mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
        }
    }
}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 2.0f, -2.0f });
    mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

    mDirectionalLight.direction = MathC::Normalize({1.0f, -1.0f, 1.0f});
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    mModelID_01 = ModelManager::Get()->LoadModelID("../../Assets/Models/Character02/Ch03_nonPBR.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/Bashful.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/Bboy.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/Capoeira.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/Flair.fbx");
    ModelManager::Get()->AddAnimation(mModelID_01, "../../Assets/Models/Character02/Animations/SoulSpinCombo.fbx");
    mCharacter_01 = CreateRenderGroup(mModelID_01, &mCharacterAnimator_01);
    mCharacterAnimator_01.Initialize(mModelID_01);
    SetRenderGroupPosition(mCharacter_01, { 0.0f, 0.5f, 0.0f });

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFilePath);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();
    CleanupRenderGroup(mCharacter_01);
}

void GameState::Update(float deltaTime)
{
    CameraControl(deltaTime, mCamera);

    mCharacterAnimator_01.Update(deltaTime);
}

void GameState::Render()
{
    if (mDrawSkeleton)
    {
        AnimationUtils::BoneTransforms boneTransforms;

        AnimationUtils::ComputeBoneTransforms(mModelID_01, boneTransforms, &mCharacterAnimator_01);
        AnimationUtils::DrawSkeleton(mModelID_01, boneTransforms);
    }

    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    if (mDrawSkeleton == false)
    {
        mStandardEffect.Begin();
            DrawRenderGroup(mStandardEffect, mCharacter_01);
        mStandardEffect.End();
    }
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
                mDirectionalLight.direction = MathC::Normalize(mDirectionalLight.direction);

            ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
            ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
            ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
        }

        ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);

        if (ImGui::DragInt("Animation", &mAnimationIndex, 1, -1, mCharacterAnimator_01.GetAnimationCount() - 1))
            mCharacterAnimator_01.PlayAnimation(mAnimationIndex, true);

        mStandardEffect.DebugUI();
    ImGui::End();
}