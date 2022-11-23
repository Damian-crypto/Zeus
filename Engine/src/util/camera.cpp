#include "corepch.h"
#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace zeus
{
	Camera::Camera()
	{
		m_CameraProps.WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraProps.Yaw = YAW;
		m_CameraProps.Pitch = PITCH;
		m_CameraProps.Roll = 0.0f;
		m_CameraProps.MouseSensitivity = SENSITIVITY;
		m_CameraProps.MovementSpeed = SPEED;
		m_CameraProps.ZoomLevel = ZOOM;
		m_CameraProps.Position = glm::vec3(1.0f);
		m_CameraProps.Direction = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CameraProps.Right = glm::vec3(0.0f);
		m_CameraProps.CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraProps.Type = CameraType::Orthographic;

		UpdateCamera();
	}

	Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
	{
		m_CameraProps.WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraProps.Yaw = YAW;
		m_CameraProps.Pitch = PITCH;
		m_CameraProps.Roll = 0.0f;
		m_CameraProps.MouseSensitivity = SENSITIVITY;
		m_CameraProps.MovementSpeed = SPEED;
		m_CameraProps.ZoomLevel = ZOOM;
		m_CameraProps.Position = glm::vec3(1.0f);
		m_CameraProps.Direction = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CameraProps.Right = glm::vec3(0.0f);
		m_CameraProps.CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraProps.Type = CameraType::Orthographic;

		UpdateCamera();
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		//UpdateCamera();
		const auto& eye = m_CameraProps.Position;
		const auto& center = m_CameraProps.Direction;
		const auto& up = m_CameraProps.CameraUp;
		return glm::lookAt(eye, eye + center, up);
		//return glm::inverse(glm::lookAt(eye, eye + center, up));
	}

	glm::mat4 Camera::GetProjectionMatrix(ProjectionProperties props)
	{
		glm::mat4 projection{ 1.0f };
		switch (m_CameraProps.Type)
		{
			case CameraType::Orthographic:
			{
				projection = glm::ortho(props.Left, props.Right, props.Bottom, props.Top, props.Near, props.Far);
				break;
			}
			case CameraType::Perspective:
			{
				float width = props.Right - props.Left;
				float height = props.Top - props.Bottom;
				projection = glm::perspective(glm::radians(m_CameraProps.ZoomLevel), width / height, props.Near, props.Far);
				break;
			}
		}

		return projection;
	}

	void Camera::ProcessKeyboard(CameraMovement dir, float dt)
	{
		float velocity = m_CameraProps.MovementSpeed * dt;
		switch (dir)
		{
		case CameraMovement::UP:
			m_CameraProps.Position += m_CameraProps.CameraUp * velocity;
			break;
		case CameraMovement::DOWN:
			m_CameraProps.Position -= m_CameraProps.CameraUp * velocity;
			break;
		case CameraMovement::LEFT:
			m_CameraProps.Position -= m_CameraProps.Right * velocity;
			break;
		case CameraMovement::RIGHT:
			m_CameraProps.Position += m_CameraProps.Right * velocity;
			break;
		case CameraMovement::FORWARD:
			m_CameraProps.Position += m_CameraProps.Direction * velocity;
			break;
		case CameraMovement::BACKWARD:
			m_CameraProps.Position -= m_CameraProps.Direction * velocity;
			break;
		}
	}

	void Camera::ProcessMouse(float x, float y, bool contraintPitch)
	{
		x *= m_CameraProps.MouseSensitivity;
		y *= m_CameraProps.MouseSensitivity;

		m_CameraProps.Yaw += x;
		m_CameraProps.Pitch += y;

		if (contraintPitch)
		{
			if (m_CameraProps.Pitch > 89.0f)
				m_CameraProps.Pitch = 89.0f;
			if (m_CameraProps.Pitch < -89.0f)
				m_CameraProps.Pitch = -89.0f;
		}

		UpdateCamera();
	}

	void Camera::ProcessMouseScroll(float y, bool constraintScroll)
	{
		m_CameraProps.ZoomLevel -= y;
		if (constraintScroll)
		{
			if (m_CameraProps.ZoomLevel < 1.0f)
				m_CameraProps.ZoomLevel = 1.0f;
			if (m_CameraProps.ZoomLevel > 45.0f)
				m_CameraProps.ZoomLevel = 45.0f;
		}
	}

	float Camera::GetZoomLevel()
	{
		return m_CameraProps.ZoomLevel;
	}

	CameraProperties& Camera::GetProperties()
	{
		return m_CameraProps;
	}

	void Camera::UpdateCamera()
	{
		glm::vec3 newFront(0.0f);
		newFront.x = cos(glm::radians(m_CameraProps.Yaw)) * cos(glm::radians(m_CameraProps.Pitch));
		newFront.y = sin(glm::radians(m_CameraProps.Pitch));
		newFront.z = sin(glm::radians(m_CameraProps.Yaw)) * cos(glm::radians(m_CameraProps.Pitch));
		m_CameraProps.Direction = glm::normalize(newFront);
		m_CameraProps.Right = glm::normalize(glm::cross(m_CameraProps.Direction, m_CameraProps.WorldUp));
		m_CameraProps.CameraUp = glm::normalize(glm::cross(m_CameraProps.Right, m_CameraProps.Direction));
	}
}
