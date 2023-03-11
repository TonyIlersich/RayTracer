#include "ComputeShader.h"
using namespace std;

ComputeShader::ComputeShader(const char *source):
	id(0)
{
	int success;
	char infoLog[512];

	GLuint shaderId = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shaderId, 1, &source, NULL);
	glCompileShader(shaderId);

	// Check for compile errors
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, sizeof(infoLog), NULL, infoLog);
		glDeleteShader(shaderId);
		cerr << "Compute shader compilation failed:\n" << infoLog;
		return;
	}

	// shader Program
	id = glCreateProgram();
	glAttachShader(id, shaderId);
	glLinkProgram(id);
	glDeleteShader(shaderId);

	// Check for link errors
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
		cerr << "Compute shader program linking failed:\n" << infoLog;
		return;
	}
}

void ComputeShader::bind() const
{
	glUseProgram(id);
}
