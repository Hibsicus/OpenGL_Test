#pragma once
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CreateWindow
{
public:
	CreateWindow();
	~CreateWindow();

	int InitialWindow();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	static void mouse_callback(GLFWwindow* window, double xpose, double ypose);

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void CreateImage(const char* path, GLuint &texture);
	
	void Do_Movement();


private:
	int Width, Height;

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	GLuint texture1, texture2;

	const GLchar* vertexShaderSource = "#version 330 core \n"
		"layout (location = 0) in vec3 position;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";
	
	const GLchar* fragmentShaderSource = "#version 330 core \n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
};

