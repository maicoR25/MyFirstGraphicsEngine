#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "headers/shader.h"
#include "headers/camera.h"
#include "headers/model.h"
#include "headers/scene_object.h"
#include "gui/gui_manager.h"
#include "gui/panels/inspector_panel.h"

const int INITAIL_WINDOW_WIDTH = 800;
const int INITAIL_WINDOW_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callBack(GLFWwindow* window, double xpos, double ypos);
void scroll_callBack(GLFWwindow* window, double xOffSet, double yOffSet);
void key_callBack(GLFWwindow* window, int key, int scanCode, int action, int mods);

int windowWidth = INITAIL_WINDOW_WIDTH;
int windowHeight = INITAIL_WINDOW_HEIGHT;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX;
float lastY;
float fov = 45;
bool firstMouse = true;
bool cameraMovementEnabled = true;
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
	glfwSetKeyCallback(window, key_callBack);

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

	// Setup stbi flags
	stbi_set_flip_vertically_on_load(true);
		
	glEnable(GL_DEPTH_TEST);

	// Compile Shaders
	Shader cubeShader("assets/shaders/cubeVertexShader.vert", "assets/shaders/lightingShader.frag");
	Shader lightShader("assets/shaders/simpleVertexShader.vert", "assets/shaders/lightSourceShader.frag");
	Shader modelShader("assets/shaders/modelVertexShader.vert", "assets/shaders/modelFragmentShader.frag");

	// Meshes/Models Chapter

	Model backpack("assets/models/backpack/backpack.obj");
	Model cube("assets/models/cube.obj");
	Model ayaya("assets/models/ayaka/body.obj");


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	GUIManager guiManager(window);

	guiManager.AddPanel(std::make_unique<InspectorPanel>());

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput(window);

		guiManager.CreateNewFrame();
		
		//guiManager.DrawDemoWindow();
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		//std::cout << std::string("FPS: ") << 1 / deltaTime << std::endl;

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.getViewMatrix();
		
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), ((float)windowWidth / windowHeight), 0.1f, 100.0f);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f));

		modelShader.use();
		modelShader.setMat4("model", model);
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);
		backpack.Draw(modelShader);

		//model = glm::translate(model, cubePositions[1]);
		//modelShader.setMat4("model", model);
		//ayaya.Draw(modelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		cubeShader.use();
		cubeShader.setVec3("objectColor", 0.2f, 0.5f, 0.0f);
		cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		cubeShader.setVec3("material.ambient", 0.0f, 0.5f, 0.31f);
		cubeShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		cubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		cubeShader.setFloat("material.shininess", 32.0f);
		cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		cubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
		cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		cubeShader.setVec3("light.position", 1.0f, 2.0f, 1.0f);
		cubeShader.setVec3("viewPos", camera.Position);
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", projection);
		cube.Draw(cubeShader);

		//lightShader.use();
		//lightShader.setMat4("model", model);
		//lightShader.setMat4("view", view);
		//lightShader.setMat4("projection", projection);
		//glBindVertexArray(lightVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		guiManager.DrawGUI();

		glfwSwapBuffers(window);
	}

	
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
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
	if (!cameraMovementEnabled) {
		return;
	}
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

void key_callBack(GLFWwindow* window, int key, int scanCode, int action, int mods) {
	if (key == GLFW_KEY_LEFT_ALT) {
		if (action == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			if (cameraMovementEnabled == true) {
				glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
				cameraMovementEnabled = false;
			}
			firstMouse = true;
		}
		else if (action == GLFW_RELEASE) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
			cameraMovementEnabled = true;
		}
	}
}
