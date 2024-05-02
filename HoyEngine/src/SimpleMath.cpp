
#include "SimpleMath.h"
#include "Components.h"

#include <iostream>

namespace Tofino
{
	namespace Math
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

	std::ostream& operator<<(std::ostream& os, const Vector4& v)
	{
		os << "x: " << v.x << "y: " << v.y << "z: " << v.z << "w: " << v.w;
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Vector3& v)
	{
		os << "x: " << v.x << "y: " << v.y << "z: " << v.z;
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Matrix& m)
	{
		os << m._11 << "    " << m._12 << "    " << m._13 << "    " << m._14 << "\n";
		os << m._21 << "    " << m._22 << "    " << m._23 << "    " << m._24 << "\n";
		os << m._31 << "    " << m._32 << "    " << m._33 << "    " << m._34 << "\n";
		os << m._41 << "    " << m._42 << "    " << m._43 << "    " << m._44;
		return os;
	}
}


