#include "Precompiled.h"
#include "VertexShader.h"

#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace CultyEngine;
using namespace CultyEngine::Graphics;

namespace
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexLayout(uint32_t vertexFormat)
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
        if (vertexFormat & VE_Position)
            desc.push_back({ "POSITION", 0, 
                DXGI_FORMAT_R32G32B32_FLOAT, 0, 
                D3D11_APPEND_ALIGNED_ELEMENT, 
                D3D11_INPUT_PER_VERTEX_DATA, 0 });

        if (vertexFormat & VE_Normal)
            desc.push_back({ "NORMAL", 0, 
                DXGI_FORMAT_R32G32B32_FLOAT, 0, 
                D3D11_APPEND_ALIGNED_ELEMENT, 
                D3D11_INPUT_PER_VERTEX_DATA, 0 });

        if (vertexFormat & VE_Tangent)
            desc.push_back({ "TANGENT", 0, 
                DXGI_FORMAT_R32G32B32_FLOAT, 0, 
                D3D11_APPEND_ALIGNED_ELEMENT, 
                D3D11_INPUT_PER_VERTEX_DATA, 0 });

        if (vertexFormat & VE_Color)
            desc.push_back({ "COLOR", 0, 
                DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 
                D3D11_APPEND_ALIGNED_ELEMENT, 
                D3D11_INPUT_PER_VERTEX_DATA, 0 });

        if (vertexFormat & VE_TexCoord)
            desc.push_back({ "TEXCOORD", 0, 
                DXGI_FORMAT_R32G32_FLOAT, 0, 
                D3D11_APPEND_ALIGNED_ELEMENT, 
                D3D11_INPUT_PER_VERTEX_DATA, 0 });

        if (vertexFormat & VE_BlendIndex)
            desc.push_back({ "BLENDINDICES", 0, 
                DXGI_FORMAT_R32G32B32A32_SINT, 0, 
                D3D11_APPEND_ALIGNED_ELEMENT, 
                D3D11_INPUT_PER_VERTEX_DATA, 0 });

        if (vertexFormat & VE_BlendWeight)
            desc.push_back({ "BLENDWEIGHT", 0, 
                DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 
                D3D11_APPEND_ALIGNED_ELEMENT, 
                D3D11_INPUT_PER_VERTEX_DATA, 0 });

        return desc;
    }
}

void VertexShader::Initialize(const std::filesystem::path& filePath, uint32_t format)
{
    auto device = CultyEngine::Graphics::GraphicsSystem::Get()->GetDevice();
    //===========================================================================
    // Need to create a vertex shader

    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HRESULT hResult = D3DCompileFromFile(
        filePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS", "vs_5_0",
        shaderFlags, 0,
        &shaderBlob,
        &errorBlob
    );

    if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
    {
        LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
    }
    ASSERT(SUCCEEDED(hResult), "Failed to compile vertex shader!");

    hResult = device->CreateVertexShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        &mVertexShader
    );
    ASSERT(SUCCEEDED(hResult), "Failed to create vertex shader!");

    //===========================================================================
    // Need to create input layout
    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout = GetVertexLayout(format);

    hResult = device->CreateInputLayout(
        vertexLayout.data(),
        static_cast<UINT>(vertexLayout.size()),
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        &mInputLayout
    );

    ASSERT(SUCCEEDED(hResult), "Failed to create input layout!");
    SafeRelease(shaderBlob);
    SafeRelease(errorBlob);
}

void VertexShader::Terminate()
{
    SafeRelease(mInputLayout);
    SafeRelease(mVertexShader);
}

void VertexShader::Bind()
{
    auto context = GraphicsSystem::Get()->GetContext();

    // bind
    context->VSSetShader(mVertexShader, nullptr, 0);
    context->IASetInputLayout(mInputLayout);
}