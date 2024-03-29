#pragma once

#include "Material.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

// 16Byte align
struct ModelBuffer // Must Store as a Column Matrix 
{   // every model own this const buffer
    DirectX::SimpleMath::Matrix world; // equal model matrix 
    DirectX::SimpleMath::Matrix worldIT;

    Material material; // 32Bytes, It's better having material each mesh for lighting, but art it later
    
    // for test 
    int activeAlbedoMap = 1;
    int activeNormalMap = 1;
    int activeHeightMap = 1;
    int padding;
};

// 16Byte align
struct TextureStatus 
{   // every mesh own this const buffer
    int hasAlbedoMap = 0;
    int hasNormalMap = 0;
    int hasHeightMap = 0;
    int padding;
};

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

    void LoadDDSTexture(ComPtr<ID3D11Device>& device, const std::wstring filepath);
    void LoadTexture(ComPtr<ID3D11Device>& device, const std::string filepath, ComPtr<ID3D11ShaderResourceView>& srv, const bool gammaDecode);
    void LoadTextures(ComPtr<ID3D11Device>& device);
    void SetSRVs(ComPtr<ID3D11DeviceContext>& context);

public:
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    UINT m_indexCount = 0;

    ConstantBuffer<TextureStatus> m_meshMapInfoBuffer;  // each mesh has a different buffer 

    // Textures
    std::string m_diffuseFilename;
    std::string m_normalFilename;
    std::string m_heightFilename;
    ComPtr<ID3D11ShaderResourceView> m_diffuseSRV;
    ComPtr<ID3D11ShaderResourceView> m_normalSRV;
    ComPtr<ID3D11ShaderResourceView> m_heightSRV;
};