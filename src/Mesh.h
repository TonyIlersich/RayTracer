#ifndef MODEL_H
#define MODEL_H

#include "includes.h"
#include "ShaderProgram.h"

class Mesh
{
public:
	Mesh();
	
	void setAttributes(const std::vector<GLsizei>& sizes);
	void setVertices(const std::vector<float>& vertices);
	void setIndices(const std::vector<GLuint>& indices);
	void bind() const;
	void draw() const;

private:
	GLuint vbo, vao, ebo;
	std::vector<float> vertices;
	std::vector<GLuint> indices;
};

#endif