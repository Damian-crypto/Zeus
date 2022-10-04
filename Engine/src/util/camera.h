#pragma once

#include "glm/glm.hpp"

static constexpr float YAW			= -90.0f;
static constexpr float PITCH		= 0.0f;
static constexpr float SPEED		= 2.5f;
static constexpr float SENSITIVITY	= 0.1f;
static constexpr float ZOOM			= 45.0f; // field of view angle

namespace zeus
{
	enum class CameraMovement
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
		FORWARD,
		BACKWARD
	};

	enum class CameraType
	{
		Orthographic,
		Perspective
	};

	struct CameraProperties
	{
		// Camera attributes
		glm::vec3 Position;
		glm::vec3 Direction;
		glm::vec3 CameraUp;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		// Euler angles
		float Yaw;
		float Pitch;
		float Roll;

		// Camera options
		float MovementSpeed;
		float MouseSensitivity;
		float ZoomLevel;

		// Camera type
		CameraType Type;
	};

	struct ProjectionProperties
	{
		float Left;
		float Right;
		float Bottom;
		float Top;
		float Near;
		float Far;
	};

	class Camera
	{
	private:
		CameraProperties m_CameraProps;

	public:
		Camera();
		Camera(glm::vec3 position, glm::vec3 worldUp, float yaw = YAW, float pitch = PITCH);

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix(ProjectionProperties props);
		void ProcessKeyboard(CameraMovement dir, float dt);
		void ProcessMouse(float x, float y, bool contraintPitch = true);
		void ProcessMouseScroll(float y, bool constraintScroll = true);
		float GetZoomLevel();
		CameraProperties& GetProperties();

	private:
		void UpdateCamera();
	};
}
