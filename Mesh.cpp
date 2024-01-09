#include "Mesh.h"
#include "TextureLoader.h"

using namespace std;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Matrix;
using DirectX::BoundingSphere;

void Mesh::CreateBuffers(ComPtr<ID3D11Device>& device)
{
    Util::CreateVertexBuffer(device, m_vertices, m_vertexBuffer);
    Util::CreateIndexBuffer(device, m_indices, m_indexBuffer);
    Util::CreateConstantBuffer(device, m_constBufferCPU, m_constBufferGPU);
}

void Mesh::UpdateBuffer(ComPtr<ID3D11DeviceContext>& context)
{
    Util::UpdateConstantBuffer(context, m_constBufferCPU, m_constBufferGPU);
}

void Mesh::Render(ComPtr<ID3D11DeviceContext>& context)
{
    context->VSSetConstantBuffers(0, 1, m_constBufferGPU.GetAddressOf());
    context->PSSetConstantBuffers(0, 1, m_constBufferGPU.GetAddressOf());

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->DrawIndexed(m_indexCount, 0, 0);
}

void Mesh::RenderNormal(ComPtr<ID3D11DeviceContext>& context)
{
    context->VSSetConstantBuffers(0, 1, m_constBufferGPU.GetAddressOf());

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    //context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->Draw(UINT(m_vertices.size()), 0);
}

void Mesh::CopySquareRenderSetup(ComPtr<ID3D11DeviceContext>& context)
{
    // PS SET CONSTANT ( ex) gamma )
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    context->DrawIndexed(m_indexCount, 0, 0);
}


DirectX::SimpleMath::Matrix Mesh::GetWorldMatrix()
{
    return m_constBufferCPU.world.Transpose();
}


void Mesh::UpdateWorldMatrix(DirectX::SimpleMath::Matrix worldRow)
{
    Matrix worldColumn = worldRow.Transpose();
    m_constBufferCPU.world = worldColumn;

    m_constBufferCPU.worldIT = worldColumn;
    m_constBufferCPU.worldIT.Translation(Vector3(0.0f));
    m_constBufferCPU.worldIT.Invert().Transpose();
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
    TextureLoader::CreateTextureFromImage(device, m_diffuseFilename, m_diffuseSRV, true);
    TextureLoader::CreateTextureFromImage(device, m_normalFilename, m_normalSRV, false);
    TextureLoader::CreateTextureFromImage(device, m_heightFilename, m_heightSRV, false);
}
void Mesh::SetSRVs(ComPtr<ID3D11DeviceContext>& context)
{
    context->VSSetShaderResources(0, 1, m_heightSRV.GetAddressOf());
    ID3D11ShaderResourceView* textures[] = { m_diffuseSRV.Get(), m_normalSRV.Get() };
    context->PSSetShaderResources(0, 2, textures);
}