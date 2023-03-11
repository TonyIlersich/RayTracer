#include "Mesh.h"
using namespace std;
using namespace glm;

Mesh::Mesh()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Mesh::setAttributes(const vector<GLsizei>& sizes)
{
	void* offset = 0;
	GLsizei stride = 0;
	for (auto& a : sizes)
	{
		stride += a * sizeof(float);
	}
	for (int i = 0; i < sizes.size(); i++)
	{
		glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(i);
		offset = (uint8*)offset + sizes[i] * sizeof(float);
	}
	assert(offset == (void*)(size_t)stride);
}

void Mesh::setVertices(const vector<float>& vertices)
{
	this->vertices = vertices;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);
}

void Mesh::setIndices(const vector<GLuint>& indices)
{
	this->indices = indices;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->indices.size(), this->indices.data(), GL_STATIC_DRAW);
}

void Mesh::bind() const
{
	glBindVertexArray(vao);
}

void Mesh::draw() const
{
	glDrawElements(GL_TRIANGLES, (GLsizei)vertices.size(), GL_UNSIGNED_INT, 0);
}
