
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Camera.h"

using namespace DirectX;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;


Camera::Camera() :
	m_origin(Vector3(0.0f, 0.0f, -5.0f)),
	m_direction(Vector3(0.0f, 0.0f, 1.0f)),
	m_up(Vector3(0.0f, 1.0f, 0.0f)),
	m_nearZ(0.01f), m_farZ(100.0f), m_fovY(70.0f),
	m_aspect(16.0f / 9.0f)
{
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

void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}
