#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>

struct Vertex 
{ // Match with input element
    DirectX::SimpleMath::Vector3 pos;
    DirectX::SimpleMath::Vector3 color;
};

struct MVPBuffer 
{
    DirectX::SimpleMath::Matrix world; // equal model matrix
    DirectX::SimpleMath::Matrix view;
    DirectX::SimpleMath::Matrix projection;
};

using Microsoft::WRL::ComPtr;
class Geometry
{
protected:
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    ComPtr<ID3D11Buffer> m_constBufferGPU;
    UINT m_indexCount;

    MVPBuffer m_constBufferCPU;

public:
    virtual void Init() = 0;
};

class Triangle : public Geometry
{
    void Init() override;
};