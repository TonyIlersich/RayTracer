#include "ShaderProgram.h"
using namespace std;

ShaderProgram::ShaderProgram(const char *vertSource, const char* fragSource):
	id(0), isBad(false)
{
	int success;
	char infoLog[1024];

	// Load and compile vertex shader
	GLuint vertId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertId, 1, &vertSource, NULL);
	glCompileShader(vertId);

	// Check for compile errors
	glGetShaderiv(vertId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		isBad = true;
		glGetShaderInfoLog(vertId, sizeof(infoLog), NULL, infoLog);
		cerr << "Vertex shader compilation failed:\n" << infoLog;
	}

	// Load and compile fragment shader
	GLuint fragId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragId, 1, &fragSource, NULL);
	glCompileShader(fragId);

	// Check for compile errors
	glGetShaderiv(fragId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		isBad = true;
		glGetShaderInfoLog(fragId, sizeof(infoLog), NULL, infoLog);
		cerr << "Fragment shader compilation failed:\n" << infoLog;
	}

	// Attach shader to shader program
	id = glCreateProgram();
	glAttachShader(id, vertId);
	glAttachShader(id, fragId);
	glLinkProgram(id);

	// Check for link errors
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		isBad = true;
		glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
		cerr << "Shader program linking failed:\n" << infoLog;
	}

	glDeleteShader(vertId);
	glDeleteShader(fragId);
}

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
