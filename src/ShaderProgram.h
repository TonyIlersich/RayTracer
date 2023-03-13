#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "includes.h"
#include "utils.h"

class ShaderProgram
{
public:
	ShaderProgram();

	void bind() const;
	bool getIsBad() const;

	template <typename T>
	void setUniform(const std::string& name, const T& value);

	void addShader(const char* source, GLenum type);
	void link();
	
	GLuint id; // TODO: this is temporarily public! it should be private
private:
	void deleteAddedShaders();

	bool isBad;
	std::vector<GLuint> addedShaders;
};

template <typename T>
inline void ShaderProgram::setUniform(const std::string& name, const T& value)
{
	static_assert("Unsupported uniform data type");
}

template <>
inline void ShaderProgram::setUniform(const std::string& name, const int& value)
{
	glGetError();
	GLint loc = glGetUniformLocation(id, name.c_str());
	if (glGetError() || loc == -1)
	{
		std::cerr << "Could not find uniform \"" << name << "\"\n";
		return;
	}
	glUniform1i(loc, value);
	if (GLint error = glGetError())
	{
		std::cerr << "GL error " << std::hex << error << ": Could not assign uniform \"" << name << "\" to value: " << value << '\n';
		return;
	}
}

template <>
inline void ShaderProgram::setUniform(const std::string& name, const float& value)
{
	glGetError();
	GLint loc = glGetUniformLocation(id, name.c_str());
	if (glGetError() || loc == -1)
	{
		std::cerr << "Could not find uniform \"" << name << "\"\n";
		return;
	}
	glUniform1f(loc, value);
	if (GLint error = glGetError())
	{
		std::cerr << "GL error " << std::hex << error << ": Could not assign uniform \"" << name << "\" to value: " << value << '\n';
		return;
	}
}

template <>
inline void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value)
{
	glGetError();
	GLint loc = glGetUniformLocation(id, name.c_str());
	if (glGetError() || loc == -1)
	{
		std::cerr << "Could not find uniform \"" << name << "\"\n";
		return;
	}
	glUniform3fv(loc, 1, value_ptr(value));
	if (GLint error = glGetError())
	{
		std::cerr << "GL error " << std::hex << error << ": Could not assign uniform \"" << name << "\" to value: " << value << '\n';
		return;
	}
}

template <>
inline void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value)
{
	glGetError();
	GLint loc = glGetUniformLocation(id, name.c_str());
	if (glGetError() || loc == -1)
	{
		std::cerr << "Could not find uniform \"" << name << "\"\n";
		return;
	}
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(value));
	if (GLint error = glGetError())
	{
		std::cerr << "GL error " << std::hex << error << ": Could not assign uniform \"" << name << "\" to value:\n" << value << '\n';
		return;
	}
}

#endif
