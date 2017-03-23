#pragma once
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

class CreateWindow
{
public:
	CreateWindow();
	~CreateWindow();

	int InitialWindow();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

private:
	int Width, Height;

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

