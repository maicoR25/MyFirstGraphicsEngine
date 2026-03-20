#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	ourColor = aColor;\n"
"}\0";
const char* squareShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(ourColor, 1.0);\n"
"}\0";
const char* triangleShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(0.5f, 0.0f, 0.5f, 1.0f);\n"
"}\0";

int  success;
char infoLog[512];


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void drawTriangle();

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	float vertices[] = {
		// Positions		// Colors
		 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
		//-0.5f,  0.5f, 0.0f, 0.5f, 0.5f, 0.5f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
		0, 1, 4,
		2, 3, 5
	};


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//Compile the shaders
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//Check if shader compiled properly
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &squareShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int triangleShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(triangleShader, 1, &triangleShaderSource, NULL);
	glCompileShader(triangleShader);
	glGetShaderiv(triangleShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(triangleShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// Create shader programs
	unsigned int squareShaderProgram = glCreateProgram();
	unsigned int triangleShaderProgram = glCreateProgram();

	// Attatch shaders to program and link them
	glAttachShader(squareShaderProgram, vertexShader);
	glAttachShader(squareShaderProgram, fragmentShader);

	glAttachShader(triangleShaderProgram, vertexShader);
	glAttachShader(triangleShaderProgram, triangleShader);

	glLinkProgram(squareShaderProgram);
	// Check if program created successfully
	glGetProgramiv(squareShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(squareShaderProgram, 512, NULL, infoLog);
	}
	glLinkProgram(triangleShaderProgram);

	// Check if program created successfully
	glGetProgramiv(triangleShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(triangleShaderProgram, 512, NULL, infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(triangleShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	

	while (!glfwWindowShouldClose(window)) {
		
		glUseProgram(squareShaderProgram);
		
	////	glUseProgram(triangleShaderProgram);
	//	/*float timeValue = glfwGetTime();
	//	float greenValue = sin(timeValue) / 2.0f + 0.5f;
	//	int vertexColorLocation = glGetUniformLocation(squareShaderProgram, "ourColor");*/
	//	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//drawTriangle();
		//glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void drawTriangle() {
	float triangleVertices[] = {
		0.75f,  0.75f, 0.0f,
		-1.75f, -0.75f, 0.0f,
		-0.75f, -0.75f, 0.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
