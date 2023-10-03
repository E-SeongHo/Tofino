#pragma once

#include <directxtk/SimpleMath.h>

class Camera
{
private:
	// Camera Internal Parameters for View Transform
	DirectX::SimpleMath::Vector3 m_origin;
	DirectX::SimpleMath::Vector3 m_direction;
	DirectX::SimpleMath::Vector3 m_up;
	DirectX::SimpleMath::Vector3 m_right; 

	// Camera External Parameters for Projection
	float m_nearZ;
	float m_farZ;
	float m_fovY;
	float m_aspect;
	float m_speed;

	// For Rotation
	float m_prevX, m_prevY; // NDC Coords

	// Run variables
	const float walkSpeed = 5.0f;
	const float walkFovY = 70.0f;
	

public:
	Camera();
	DirectX::SimpleMath::Vector3 GetOrigin();
	DirectX::SimpleMath::Matrix GetViewMatrix();
	DirectX::SimpleMath::Matrix GetProjectionMatrix();

	void RotateFromMouse(const float ndcX, const float ndcY);
	void MoveForward(float dt);
	void MoveRight(float dt);
	void SetRunVars(bool flag);

	void SetAspect(float aspect);
};
