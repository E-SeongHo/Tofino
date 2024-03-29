
#include <vector>
#include <iostream>

#include "Geometry.h"

using namespace std;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Matrix;
using DirectX::BoundingSphere;

//void Geometry::ReverseIndices()
//{
//    std::reverse(m_indices.begin(), m_indices.end());
//}

void Triangle::LoadGeometry(const float scale)
{
    vector<Vertex> vertices;
    vector<uint32_t> indices;

    vertices.push_back(Vertex{ Vector3{ -1.0f, -1.0f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });
    vertices.push_back(Vertex{ Vector3{ 0.0f, 1.0f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });
    vertices.push_back(Vertex{ Vector3{ 1.0f, -1.0f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });

    indices = { 0, 1, 2 };

    m_meshes.push_back(Mesh(vertices, indices));
}

void Square::LoadGeometry(const float scale)
{
    vector<Vertex> vertices;
    vector<uint32_t> indices;

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

    for (size_t i = 0; i < positions.size(); i++) 
    {
        Vertex v;
        v.pos = positions[i];
        v.color = colors[i];
        v.normal = normals[i];
        v.uv = texcoords[i];

        vertices.push_back(v);
    }
    indices = { 0, 1, 2, 0, 2, 3, };

    m_meshes.push_back(Mesh(vertices, indices));
}

void Cube::LoadGeometry(const float scale)
{
    vector<Vertex> vertices;
    vector<uint32_t> indices;

    vector<Vector3> positions;
    vector<Vector3> colors;
    vector<Vector3> normals;
    vector<Vector2> texcoords; 

    // À­¸é
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

    // ¾Æ·§¸é
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

    // ¾Õ¸é
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

    // µÞ¸é
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

    // ¿ÞÂÊ
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

    // ¿À¸¥ÂÊ
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

    for (size_t i = 0; i < positions.size(); i++) 
    {
        Vertex v;
        v.pos = positions[i];
        v.color = colors[i];
        v.normal = normals[i];
        v.uv = texcoords[i];

        vertices.push_back(v);
    }

    indices = 
    {
        0,  1,  2,  0,  2,  3,  // À­¸é
        4,  5,  6,  4,  6,  7,  // ¾Æ·§¸é
        8,  9,  10, 8,  10, 11, // ¾Õ¸é
        12, 13, 14, 12, 14, 15, // µÞ¸é
        16, 17, 18, 16, 18, 19, // ¿ÞÂÊ
        20, 21, 22, 20, 22, 23  // ¿À¸¥ÂÊ
    };

    m_meshes.push_back(Mesh(vertices, indices));

    m_boundingSphere = BoundingSphere(Vector3(0.0f, 0.0f, 0.0f), scale / 2.0f);
}

void Sphere::LoadGeometry(const float scale)
{
    // https://www.songho.ca/opengl/gl_sphere.html
    using namespace DirectX;

    vector<Vertex> vertices;
    vector<uint32_t> indices;

    vector<Vector3> positions;
    vector<Vector3> colors;
    vector<Vector3> normals;
    vector<Vector2> texcoords;
    vector<Vector3> tangents;

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
            
            // Compute tangent of this mesh.
            // Partial derivative of P with respect to theta
            Vector3 T;
            T.x = -radius * sinf(stackAngle) * sinf(sectorAngle);
            T.y = 0.0f;
            T.z = radius * sinf(stackAngle) * cosf(sectorAngle);
            T.Normalize();
            tangents.push_back(T);
        }
    }

    for (size_t i = 0; i < positions.size(); i++) 
    {
        Vertex v;
        v.pos = positions[i];
        v.color = Vector3(0.5f, 0.5f, 0.5f);
        v.normal = normals[i];
        v.uv = texcoords[i];
        v.tangent = tangents[i];

        vertices.push_back(v);
    }

    for (int i = 0; i < stackCount; ++i)
    {
        const int offset = (sectorCount + 1) * i;
        
        for (int j = 0; j < sectorCount; ++j)
        {
            indices.push_back(offset + j);
            indices.push_back(offset + j + 1);
            indices.push_back(offset + j + sectorCount + 1);
            
            indices.push_back(offset + j + 1);
            indices.push_back(offset + j + 1 + sectorCount + 1);
            indices.push_back(offset + j + sectorCount + 1);
        }
    }

    m_meshes.push_back(Mesh(vertices, indices));

    m_boundingSphere = BoundingSphere(Vector3(0.0f, 0.0f, 0.0f), radius);
}
