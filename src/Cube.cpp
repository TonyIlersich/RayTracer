#include "constants.h"
#include "Cube.h"
#include "surfaces.h"
using namespace glm;

static const mat4 sideTrans[6]{
	inverse(translate(identityTrans, upward)), // top
	inverse(translate(scale(identityTrans, vec3(1,-1,1)), upward)), // bottom
	inverse(translate(rotate(identityTrans, tau * 0.25f, forward), upward)), // left
	inverse(translate(rotate(identityTrans, tau * -0.25f, forward), upward)), // right
	inverse(translate(rotate(identityTrans, tau * 0.25f, rightward), upward)), // front
	inverse(translate(rotate(identityTrans, tau * -0.25f, rightward), upward)), // back
};

Surface::SurfaceType Cube::getSurfaceType() const
{
	return SurfaceType::Cube;
}

bool Cube::intersect(const Ray &ray, Interval<float> &tInterval, Hit &hit) const
{
	bool result = false;
	for (auto& trans : sideTrans)
	{
		const Ray localRay = trans * ray;
		if(surfaces::quad.intersect(localRay, tInterval, hit))
		{
			result = true;
			hit.applyTransform(trans);
		}
	}
	return result;
}