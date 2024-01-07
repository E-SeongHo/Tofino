#include "Model.h"
#include "TextureLoader.h"

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;
using namespace std;

void Model::LoadModel(const std::string& filename)
{
	int idx = filename.rfind("/");
	m_directory = filename.substr(0, idx + 1);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!scene)
	{
		std::cout << "failed to load model ( " << filename << " ) " << importer.GetErrorString() << std::endl;
		return;
	}
    
    Matrix tr; // Initial transformation
	LoadNode(scene->mRootNode, scene, tr);
}

void Model::Init(ComPtr<ID3D11Device>& device)
{	// Load Textures, Create Buffers

	using namespace DirectX;
	// Normalize Vertex
	Vector3 vmin(1000, 1000, 1000);
	Vector3 vmax(-1000, -1000, -1000);
	
	for (auto& mesh : m_meshes) 
	{
		for (auto& v : mesh.m_vertices) 
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

	for (auto& mesh : m_meshes) 
	{
		for (auto& v : mesh.m_vertices) 
		{
			v.pos.x = (v.pos.x - cx) / dl;
			v.pos.y = (v.pos.y - cy) / dl;
			v.pos.z = (v.pos.z - cz) / dl;
		}
	}

	// for just make one shared constant buffer instead of same different constant buffers for every mesh
	m_constBufferCPU.world = Matrix();
	Util::CreateConstantBuffer(device, m_constBufferCPU, m_constBufferGPU);

	for (auto& mesh : m_meshes)
	{
		// diffuse texture
		TextureLoader::CreateTextureFromImage(device, mesh.m_diffuseFilename, mesh.m_diffuseSRV, true);
		
		// Create Buffers
		mesh.m_indexCount = (UINT)mesh.m_indices.size();
		mesh.m_constBufferCPU.world = Matrix();
		
		// mesh.CreateBuffers(device);
		Util::CreateVertexBuffer(device, mesh.m_vertices, mesh.m_vertexBuffer);
		Util::CreateIndexBuffer(device, mesh.m_indices, mesh.m_indexBuffer);
		mesh.m_constBufferGPU = m_constBufferGPU; // sharing resource
	}
}

void Model::Render(ComPtr<ID3D11DeviceContext>& context)
{
	for (auto& mesh : m_meshes)
	{
		mesh.SetSRVs(context);
		mesh.Render(context);
	}
}

DirectX::SimpleMath::Matrix Model::GetWorldMatrix()
{
	return m_constBufferCPU.world.Transpose();
}

void Model::UpdateWorldMatrix(DirectX::SimpleMath::Matrix worldRow)
{
	Matrix worldColumn = worldRow.Transpose();
	m_constBufferCPU.world = worldColumn;
	m_constBufferCPU.worldIT = worldColumn;
	m_constBufferCPU.worldIT.Translation(Vector3(0.0f));
	m_constBufferCPU.worldIT.Invert().Transpose();
}

void Model::UpdateBuffer(ComPtr<ID3D11DeviceContext>& context)
{
	Util::UpdateConstantBuffer(context, m_constBufferCPU, m_constBufferGPU);
}

void Model::LoadNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr)
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
        this->LoadMesh(mesh, scene);
    }

    for (UINT i = 0; i < node->mNumChildren; i++) 
    {
        this->LoadNode(node->mChildren[i], scene, m);
    }

}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	
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

		if (mesh->mTextureCoords[0])
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

	Mesh newMesh;
	newMesh.m_vertices = vertices;
	newMesh.m_indices = indices;

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE)) 
		{
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			int idx = string(path.data).rfind("\\");
			string filename = string(path.data).substr(idx + 1);

			newMesh.m_diffuseFilename = m_directory + filename;
		}
		else
		{
			cout << "No texture: " << m_directory << endl;
		}
	}

	m_meshes.push_back(newMesh);
}

