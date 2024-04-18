
#include "SimpleMath.h"
#include "Components.h"

#include <iostream>

namespace Tofino::Math
{
	void Translate(Matrix& inMatrix, Vector3 translation)
	{
		inMatrix.Translation(translation);
	}

	void Rotate(Matrix& inMatrix, Vector3 rotation)
	{	// Rotates about X - Y - Z order

		const Matrix X = Matrix::CreateRotationX(DirectX::XMConvertToRadians(rotation.x));
		const Matrix Y = Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotation.y));
		const Matrix Z = Matrix::CreateRotationZ(DirectX::XMConvertToRadians(rotation.z));

		inMatrix = inMatrix * X * Y * Z;
	}

	void Scale(Matrix& inMatrix, Vector3 scale)
	{
		inMatrix._11 = scale.x;
		inMatrix._22 = scale.y;
		inMatrix._33 = scale.z;
	}

	Matrix Transformer(const Tofino::TransformComponent& transform)
	{
		Matrix m = Matrix(); // Identity Matrix

		Scale(m, transform.Scale);
		Rotate(m, transform.Rotation);
		Translate(m, transform.Translation);

		return m;
	}
}
