#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "headers/shader.h"
#include "headers/stb_image.h"
#include "headers/camera.h"



const int INITAIL_WINDOW_WIDTH = 800;
const int INITAIL_WINDOW_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callBack(GLFWwindow* window, double xpos, double ypos);
void scroll_callBack(GLFWwindow* window, double xOffSet, double yOffSet);
void drawCube();
void drawTriangle();

int windowWidth = INITAIL_WINDOW_WIDTH;
int windowHeight = INITAIL_WINDOW_HEIGHT;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX;
float lastY;
float fov = 45;
bool firstMouse = true;
Camera camera;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	GLFWwindow* window = glfwCreateWindow(INITAIL_WINDOW_WIDTH, INITAIL_WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callBack);
	glfwSetScrollCallback(window, scroll_callBack);

	if (window == NULL) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};


	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	stbi_set_flip_vertically_on_load(true);
	Shader epicShader("assets/shaders/simpleVertexShader.vert", "assets/shaders/simpleFragmentShader.frag");
	int width, height, nrChannels;
	unsigned char* data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);

	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	epicShader.use();
	epicShader.setInt("texture1", 0);
	
	glEnable(GL_DEPTH_TEST);
	stbi_image_free(data);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		epicShader.use();
		std::cout << std::string("FPS: ") << 1 / deltaTime << std::endl;

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.getViewMatrix();
		
		
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), ((float)windowWidth / windowHeight), 0.1f, 100.0f);


		epicShader.setMat4("view", view);
		epicShader.setMat4("projection", projection);

		glBindVertexArray(VAO);
	
		for (unsigned int x = 0; x < 16; x++) {
			for (unsigned int y = 0; y < 64; y++) {
				for (unsigned int z = 0; z < 16; z++) {
					glm::mat4 model = glm::mat4(1.0f);

					model = glm::translate(model, glm::vec3(x, y, z));
					model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
					model = glm::rotate(model, (float)glm::radians(glfwGetTime()), glm::vec3(x, y, z));

					epicShader.setMat4("model", model);
					drawCube();
				}
			}

			//drawTriangle();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void drawCube() {
	float cubeVertices[] = {
		// Positions		  // Texture coords
		-0.25f, -0.25f, -0.25f,  0.0f, 0.0f,
		 0.25f, -0.25f, -0.25f,  1.0f, 0.0f,
		 0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
		 0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
		-0.25f,  0.25f, -0.25f,  0.0f, 1.0f,
		-0.25f, -0.25f, -0.25f,  0.0f, 0.0f,

		-0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
		 0.25f, -0.25f,  0.25f,  1.0f, 0.0f,
		 0.25f,  0.25f,  0.25f,  1.0f, 1.0f,
		 0.25f,  0.25f,  0.25f,  1.0f, 1.0f,
		-0.25f,  0.25f,  0.25f,  0.0f, 1.0f,
		-0.25f, -0.25f,  0.25f,  0.0f, 0.0f,

		-0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
		-0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
		-0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
		-0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
		-0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
		-0.25f,  0.25f,  0.25f,  1.0f, 0.0f,

		 0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
		 0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
		 0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
		 0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
		 0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
		 0.25f,  0.25f,  0.25f,  1.0f, 0.0f,

		-0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
		 0.25f, -0.25f, -0.25f,  1.0f, 1.0f,
		 0.25f, -0.25f,  0.25f,  1.0f, 0.0f,
		 0.25f, -0.25f,  0.25f,  1.0f, 0.0f,
		-0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
		-0.25f, -0.25f, -0.25f,  0.0f, 1.0f,

		-0.25f,  0.25f, -0.25f,  0.0f, 1.0f,
		 0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
		 0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
		 0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
		-0.25f,  0.25f,  0.25f,  0.0f, 0.0f,
		-0.25f,  0.25f, -0.25f,  0.0f, 1.0f
	};
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void drawTriangle() {
	float vertices[] = {
		// Positions     	// Texture Coords
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	};

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processInput(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processInput(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processInput(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processInput(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processInput(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.processInput(DOWN, deltaTime);
}

void mouse_callBack(GLFWwindow* window, double xpos, double ypos) {
	std::cout << xpos << ", " << ypos << std::endl;
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	camera.processMouseInput(xOffset, yOffset);
}

void scroll_callBack(GLFWwindow* window, double xOffSet, double yOffSet) {
	fov -= (float)yOffSet;
	if (fov < 1.0f) {
		fov = 1.0f;
	} 
	if (fov > 45.0f) {
		fov = 45.0f;
	}
}