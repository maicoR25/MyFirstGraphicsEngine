#pragma once
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};



class Camera {
public:
	float speed = 2.5f;
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float pitch;
	float yaw;
	float sensitivity;

	Camera() {
		Position = glm::vec3(0.0f, 0.0f, 0.0f);
		WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		sensitivity = 0.1f;
		pitch = 0.0f;
		yaw = -90.0f;
		updateCameraVectors();
	}

	

	void processInput(Camera_Movement direction, float deltaTime) {
		float cameraSpeed = speed * deltaTime;
		if (direction == FORWARD)
			Position += cameraSpeed * Front;
		if (direction == BACKWARD)
			Position -= cameraSpeed * Front;
		if (direction == RIGHT)
			Position += Right * cameraSpeed;
		if (direction == LEFT)
			Position -= Right * cameraSpeed;
		//if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		//	speed += 0.05;
		//if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && cameraSpeed - 0.05 >= 0)
		//	speed -= 0.05;
		//std::cout << std::string("Camera Speed: ") << cameraSpeed << std::endl;
	}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	void processMouseInput(float xOffset, float yOffset, GLboolean constrainPitch = true) {
		xOffset *= sensitivity;
		yOffset *= sensitivity;
		yaw += xOffset;
		pitch += yOffset;

		if(constrainPitch) {
			if (pitch > 89.0f) {
				pitch = 89.0f;
			}
			if (pitch < -89.0f) {
				pitch = -89.0f;
			}
		}
		updateCameraVectors();
	}
private:
	void updateCameraVectors() {
		glm::vec3 Direction;
		Direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		Direction.y = sin(glm::radians(pitch));
		Direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		Front = glm::normalize(Direction);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};