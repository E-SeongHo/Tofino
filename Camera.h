#pragma once

#include <directxtk/SimpleMath.h>

class Camera
{
private:
	// Camera Internal Parameters for View Transform
	DirectX::SimpleMath::Vector3 m_origin;
	DirectX::SimpleMath::Vector3 m_direction;
	DirectX::SimpleMath::Vector3 m_up;
	// DirectX::SimpleMath::Vector3 m_right; // TODO :: Move 구현시 사용

	// Camera External Parameters for Projection
	float m_nearZ;
	float m_farZ;
	float m_fovY;
	float m_aspect;

public:
	Camera();
	DirectX::SimpleMath::Vector3 GetOrigin();
	DirectX::SimpleMath::Matrix GetViewMatrix();
	DirectX::SimpleMath::Matrix GetProjectionMatrix();

	void SetAspect(float aspect);
};
