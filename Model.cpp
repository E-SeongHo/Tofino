
#include <vector>
#include <iostream>

#include "Model.h"
#include "TextureLoader.h"

using namespace std;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Matrix;
using DirectX::BoundingSphere;

void Geometry::ReverseIndices()
{
    std::reverse(m_indices.begin(), m_indices.end());
}
void Geometry::CreateBuffers(ComPtr<ID3D11Device>& device)
{
    Geometry::CreateVertexBuffer(device, m_vertices, m_vertexBuffer);
    Geometry::CreateIndexBuffer(device, m_indices, m_indexBuffer);
    Util::CreateConstantBuffer(device, m_constBufferCPU, m_constBufferGPU);
}

void Geometry::UpdateBuffer(ComPtr<ID3D11DeviceContext>& context)
{
    Util::UpdateConstantBuffer(context, m_constBufferCPU, m_constBufferGPU);
}

void Geometry::Render(ComPtr<ID3D11DeviceContext>& context)
{
    context->VSSetConstantBuffers(0, 1, m_constBufferGPU.GetAddressOf()); 

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->DrawIndexed(m_indexCount, 0, 0);
}

void Geometry::RenderNormal(ComPtr<ID3D11DeviceContext>& context)
{
    context->VSSetConstantBuffers(0, 1, m_constBufferGPU.GetAddressOf());

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    //context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->Draw(UINT(m_vertices.size()), 0);
}

void Geometry::CopySquareRenderSetup(ComPtr<ID3D11DeviceContext>& context)
{
    // PS SET CONSTANT ( ex) gamma )
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    context->DrawIndexed(m_indexCount, 0, 0);
}


DirectX::SimpleMath::Matrix Geometry::GetWorldMatrix()
{
    return m_constBufferCPU.world.Transpose();
}


void Geometry::UpdateWorldMatrix(DirectX::SimpleMath::Matrix worldRow)
{
    Matrix worldColumn = worldRow.Transpose();
    m_constBufferCPU.world = worldColumn;
    
    m_constBufferCPU.worldIT = worldColumn;
    m_constBufferCPU.worldIT.Translation(Vector3(0.0f));
    m_constBufferCPU.worldIT.Invert().Transpose();
}

void Geometry::LoadTexture(ComPtr<ID3D11Device>& device, const wstring filepath)
{
    //const wstring diffuseFilename = filepath + L"worn-painted-metal_albedo.dds";
    const wstring diffuseFilename = filepath + L"wet-mossy-rocks_albedo.dds";

    TextureLoader::CreateDDSTexture(device, diffuseFilename, m_diffuseSRV);
}

void Geometry::SetSRVs(ComPtr<ID3D11DeviceContext>& context)
{
    context->PSSetShaderResources(0, 1, m_diffuseSRV.GetAddressOf());
}

void Triangle::Init(const float scale, bool isHittable)
{
    m_vertices.push_back(Vertex{ Vector3{ -1.0f, -1.0f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });
    m_vertices.push_back(Vertex{ Vector3{ 0.0f, 1.0f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });
    m_vertices.push_back(Vertex{ Vector3{ 1.0f, -1.0f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });

    m_indices = { 0, 1, 2 };
    m_indexCount = (UINT)m_indices.size();

    m_constBufferCPU.world = Matrix();
}

void Square::Init(const float scale, bool isHittable)
{
    vector<Vector3> positions;
    vector<Vector3> colors;
    vector<Vector3> normals;
    vector<Vector2> texcoords; 

    positions.push_back(Vector3(-1.0f, 1.0f, 0.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 0.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 0.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, 0.0f) * scale);
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.pos = positions[i];
        v.color = colors[i];
        v.normal = normals[i];
        v.uv = texcoords[i];

        m_vertices.push_back(v);
    }
    m_indices = { 0, 1, 2, 0, 2, 3, };

    m_indexCount = (UINT)m_indices.size();

    m_constBufferCPU.world = Matrix();
}

void Cube::Init(const float scale, bool isHittable)
{
    vector<Vector3> positions;
    vector<Vector3> colors;
    vector<Vector3> normals;
    vector<Vector2> texcoords; 

    // ����
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // �Ʒ���
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // �ո�
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // �޸�
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // ����
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // ������
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.pos = positions[i];
        v.color = colors[i];
        v.normal = normals[i];
        v.uv = texcoords[i];

        m_vertices.push_back(v);
    }

    m_indices = {
        0,  1,  2,  0,  2,  3,  // ����
        4,  5,  6,  4,  6,  7,  // �Ʒ���
        8,  9,  10, 8,  10, 11, // �ո�
        12, 13, 14, 12, 14, 15, // �޸�
        16, 17, 18, 16, 18, 19, // ����
        20, 21, 22, 20, 22, 23  // ������
    };
    m_indexCount = (UINT)m_indices.size();

    m_constBufferCPU.world = Matrix();
}

void Sphere::Init(const float scale, bool isHittable)
{
    // https://www.songho.ca/opengl/gl_sphere.html
    using namespace DirectX;

    vector<Vector3> positions;
    vector<Vector3> colors;
    vector<Vector3> normals;
    vector<Vector2> texcoords;

    const float radius = 1.0f * scale;
    const int sectorCount = 20;
    const int stackCount = 20;

    float sectorStep = XM_2PI / (float)sectorCount;
    float stackStep = XM_PI / (float)stackCount;

    for (int i = 0; i <= stackCount; ++i)
    {
        float stackAngle = (float)i * stackStep;
        Vector3 stackStart = Vector3::Transform(Vector3(0.0f, -radius, 0.0f), Matrix::CreateRotationZ(stackAngle));
        
        // add (sectorCount+1) vertices per stack
        // first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            float sectorAngle = (float)j * sectorStep;
            Vector3 currentPoint = Vector3::Transform(stackStart, Matrix::CreateRotationY(sectorAngle));
            positions.push_back(currentPoint);
            
            currentPoint.Normalize();
            normals.push_back(currentPoint);

            float u = (float)j / sectorCount;
            float v = 1.0f - ((float)i / stackCount);
            texcoords.push_back(Vector2(u, v));
        }
    }

    for (size_t i = 0; i < positions.size(); i++) 
    {
        Vertex v;
        v.pos = positions[i];
        v.color = Vector3(0.5f, 0.5f, 0.5f);
        v.normal = normals[i];
        v.uv = texcoords[i];

        m_vertices.push_back(v);
    }

    for (int i = 0; i < stackCount; ++i)
    {
        const int offset = (sectorCount + 1) * i;
        
        for (int j = 0; j < sectorCount; ++j)
        {
            m_indices.push_back(offset + j);
            m_indices.push_back(offset + j + 1);
            m_indices.push_back(offset + j + sectorCount + 1);
            
            m_indices.push_back(offset + j + 1);
            m_indices.push_back(offset + j + 1 + sectorCount + 1);
            m_indices.push_back(offset + j + sectorCount + 1);
        }
    }

    m_indexCount = (UINT)m_indices.size();
    m_constBufferCPU.world = Matrix();

    if (isHittable)
    {
        onActive = isHittable;
        m_boundingSphere = BoundingSphere(Vector3(0.0f, 0.0f, 0.0f), radius);
    }
}

void EnvMap::Init(ComPtr<ID3D11Device>& device, const wstring filepath)
{
    m_mesh = new Cube();
    m_mesh->Init(100.0f);
    m_mesh->ReverseIndices();

    const wstring envFilename = filepath + L"EnvHDR.dds";
    const wstring irradianceFilename = filepath + L"DiffuseHDR.dds";
    const wstring specularFilename = filepath + L"SpecularHDR.dds";
    const wstring brdfFilename = filepath + L"Brdf.dds";

    TextureLoader::CreateDDSCubemapTexture(device, envFilename, m_envSRV);
    TextureLoader::CreateDDSCubemapTexture(device, irradianceFilename, m_irradianceSRV);
    TextureLoader::CreateDDSCubemapTexture(device, specularFilename, m_specularSRV);
    TextureLoader::CreateDDSTexture(device, brdfFilename, m_brdfLookUpSRV);

    m_mesh->CreateBuffers(device); // m_mesh�� private�̹Ƿ� ���⼭ Buffer ����
}

void EnvMap::Render(ComPtr<ID3D11DeviceContext>& context)
{
    context->PSSetShaderResources(0, 1, m_envSRV.GetAddressOf());

    m_mesh->Render(context);
}

void EnvMap::func(ComPtr<ID3D11DeviceContext>& context)
{
    
}

EnvMap::~EnvMap()
{
    delete(m_mesh);
}