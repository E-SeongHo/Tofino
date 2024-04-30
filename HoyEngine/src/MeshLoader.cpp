
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <DirectXMesh.h>

#include "MeshLoader.h"
#include "Mesh.h"
#include "Graphics.h"

namespace Tofino
{
    std::vector<Mesh> MeshLoader::LoadTriangle(const float scale)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        vertices.push_back(Vertex{ Vector3{ -1.0f, -1.0f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });
        vertices.push_back(Vertex{ Vector3{ 0.0f, 1.0f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });
        vertices.push_back(Vertex{ Vector3{ 1.0f, -1.0f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });

        indices = { 0, 1, 2 };

        return std::vector<Mesh>({ Mesh(vertices, indices) }); // RVO
    }

    std::vector<Mesh> MeshLoader::LoadSquare(const float scale)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        std::vector<Vector3> positions;
        std::vector<Vector3> colors;
        std::vector<Vector3> normals;
        std::vector<Vector2> texcoords;

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

        return std::vector<Mesh>({ Mesh(vertices, indices) }); // RVO
    }

    std::vector<Mesh> MeshLoader::LoadCube(const float scale)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        std::vector<Vector3> positions;
        std::vector<Vector3> colors;
        std::vector<Vector3> normals;
        std::vector<Vector2> texcoords;

        // up
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

        // down
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

        // front
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

        // rear
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

        // left
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

        // right
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
            0,  1,  2,  0,  2,  3,  // up
            4,  5,  6,  4,  6,  7,  // down
            8,  9,  10, 8,  10, 11, // front
            12, 13, 14, 12, 14, 15, // rear
            16, 17, 18, 16, 18, 19, // left
            20, 21, 22, 20, 22, 23  // right
        };

        return std::vector<Mesh>({ Mesh(vertices, indices) }); // RVO
    }

    std::vector<Mesh> MeshLoader::LoadSphere(const float scale)
    {
        // https://www.songho.ca/opengl/gl_sphere.html

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        std::vector<Vector3> positions;
        std::vector<Vector3> colors;
        std::vector<Vector3> normals;
        std::vector<Vector2> texcoords;
        std::vector<Vector3> tangents;

        const float radius = 1.0f * scale;
        const int sectorCount = 20;
        const int stackCount = 20;

        float sectorStep = DirectX::XM_2PI / (float)sectorCount;
        float stackStep = DirectX::XM_PI / (float)stackCount;

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
        
        return std::vector<Mesh>({ (Mesh(vertices, indices)) });
    }

    std::vector<Mesh> MeshLoader::LoadModel(const std::string& filename, const float scale)
    {
        ModelReader mr;
        mr.ReadModelFile(filename, scale);

        return std::move(mr.meshes);
    }

    void MeshLoader::ModelReader::ReadModelFile(const std::string& filename, const float scale)
    {
        using namespace DirectX;

        std::cout << "Reading model from... "  << filename << std::endl;

        size_t idx = filename.rfind("/");
        directory = filename.substr(0, idx + 1);

        Assimp::Importer importer;
        const ::aiScene* scene = importer.ReadFile(filename,
            aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

        if (!scene)
        {
            std::cout << "Failed to load model ( " << filename << " ) " << importer.GetErrorString() << std::endl;
            assert(false);
        	return;
        }

        Matrix tr; // Initial transformation
        ReadNode(scene->mRootNode, scene, tr);

        // Normalize
        Vector3 vmin(1000, 1000, 1000);
        Vector3 vmax(-1000, -1000, -1000);
        for (auto& mesh : meshes)
        {
            for (auto& v : mesh.GetVertexBuffer().GetData())
            {
                vmin.x = XMMin(vmin.x, v.pos.x);
                vmin.y = XMMin(vmin.y, v.pos.y);
                vmin.z = XMMin(vmin.z, v.pos.z);
                vmax.x = XMMax(vmax.x, v.pos.x);
                vmax.y = XMMax(vmax.y, v.pos.y);
                vmax.z = XMMax(vmax.z, v.pos.z);
            }
        }

        float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
        float dl = XMMax(XMMax(dx, dy), dz);
        float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f,
            cz = (vmax.z + vmin.z) * 0.5f;

        for (auto& mesh : meshes) {
            for (auto& v : mesh.GetVertexBuffer().GetData()) {
                v.pos.x = (v.pos.x - cx) / dl;
                v.pos.y = (v.pos.y - cy) / dl;
                v.pos.z = (v.pos.z - cz) / dl;
            }
        }

        std::cout << "Total meshes : " << meshes.size() << std::endl;
        std::cout << " Material Mapping status : " << std::endl;

        for (int i = 0; i < meshes.size(); i++)
        {
            auto& mat = meshes[i].GetMaterial();
            std::cout << "Mesh[ " << i << " ]" << std::endl;
            std::cout << "albedo map : " << mat.GetMaterialStatus().hasAlbedoMap << std::endl;
            std::cout << "normal map : " << mat.GetMaterialStatus().hasNormalMap << std::endl;
            std::cout << "height map : " << mat.GetMaterialStatus().hasHeightMap << std::endl;
        }
    }

    void MeshLoader::ModelReader::ReadNode(aiNode* node, const aiScene* scene, Matrix tr)
    {
        Matrix m;
        ai_real* temp = &node->mTransformation.a1;
        float* mTemp = &m._11;
        for (int t = 0; t < 16; t++)
        {
            mTemp[t] = float(temp[t]);
        }
        m = m.Transpose() * tr;

        for (UINT i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ReadMesh(mesh, scene);
        }

        for (UINT i = 0; i < node->mNumChildren; i++)
        {
            ReadNode(node->mChildren[i], scene, m);
        }
    }

    void MeshLoader::ModelReader::ReadMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        for (UINT i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;

            vertex.pos.x = mesh->mVertices[i].x;
            vertex.pos.y = mesh->mVertices[i].y;
            vertex.pos.z = mesh->mVertices[i].z;

            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
            vertex.normal.Normalize();

            if (mesh->mTextureCoords[0]) // has texcoords
            {
                vertex.uv.x = mesh->mTextureCoords[0][i].x;
                vertex.uv.y = mesh->mTextureCoords[0][i].y;
            }

            vertices.push_back(vertex);
        }

        for (UINT i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (UINT j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        // Compute Tangent Frame
        std::vector<DirectX::XMFLOAT3> positions(vertices.size());
        std::vector<DirectX::XMFLOAT3> normals(vertices.size());
        std::vector<DirectX::XMFLOAT2> texcoords(vertices.size());
        std::vector<DirectX::XMFLOAT4> tangents(vertices.size());

        for (size_t i = 0; i < vertices.size(); i++)
        {
            Vertex& vertex = vertices[i];
            positions[i] = vertex.pos;
            normals[i] = vertex.normal;
            texcoords[i] = vertex.uv;
        }

        DirectX::ComputeTangentFrame(indices.data(), indices.size() / 3,
            positions.data(), normals.data(), texcoords.data(), vertices.size(), tangents.data());

        for (size_t i = 0; i < vertices.size(); i++)
        {
            //cout << "check tangent w : " << tangents[i].w << endl;
            vertices[i].tangent.x = tangents[i].x;
            vertices[i].tangent.y = tangents[i].y;
            vertices[i].tangent.z = tangents[i].z;
            /*cout << newMesh.m_vertices[i].tangent.x << " " << newMesh.m_vertices[i].tangent.y <<
                " " << newMesh.m_vertices[i].tangent.z << endl;*/
        }

        meshes.push_back(Mesh(vertices, indices));
        Mesh& newMesh = meshes.back();

        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            Material& meshMat = newMesh.GetMaterial();

            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString path;
                if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS)
                {
                    int idx = std::string(path.data).rfind("\\");
                    std::string filename = std::string(path.data).substr(idx + 1);

                    std::cout << "Found diffuse texture: " << filename << std::endl;

                    Texture texture(RendererDevice, directory + filename, TextureType::ALBEDO);
                    meshMat.SetAlbedoMap(texture);

                }
                else
                {
                    std::cout << "invalid diffuse texture path" << std::endl;
                }
            }
            else
            {
                std::cout << "No diffuse texture: " << directory << std::endl;
            }

            if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
            {
                aiString path;
                if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == aiReturn_SUCCESS)
                {
                    size_t idx = std::string(path.data).rfind("\\");
                    std::string filename = std::string(path.data).substr(idx + 1);

                    std::cout << "Found normal texture: " << filename << std::endl;

                    Texture texture(RendererDevice, directory + filename, TextureType::NORMAL);
                    meshMat.SetNormalMap(texture);

                }
                else
                {
                    std::cout << "invalid diffuse texture path" << std::endl;
                }
            }
            else
            {
                std::cout << "No normal texture: " << directory << std::endl;
            }

            if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
            {
                aiString path;
                if (material->GetTexture(aiTextureType_HEIGHT, 0, &path) == aiReturn_SUCCESS)
                {
                    size_t idx = std::string(path.data).rfind("\\");
                    std::string filename = std::string(path.data).substr(idx + 1);

                    std::cout << "Found height texture: " << filename << std::endl;

                    Texture texture(RendererDevice, directory + filename, TextureType::HEIGHT);
                    meshMat.SetHeightMap(texture);
                }
                else
                {
                    std::cout << "invalid diffuse texture path" << std::endl;
                }
            }
            else
            {
                std::cout << "No height texture: " << directory << std::endl;
            }
        }
    }
}
