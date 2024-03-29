#include <directxtk/SimpleMath.h>

#include "Object.h"

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;
using namespace std;

Object::Object(const std::string name, const bool isHittable)
{
	hitEnabled = isHittable;
	m_name = name;

	m_constBuffer = ConstantBuffer<ModelBuffer>(VERTEX_SHADER | PIXEL_SHADER, 0);
}

void Object::Init(ComPtr<ID3D11Device>& device)
{	
	m_constBuffer.Init(device);

	for (auto& mesh : m_meshes)
	{
		mesh.Init(device);
	}
}

void Object::Render(ComPtr<ID3D11DeviceContext>& context)
{
	m_constBuffer.Bind(context);

	for (auto& mesh : m_meshes)
	{
		mesh.SetSRVs(context);
		mesh.Render(context);
	}
}

void Object::RenderNormal(ComPtr<ID3D11DeviceContext>& context)
{
	m_constBuffer.Bind(context);

	for (auto& mesh : m_meshes)
	{
		mesh.RenderNormal(context);
	}
}

DirectX::SimpleMath::Matrix Object::GetWorldMatrix()
{
	return m_constBuffer.GetData().world.Transpose();
}

void Object::UpdateWorldMatrix(DirectX::SimpleMath::Matrix worldRow)
{
	Matrix worldColumn = worldRow.Transpose();

	m_constBuffer.GetData().world = worldColumn;
	m_constBuffer.GetData().worldIT = worldColumn;
	m_constBuffer.GetData().worldIT.Translation(Vector3(0.0f));
	m_constBuffer.GetData().worldIT.Invert().Transpose();

	m_updateFlag = true;
}

//Transform& Object::GetTransform()
//{
//	return m_transform;
//}

void Object::SetLocation(DirectX::SimpleMath::Vector3 location)
{	// set position of bounding sphere as well
	m_transform.Location = location;
	m_boundingSphere.Center = location;

	Matrix m = GetWorldMatrix();
	m.Translation(location); // switch translation term
	UpdateWorldMatrix(m);
}

void Object::SetRotation(DirectX::SimpleMath::Vector3 rotation)
{	// Rotates about X - Y - Z order
	m_transform.Rotation = rotation;

	Matrix m = Matrix(); // Identity Matrix
	
	const Matrix X = Matrix::CreateRotationX(DirectX::XMConvertToRadians(rotation.x));
	const Matrix Y = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotation.y));
	const Matrix Z = Matrix::CreateRotationZ(DirectX::XMConvertToRadians(rotation.z));

	m = m * X * Y * Z;
	m.Translation(Vector3(m_transform.Location));
	UpdateWorldMatrix(m);
}

void Object::SetScale(DirectX::SimpleMath::Vector3 scale)
{
	m_transform.Scale = scale;
	Matrix m = GetWorldMatrix();
	m._11 = scale.x;
	m._22 = scale.y;
	m._33 = scale.z;

	UpdateWorldMatrix(m);
}

void Object::Update(float deltaTime)
{
	//if(m_updateFlag) UpdateBuffer()
}

ConstantBuffer<ModelBuffer>& Object::GetConstBuffer()
{
	return m_constBuffer;
}

void Object::UpdateBuffer(ComPtr<ID3D11DeviceContext>& context)
{
	m_constBuffer.Update(context);

	//Util::UpdateConstantBuffer(context, m_modelBufferCPU, m_modelBufferGPU);
	m_updateFlag = false;
}

void Object::SetMaterials(const float roughness, const float metallic, const DirectX::SimpleMath::Vector4 albedo)
{
	m_constBuffer.GetData().material.albedo = albedo;
	m_constBuffer.GetData().material.roughness = roughness;
	m_constBuffer.GetData().material.metallic = metallic;

	m_updateFlag = true;
}

void Object::AttachAlbedoTexture(std::string filename, int partNumber)
{	// match given texture file name with the mesh
	if (partNumber < m_meshes.size())
	{
		m_meshes[partNumber].m_diffuseFilename = filename;
	}
	else
	{
		std::cout << "Wrong part number. part number starts from 0 " << std::endl;
	}
}

void Object::AttachNormalTexture(std::string filename, int partNumber)
{	// match given texture file name with the mesh
	if (partNumber < m_meshes.size())
	{
		m_meshes[partNumber].m_normalFilename = filename;
	}
	else
	{
		std::cout << "Wrong part number. part number starts from 0 " << std::endl;
	}
}

void Object::AttachHeightTexture(std::string filename, int partNumber)
{	// match given texture file name with the mesh
	if (partNumber < m_meshes.size())
	{
		m_meshes[partNumber].m_heightFilename = filename;
	}
	else
	{
		std::cout << "Wrong part number. part number starts from 0 " << std::endl;
	}
}