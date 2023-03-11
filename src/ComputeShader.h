#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H

#include "includes.h"

class ComputeShader
{
public:
	ComputeShader(const char* source);

	void bind() const;

private:
	GLuint id;
};

#endif
