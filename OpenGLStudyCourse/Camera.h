#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startPitch, GLfloat startYaw, 
		GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera();

	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);
	glm::mat4 CalculateViewMatrix();
	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraDirection();
	glm::vec3 GetCameraForward();
	glm::vec3 GetCameraUp();

private:
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat pitch;
	GLfloat yaw;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void Update();
};

