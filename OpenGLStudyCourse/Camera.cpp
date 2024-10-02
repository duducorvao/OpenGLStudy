#include "Camera.h"

Camera::Camera() :
	position(glm::vec3(0.0f, 0.0f, 0.0f)),
	forward(glm::vec3(0.0f, 0.0f, 0.0f)),
	up(glm::vec3(0.0f, 0.0f, 0.0f)),
	right(glm::vec3(0.0f, 0.0f, 0.0f)),
	worldUp(glm::vec3(0.0f, 0.0f, 0.0f)),
	yaw(0.0f),
	pitch(0.0f),
	moveSpeed(0.0f),
	turnSpeed(0.0f)
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startPitch, 
	GLfloat startYaw, GLfloat startMoveSpeed, GLfloat startTurnSpeed) :
	position(startPosition),
	worldUp(startUp),
	pitch(startPitch),
	yaw(startYaw),
	moveSpeed(startMoveSpeed),
	turnSpeed(startTurnSpeed),
	forward(glm::vec3(0.0f, 0.0f, -1.0f)),
	right(glm::vec3(1.0f, 0.0f, 0.0f)),
	up(glm::vec3(0.0f, 0.0f, 0.0f))
{
	Update();
}

Camera::~Camera()
{
}

void Camera::KeyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += forward * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= forward * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}

	if (keys[GLFW_KEY_SPACE])
	{
		position += worldUp * velocity;
	}

	if (keys[GLFW_KEY_LEFT_CONTROL])
	{
		position -= worldUp * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	
	Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(position, position + forward, up);
}

glm::vec3 Camera::GetCameraPosition()
{
	return position;
}

glm::vec3 Camera::GetCameraDirection()
{
	return glm::normalize(forward);
}

void Camera::Update()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::cross(right, forward); // right and forward is already normalized
}
