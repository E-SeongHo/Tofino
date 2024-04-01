#include <directxtk/SimpleMath.h>

#include "Object.h"

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;

using namespace std;

Object::Object(const std::string name, const bool isHittable)
	: m_constBuffer(ConstantBuffer<ModelBuffer>(VERTEX_SHADER | PIXEL_SHADER, 0))
{
	hitEnabled = isHittable;
	m_name = name;
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
}

ConstantBuffer<ModelBuffer>& Object::GetConstBuffer()
{
	return m_constBuffer;
}

void Object::UpdateBuffer(ComPtr<ID3D11DeviceContext>& context)
{
	m_constBuffer.Update(context);
	for (auto& mesh : m_meshes)
	{
		if (mesh.IsUpdateFlagSet()) mesh.UpdateBuffer(context);
	}

	m_updateFlag = false;
}

void Object::SetMeshMaterialFactors(DirectX::SimpleMath::Vector4 baseColor, const float roughness, const float metallic, const int partNumber)
{
	if (partNumber >= 0 && partNumber < m_meshes.size())
	{
		m_meshes[partNumber].SetMaterialFactors(baseColor, roughness, metallic);
	}
}

void Object::SetAllMeshMaterialFactors(const DirectX::SimpleMath::Vector4 baseColor, const float roughness, const float metallic)
{
	for (auto& mesh : m_meshes)
	{
		mesh.SetMaterialFactors(baseColor, roughness, metallic);
	}
}

std::vector<Mesh>& Object::GetMeshes()
{
	return m_meshes;
}
