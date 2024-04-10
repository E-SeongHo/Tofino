
#include "SimpleMath.h"
#include "Object.h"

namespace Tofino
{

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

	void Object::Bind(ComPtr<ID3D11DeviceContext>& context) const
	{
		m_constBuffer.Bind(context);
	}

	Matrix Object::GetWorldMatrix()
	{
		return m_constBuffer.GetData().world.Transpose();
	}

	void Object::UpdateWorldMatrix(Matrix worldRow)
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

	void Object::SetLocation(Vector3 location)
	{	// set position of bounding sphere as well
		m_transform.Location = location;
		m_boundingSphere.Center = location;

		Matrix m = GetWorldMatrix();
		m.Translation(location); // switch translation term
		UpdateWorldMatrix(m);
	}

	void Object::SetRotation(Vector3 rotation)
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

	void Object::SetScale(Vector3 scale)
	{
		m_transform.Scale = scale;
		Matrix m = GetWorldMatrix();
		m._11 = scale.x;
		m._22 = scale.y;
		m._33 = scale.z;

		// hack : will be completed after build BVH system
		float maxScale = std::max(std::max(scale.x, scale.y), scale.z);
		m_boundingSphere.Radius = m_boundingSphere.Radius * maxScale;

		std::cout << (int)m_boundingSphere.Radius << std::endl;

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

	void Object::SetMeshMaterialFactors(Vector4 baseColor, const float roughness, const float metallic, const int partNumber)
	{
		if (partNumber >= 0 && partNumber < m_meshes.size())
		{
			m_meshes[partNumber].SetMaterialFactors(baseColor, roughness, metallic);
		}
	}

	void Object::SetAllMeshMaterialFactors(const Vector4 baseColor, const float roughness, const float metallic)
	{
		for (auto& mesh : m_meshes)
		{
			mesh.SetMaterialFactors(baseColor, roughness, metallic);
		}
	}

	void Object::SetUpdateFlag(bool flag)
	{
		m_updateFlag = flag;
	}

	bool Object::IsUpdateFlagSet() const
	{
		return m_updateFlag;
	}

	std::string Object::GetName() const
	{
		return m_name;
	}

	std::vector<Mesh>& Object::GetMeshes()
	{
		return m_meshes;
	}
}