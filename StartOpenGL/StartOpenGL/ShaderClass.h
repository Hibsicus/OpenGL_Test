#pragma once

#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glew.h>

class ShaderClass
{
public:
	GLuint Program;
	ShaderClass(const GLchar* vertexPath, const GLchar* fragmentPath);
};
#endif