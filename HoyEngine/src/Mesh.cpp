#include "Mesh.h"
#include "TextureLoader.h"

using namespace std;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Matrix;
using DirectX::BoundingSphere;

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    : m_vertexBuffer(vertices), m_indexBuffer(indices)
{
    m_indexCount = (UINT)indices.size();

    m_meshMapInfoBuffer = ConstantBuffer<TextureStatus>(VERTEX_SHADER | PIXEL_SHADER, 1);
}

void Mesh::Init(ComPtr<ID3D11Device>& device)
{
    m_meshMapInfoBuffer.GetData().hasAlbedoMap = !m_diffuseFilename.empty();
    m_meshMapInfoBuffer.GetData().hasNormalMap = !m_normalFilename.empty();
    m_meshMapInfoBuffer.GetData().hasHeightMap = !m_heightFilename.empty();

    LoadTextures(device);

    m_vertexBuffer.Init(device);
    m_indexBuffer.Init(device);
    m_meshMapInfoBuffer.Init(device);
}

void Mesh::UpdateBuffer(ComPtr<ID3D11DeviceContext>& context)
{
    m_meshMapInfoBuffer.Update(context);
}

void Mesh::Render(ComPtr<ID3D11DeviceContext>& context)
{
    m_meshMapInfoBuffer.Bind(context);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetBuffer().GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
    context->DrawIndexed(m_indexCount, 0, 0);
}

void Mesh::RenderNormal(ComPtr<ID3D11DeviceContext>& context)
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetBuffer().GetAddressOf(), &stride, &offset);
    //context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->Draw(UINT(m_vertexBuffer.GetData().size()), 0);
}

void Mesh::CopySquareRenderSetup(ComPtr<ID3D11DeviceContext>& context)
{
    // PS SET CONSTANT ( ex) gamma )
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetBuffer().GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

    context->DrawIndexed(m_indexCount, 0, 0);
}

void Mesh::LoadDDSTexture(ComPtr<ID3D11Device>& device, const wstring filepath)
{
    //const wstring diffuseFilename = filepath + L"worn-painted-metal_albedo.dds";
    const wstring diffuseFilename = filepath + L"wet-mossy-rocks_albedo.dds";

    TextureLoader::CreateDDSTexture(device, diffuseFilename, m_diffuseSRV);
}

void Mesh::LoadTexture(ComPtr<ID3D11Device>& device, const std::string filepath, ComPtr<ID3D11ShaderResourceView>& srv, const bool gammaDecode)
{
    TextureLoader::CreateTextureFromImage(device, filepath, srv, gammaDecode);
}

void Mesh::LoadTextures(ComPtr<ID3D11Device>& device)
{
    if (m_meshMapInfoBuffer.GetData().hasAlbedoMap)
    {
        cout << "Loading diffuse map...: ";
        TextureLoader::CreateTextureFromImage(device, m_diffuseFilename, m_diffuseSRV, true);
    }
    else
    {
        cout << "No diffuse map" << endl;
    }
    if (m_meshMapInfoBuffer.GetData().hasNormalMap)
    {
        cout << "Loading normal map...: ";
        TextureLoader::CreateTextureFromImage(device, m_normalFilename, m_normalSRV, false);
    }
    else
    {
        cout << "No normal map" << endl;
    }
    if (m_meshMapInfoBuffer.GetData().hasHeightMap)
    {
        cout << "Loading height map...: ";
        TextureLoader::CreateTextureFromImage(device, m_heightFilename, m_heightSRV, false);
    }
    else
    {
        cout << "No height map" << endl;
    }
}

void Mesh::SetSRVs(ComPtr<ID3D11DeviceContext>& context)
{
    context->VSSetShaderResources(0, 1, m_heightSRV.GetAddressOf());
    ID3D11ShaderResourceView* textures[] = { m_diffuseSRV.Get(), m_normalSRV.Get() };
    context->PSSetShaderResources(0, 2, textures);
}