#include "Cube.h"
#include "Quad.h"
using namespace glm;

static const Quad sides[6]{
	Quad(vec3(-1,-1, 1), vec3( 2, 0, 0), vec3( 0, 2, 0)), // front
	Quad(vec3( 1,-1,-1), vec3(-2, 0, 0), vec3( 0, 2, 0)), // back
	Quad(vec3(-1,-1,-1), vec3( 0, 0, 2), vec3( 0, 2, 0)), // left
	Quad(vec3( 1,-1, 1), vec3( 0, 0,-2), vec3( 0, 2, 0)), // right
	Quad(vec3(-1, 1, 1), vec3( 2, 0, 0), vec3( 0, 0,-2)), // top
	Quad(vec3(-1,-1,-1), vec3( 2, 0, 0), vec3( 0, 0, 2))  // bottom
};

bool Cube::intersect(const Ray &ray, Interval<float> &tInterval, Hit &hit) const
{
	bool result = false;
	for (auto& s : sides)
	{
		result |= s.intersect(ray, tInterval, hit);
	}
	return result;
}