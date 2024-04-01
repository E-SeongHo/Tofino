#include <DirectXMesh.h>

#include "Model.h"
#include "Texture.h"
#include "Graphics.h"

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;
using namespace std;

Model::Model(const std::string name, const bool isHittable) : Object(name, isHittable)
{
}

void Model::LoadModel(const std::string& filename, const float scale)
{
	cout << "Loading model..." <<  m_name << " " << filename << endl;

	size_t idx = filename.rfind("/");
	m_directory = filename.substr(0, idx + 1);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!scene)
	{
		std::cout << "Failed to load model ( " << filename << " ) " << importer.GetErrorString() << std::endl;
		return;
	}
    
    Matrix tr; // Initial transformation
	LoadNode(scene->mRootNode, scene, tr);

	cout << m_name << " Total meshes : " << m_meshes.size() << endl;
	for (int i = 0; i < m_meshes.size(); i++)
	{
		auto& mat = m_meshes[i].GetMaterial();
		cout << "Mesh[ " << i << " ]" << endl;
		cout << "albedo map : " << mat.GetMaterialStatus().hasAlbedoMap << endl;
		cout << "normal map : " << mat.GetMaterialStatus().hasNormalMap<< endl;
		cout << "height map : " << mat.GetMaterialStatus().hasHeightMap << endl;
	}

	NormalizeVertices(scale);
}

void Model::NormalizeVertices(const float scale)
{
	using namespace DirectX;
	// Normalize Vertex
	Vector3 vmin(1000, 1000, 1000);
	Vector3 vmax(-1000, -1000, -1000);

	for (auto& mesh : m_meshes)
	{
		for (auto& v : mesh.m_vertexBuffer.GetData())
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
		for (auto& v : mesh.m_vertexBuffer.GetData())
		{
			v.pos.x = ((v.pos.x - cx) / dl) * scale;
			v.pos.y = ((v.pos.y - cy) / dl) * scale;
			v.pos.z = ((v.pos.z - cz) / dl) * scale;
		}
	}

	float r = XMMin(XMMin(dx, dy), dz);
	cout << "bounding radius " << r << endl;
	m_boundingSphere = BoundingSphere(Vector3(0.0f, 0.0f, 0.0f), scale / 2.0f);
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
	vector<DirectX::XMFLOAT3> positions(vertices.size());
	vector<DirectX::XMFLOAT3> normals(vertices.size());
	vector<DirectX::XMFLOAT2> texcoords(vertices.size());
	vector<DirectX::XMFLOAT4> tangents(vertices.size());

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

	m_meshes.push_back(Mesh(vertices, indices));
	Mesh& newMesh = m_meshes.back();

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		Material& meshMat = newMesh.m_material;

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS)
			{
				int idx = string(path.data).rfind("\\");
				string filename = string(path.data).substr(idx + 1);
				
				cout << "Found diffuse texture: " << filename << endl;

				Texture texture(Graphics::GetDevice(), m_directory + filename, TextureType::ALBEDO);
				meshMat.SetAlbedoMap(texture);

			}
			else
			{
				cout << "invalid diffuse texture path" << endl;
			}
		}
		else
		{
			cout << "No diffuse texture: " << m_directory << endl;
		}

		if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
		{
			aiString path;
			if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == aiReturn_SUCCESS)
			{
				size_t idx = string(path.data).rfind("\\");
				string filename = string(path.data).substr(idx + 1);

				cout << "Found normal texture: " << filename << endl;

				Texture texture(Graphics::GetDevice(), m_directory + filename, TextureType::NORMAL);
				meshMat.SetNormalMap(texture);

			}
			else
			{
				cout << "invalid diffuse texture path" << endl;
			}
		}
		else
		{
			cout << "No normal texture: " << m_directory << endl;
		}

		if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
		{
			aiString path;
			if (material->GetTexture(aiTextureType_HEIGHT, 0, &path) == aiReturn_SUCCESS)
			{
				size_t idx = string(path.data).rfind("\\");
				string filename = string(path.data).substr(idx + 1);
				
				cout << "Found height texture: " << filename << endl;

				Texture texture(Graphics::GetDevice(), m_directory + filename, TextureType::HEIGHT);
				meshMat.SetHeightMap(texture);
			}
			else
			{
				cout << "invalid diffuse texture path" << endl;
			}
		}
		else
		{
			cout << "No height texture: " << m_directory << endl;
		}
	}

}

