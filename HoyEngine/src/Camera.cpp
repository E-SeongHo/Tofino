#include <d3d11.h>
#include <d3dcompiler.h>
#include <algorithm>
#include <iostream>

#include "Camera.h"

namespace Tofino 
{
	Camera::Camera() :
		m_origin(Vector3(0.0f, 0.0f, -5.0f)),
		m_direction(Vector3(0.0f, 0.0f, 1.0f)),
		m_up(Vector3(0.0f, 1.0f, 0.0f)),
		m_nearZ(0.01f), m_farZ(1000.0f),
		m_aspect(16.0f / 9.0f),
		m_yaw(0.0f), m_pitch(0.0f)
	{
		m_fovY = walkFovY;
		m_speed = walkSpeed;
		// std::cout << m_fovY << std::endl;
	}

	Matrix Camera::GetViewMatrix() const
	{
		return DirectX::XMMatrixLookToLH(m_origin, m_direction, m_up);
	}

	Matrix Camera::GetProjectionMatrix() const
	{
		return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_fovY), m_aspect, m_nearZ, m_farZ);
	}

	void Camera::RotateFromMouse(const float ndcX, const float ndcY)
	{	// ndcX occurs Y Rotation, ndcY occurs X Rotation
		// TODO : q, e 누를 시 배틀그라운드처럼 roll rotation 구현

		//m_direction = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), Matrix::CreateRotationY(ndcX * XM_2PI));
		//m_direction = Vector3::Transform(m_direction, Matrix::CreateRotationX(ndcY * -XM_PIDIV2)); // 반시계
		//std::cout << ndcX << ", " << ndcY << std::endl;
		//std::cout << m_direction.x << ", " << m_direction.y << ", " << m_direction.z << std::endl;
		//float yaw = (ndcX - m_prevX) * XM_2PI;
		float dx = ndcX;
		float dy = ndcY;

		m_yaw += dx; // y axis rotation
		m_pitch += -(dy); // x axis rotation
		/*if (m_pitch > 89.0f) m_pitch = 89.0f;
		if (m_pitch < -89.0f) m_pitch = -89.0f;*/
		Quaternion q = Quaternion::CreateFromYawPitchRoll(Vector3(m_pitch, m_yaw, 0.0f));
		m_direction = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), Matrix::CreateFromQuaternion(q));
		m_up = Vector3::Transform(Vector3(0.0f, 1.0f, 0.0f), Matrix::CreateFromQuaternion(q));
		m_right = m_up.Cross(m_direction);
	}

	void Camera::SetRunVars(bool flag)
	{
		if (m_isRun != flag)
		{
			m_isRun = flag;
			if (m_isRun)
			{
				m_speed = walkSpeed * 2;
				m_fovY = 65.0f;
			}
			else
			{
				m_speed = walkSpeed;
				m_fovY = walkFovY;
			}
		}
	}

}