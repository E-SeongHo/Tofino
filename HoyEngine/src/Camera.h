#pragma once

#include "SimpleMath.h"

namespace Tofino
{
	class Camera
	{
	public:
		Camera();
		Vector3 GetOrigin() const { return m_origin; }
		Vector3 GetDirection() const { return m_direction; }
		Vector3 GetRight() const { return m_right; }
		Vector3 GetUp() const { return m_up; }

		Matrix GetViewMatrix() const;
		Matrix GetProjectionMatrix() const;

		void SetAspect(float aspect) { m_aspect = aspect; }
		void SetRunVars(bool flag);

		void RotateFromMouse(const float ndcX, const float ndcY);
		void MoveForward(float dt) { m_origin += m_direction * dt * m_speed; }
		void MoveRight(float dt) { m_origin += m_right * dt * m_speed; }

	private:
		// Camera Internal Parameters for View Transform
		Vector3 m_origin;
		Vector3 m_direction;
		Vector3 m_up;
		Vector3 m_right;

		// Camera External Parameters for Projection
		float m_nearZ;
		float m_farZ;
		float m_fovY;
		float m_aspect;
		float m_speed;

		// For Rotation
		float m_yaw;
		float m_pitch;

		// Run variables
		bool m_isRun = false;
		const float walkSpeed = 5.0f;
		const float walkFovY = 70.0f;

	};

}