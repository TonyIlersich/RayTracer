#include "ShaderProgram.h"
using namespace std;

ShaderProgram::ShaderProgram():
	id(glCreateProgram()), isBad(false)
{}

void ShaderProgram::bind() const
{
	if (!isBad)
	{
		glUseProgram(id);
	}
}

bool ShaderProgram::getIsBad() const
{
	return isBad;
}

void ShaderProgram::addShader(const char* source, GLenum type)
{
	int success;
	char infoLog[1024];

	// Load and compile shader
	GLuint shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &source, NULL);
	glCompileShader(shaderId);

	// Check for compile errors
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		isBad = true;
		glGetShaderInfoLog(shaderId, sizeof(infoLog), NULL, infoLog);
		cerr << "Shader compilation failed:\n" << infoLog;
	}
	glAttachShader(id, shaderId);
}

void ShaderProgram::link()
{
	int success;
	char infoLog[1024];

	glLinkProgram(id);

	// Check for link errors
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		isBad = true;
		glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
		cerr << "Shader program linking failed:\n" << infoLog;
	}

	deleteAddedShaders();
}

void ShaderProgram::deleteAddedShaders()
{
	for (GLuint shaderId : addedShaders)
	{
		glDeleteShader(shaderId);
	}
	addedShaders.clear();
}
