#pragma once

#include <directxtk/SimpleMath.h>

namespace Tofino
{
	using Vector4 = DirectX::SimpleMath::Vector4;
	using Vector3 = DirectX::SimpleMath::Vector3;
	using Vector2 = DirectX::SimpleMath::Vector2;
	using Color = DirectX::SimpleMath::Vector4;
	using Matrix = DirectX::SimpleMath::Matrix;
	using Quaternion = DirectX::SimpleMath::Quaternion;
	using Plane = DirectX::SimpleMath::Plane;
	using Ray = DirectX::SimpleMath::Ray;

	// Matrix operations
	struct TransformComponent;
	namespace Math
	{
		Matrix Rotator(const Vector3 rotation); 
		Matrix Transformer(const Tofino::TransformComponent&);

		void Translate(Matrix& inMatrix, const Vector3 translation);
		void Rotate(Matrix& inMatrix, const Vector3 rotation); // XYZ rotation order
		void Scale(Matrix& inMatrix, const Vector3 scale);
	}
	
}