#include "CreateWindow.h"
#include <iostream>

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "ShaderClass.h"
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
static bool keys[1024];

static GLfloat lastX = 400, lastY = 300;
static GLfloat Pitch = 0.0f, Yaw = 0.0f;
static bool firstMouse = true;

static float fov = 45.0f;

CreateWindow::CreateWindow()
{

}


CreateWindow::~CreateWindow()
{
	
}

void CreateWindow::CreateImage(const char* path, GLuint &texture)
{


	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height,
		0, SOIL_LOAD_RGB);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int CreateWindow::InitialWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Fail to create OpenGL" << std::endl;
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	//鍵盤，滑鼠，滾輪註冊
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glfwGetFramebufferSize(window, &Width, &Height);
	glViewport(0, 0, Width, Height);

	ShaderClass* ourShader = new ShaderClass("..\\VertexShader.vs",
		"..\\FragmentShader.fs");

	ShaderClass* lightShader = new ShaderClass("..\\LightVertex.vs", "..\\LightFragment.fs");

	/*GLfloat vertices[] =
	{
		0.5f,  0.5f, 0.0f,  
		0.5f, -0.5f, 0.0f,  
		-0.5f, -0.5f, 0.0f,  
		-0.5f,  0.5f, 0.0f
	};
	*/

	glEnable(GL_DEPTH_TEST);

	lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

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

	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLfloat vertices[] = {
		// Positions          // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	GLuint VBO, VAO;// , EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	

	//位置屬性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//CreateImage("..\\Image\\container.jpg", texture1);
	//CreateImage("..\\Image\\awesomeface.png", texture2);
	//匯入圖片
	CreateImage("..\\Image\\container2.png", diffuseMap);
	CreateImage("..\\Image\\container2_specular.png", specularMap);

	

	while (!glfwWindowShouldClose(window))
	{

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		Do_Movement();


		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ourShader->Program);

		GLint viewPosLoc = glGetUniformLocation(ourShader->Program, "viewPos");
		GLint lightPosLoc = glGetUniformLocation(ourShader->Program, "light.position");

		glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

		//填充片段著色器裡定義的結構
		

		glUniform1i(glGetUniformLocation(ourShader->Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(ourShader->Program, "material.specular"), 1);

		//GLint matDiffuseLoc = glGetUniformLocation(ourShader->Program, "material.diffuse");
		//GLint matSpecularLoc = glGetUniformLocation(ourShader->Program, "material.specular");

		
		//glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
		//glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(ourShader->Program, "material.shininess"), 32.0f);

		//填充片段著色器裡的Light
		GLint lightAmbientloc = glGetUniformLocation(ourShader->Program, "light.ambient");
		GLint lightDiffuesLoc = glGetUniformLocation(ourShader->Program, "light.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(ourShader->Program, "light.specular");

		glUniform3f(lightAmbientloc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightDiffuesLoc, 0.5f, 0.5f, 0.5f);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

		glm::mat4 model, view, projection;

		projection = glm::perspective(fov, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);

		view = glm::lookAt(cameraPos,
			cameraPos + cameraFront,
			cameraUp);

		GLint modeLoc = glGetUniformLocation(ourShader->Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader->Program, "view");
		GLint projectLoc = glGetUniformLocation(ourShader->Program, "projection");

	
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//綁定圖片
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(VAO);
		glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glUseProgram(lightShader->Program);

		modeLoc = glGetUniformLocation(lightShader->Program, "model");
		viewLoc = glGetUniformLocation(lightShader->Program, "view");
		projectLoc = glGetUniformLocation(lightShader->Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		
		glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
//	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	delete ourShader;
	delete lightShader;
	return 0;
}

void CreateWindow::Do_Movement()
{
	GLfloat cameraSpeed = 5.0 * deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void CreateWindow::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void CreateWindow::mouse_callback(GLFWwindow* window, double xpose, double ypose)
{

	if (firstMouse)
	{
		lastX = xpose;
		lastY = ypose;
		firstMouse = false;
	}

	GLfloat xoffset = xpose - lastX;
	GLfloat yoffset = lastY - ypose;
	lastX = xpose;
	lastY = ypose;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(Yaw) * cos(glm::radians(Pitch)));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw) * cos(glm::radians(Pitch)));
	cameraFront = glm::normalize(front);
}

void CreateWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}