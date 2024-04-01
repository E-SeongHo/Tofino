#pragma once

#include "Material.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

using Microsoft::WRL::ComPtr;
class Mesh
{
public:
    Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
    
    void Init(ComPtr<ID3D11Device>& device);
    void UpdateBuffer(ComPtr<ID3D11DeviceContext>& context);
    void Render(ComPtr<ID3D11DeviceContext>& context);
    void RenderNormal(ComPtr<ID3D11DeviceContext>& context);
    void CopySquareRenderSetup(ComPtr<ID3D11DeviceContext>& context);

    bool IsUpdateFlagSet();
    Material& GetMaterial();
    void SetMaterialFactors(DirectX::SimpleMath::Vector4 baseColor, const float roughness, const float metallic);

public:
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    UINT m_indexCount = 0;

    Material m_material;
    bool m_updateFlag = false;
};