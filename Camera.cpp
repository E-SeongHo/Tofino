#include <d3d11.h>
#include <d3dcompiler.h>

#include "Camera.h"
#include "Util.h"

using namespace DirectX;
using DirectX::SimpleMath::Quaternion;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;


Camera::Camera() :
	m_origin(Vector3(0.0f, 0.0f, 0.0f)),
	m_direction(Vector3(0.0f, 0.0f, 1.0f)),
	m_up(Vector3(0.0f, 1.0f, 0.0f)),
	m_nearZ(0.01f), m_farZ(1000.0f),
	m_aspect(16.0f / 9.0f),
	m_prevX(0.0f), m_prevY(0.0f)
{
	m_fovY = walkFovY;
	m_speed = walkSpeed;
	std::cout << m_fovY << std::endl;
}

Vector3 Camera::GetOrigin()
{
	return m_origin;
}

Matrix Camera::GetViewMatrix()
{
	return XMMatrixLookToLH(m_origin, m_direction, m_up);
}

Matrix Camera::GetProjectionMatrix()
{
	return XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ);
}

void Camera::RotateFromMouse(const float ndcX, const float ndcY)
{	// ndcX occurs Y Rotation, ndcY occurs X Rotation
	// TODO : Quaterion From To로 구현 변경
	// TODO : q, e 누를 시 배틀그라운드처럼 roll rotation 구현

	//m_direction = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), Matrix::CreateRotationY(ndcX * XM_2PI));
	//m_direction = Vector3::Transform(m_direction, Matrix::CreateRotationX(ndcY * -XM_PIDIV2)); // 반시계
	//std::cout << ndcX << ", " << ndcY << std::endl;
	//std::cout << m_direction.x << ", " << m_direction.y << ", " << m_direction.z << std::endl;
	//float yaw = ndcX * XM_2PI;
	float yaw = ndcX * XM_2PI; // y axis rotation
	float pitch = -(ndcY * XM_PIDIV2); // x axis rotation

	Quaternion q = Quaternion::CreateFromYawPitchRoll(Vector3(pitch, yaw, 0.0f));
	m_direction = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), Matrix::CreateFromQuaternion(q));
	m_up = Vector3::Transform(Vector3(0.0f, 1.0f, 0.0f), Matrix::CreateFromQuaternion(q));
	m_right = m_up.Cross(m_direction);
}

void Camera::MoveForward(float dt)
{
	m_origin += m_direction * dt * m_speed;
	// std::cout << m_origin.x << ", " << m_origin.y << ", " << m_origin.z << std::endl;
}

void Camera::MoveRight(float dt)
{
	m_origin += m_right * dt * m_speed;
}

void Camera::SetRunVars(bool flag)
{
	if (flag)
	{
		m_speed = walkSpeed * 2;
		//m_fovY = 60.0f;
	}
	else
	{
		m_speed = walkSpeed;
		//m_fovY = walkFovY;
	}
	//std::cout << m_speed << ", " << m_fovY << std::endl;
}
void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}
